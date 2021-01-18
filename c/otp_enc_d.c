#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/wait.h>
#include "network_helpers.h"

#define BACKLOG 5
#define MAX_PIDS 5

static void altSIGTERM(int signo);
void CleanProcesses(int* child_pid_cnt);
//char* EncodeText(char* plaintext, char* keytext);

int child_pid_cnt = 0;

int main(int argc, char *argv[]){ 

    int port_number, socket_fd, connection_fd;
    socklen_t sz_client_info;
    struct sockaddr_in server_addr, client_addr;
    pid_t child_pid = -5;
    //char* msg_buffer;
    uint32_t ptext_size;
    char* ptext;
    uint32_t key_size;
    char* ktext;
    //char* enc_text;

    struct sigaction SIGTERM_action = {0};
    SIGTERM_action.sa_handler = altSIGTERM;
    sigfillset(&SIGTERM_action.sa_mask);
    SIGTERM_action.sa_flags = SA_RESTART;
    sigaction(SIGTSTP, &SIGTERM_action, NULL);

    if(argc < 2){
        fprintf(stderr, "otp_enc_d: Invalid number of arguments\n");
        exit(1);
    }

    port_number = atoi(argv[1]); //convert user entered port number to int

    if(port_number == 0){
        fprintf(stderr, "otp_enc_d: Invalid port number entered\n");
        exit(1);
    }    
    //set up address struct for server
    memset((char*)&server_addr, '\0', sizeof(server_addr)); //clear out memory of struct
    server_addr.sin_family = AF_INET; //create network capable socket
    server_addr.sin_port = htons(port_number); //htons() converts int from host byte order to network byte order
    server_addr.sin_addr.s_addr = INADDR_ANY; //allows any address for connection to server

    //set up socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0); //domain: IPV4; type: TCP; protocol: default  
    if(socket_fd < 0){
        perror("otp_enc_d listening socket set up error: ");
        exit(1);
    }
    //bind socket to address
    if(bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("otp_enc_d error binding socket: ");
        exit(1);
    }
    //begin listening on socket
    if(listen(socket_fd, BACKLOG) < 0){
        perror("otp_enc_d error listening on socket: ");
        exit(1);
    }
    //infinite loop to accept connections and pull from queued incoming connections
    while(1){
        //block accepting new connections until concurrent connections fall below 5
        while(child_pid_cnt == MAX_PIDS){
            CleanProcesses(&child_pid_cnt);
        }
        if(child_pid_cnt > 0){
            CleanProcesses(&child_pid_cnt);
        }
        sz_client_info = sizeof(client_addr);
        //accept connection on socket or block until one connects
        connection_fd = accept(socket_fd, (struct sockaddr*)&client_addr, &sz_client_info);
        if(connection_fd < 0){
            perror("otp_enc_d error accepting connection: ");
            continue;
        }
        else{
            //create child process
            child_pid = fork();
            switch(child_pid){
                case -1: //fork error
                    perror("otp_enc_d fork error: "); break;
                case 0: //child process
                    //recv message verifying id of otp_enc
                    if(VerifiedClient(connection_fd, "OTP_ENC")){
                        SendMsg(connection_fd, "success");
                    }
                    else{
                        close(connection_fd);
                        exit(2);
                        break;
                    }
                    //recv size of plaintext file from otp_enc
                    ptext_size = RecvSize(connection_fd);
                    if(ptext_size > 0){
                        //send message to otp_enc confirming size
                        SendSize(connection_fd, &ptext_size);
                    }
                    else{
                        fprintf(stderr, "File size of %d is invalid\n", ptext_size);
                        close(connection_fd);
                        exit(1);
                        break;
                    }
                    ptext = RecvTextfile(connection_fd, ptext_size);
                    if(ptext != NULL){
                        //printf("\n\n%s\n\n", ptext);
                        SendMsg(connection_fd, "success");
                    }
                    else{
                        fprintf(stderr, "File not received\n");
                        close(connection_fd);
                        exit(1);
                        break;
                    }
                    //recv size of keytext file from otp_enc
                    key_size = RecvSize(connection_fd);
                    if(key_size >= ptext_size){
                        //send message to otp_enc confirming size
                        SendSize(connection_fd, &key_size);
                    }
                    else{
                        fprintf(stderr, "Key size of %d is invalid\n", ptext_size);
                        close(connection_fd);
                        exit(1);
                        break;
                    }
                    ktext = RecvTextfile(connection_fd, key_size);
                    if(ptext != NULL){
                        //printf("\n\n%s\n\n", ktext);
                        SendMsg(connection_fd, "success");
                    }
                    else{
                        fprintf(stderr, "File not received\n");
                        close(connection_fd);
                        exit(1);
                        break;
                    }
                    //encode plaintext
                    //enc_text = EncodeText(ptext, ktext);
                    //send encoded text
                    free(ptext);
                    free(ktext);
                    close(connection_fd);
                    exit(0);
                    break;
                    //encode plaintext using key_file
                    //send ciphertext to otp_enc
                    //close connection socket
                default: //parent process
                    child_pid_cnt++;
            }
        }
    }
    return 0;
}
/**********************************************************************
 * altSIGTERM() - signal handler for SIGTERM
 **********************************************************************/
static void altSIGTERM(int signo){
    int status;
    //clean up child processes
    while(child_pid_cnt > 0){
        pid_t child_pid = waitpid(-1, &status, WNOHANG);
        if(child_pid > 0){
            child_pid_cnt--;
        }   
    }
    exit(0); //exit after clean up
}
/**********************************************************************
 * CleanProcesses() - checks for terminated child processes and cleans 
 * them up
 **********************************************************************/
void CleanProcesses(int* child_pid_cnt){
    int i, status;

    for(i = 0; i < *child_pid_cnt; i++){
        pid_t child_pid = waitpid(-1, &status, WNOHANG);
        if(child_pid > 0){
            (*child_pid_cnt)--;
        }
    }
}
/**********************************************************************
 * 
 **********************************************************************/

/***********************************************************************
char* EncodeText(char* plaintext, char* keytext){
    //enum char_set {A, 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 
    //              'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 
    //              'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' '};
    //enum char_set{A = 0, B = 1, C = 2, D, E, F, G, H, I,
    //              J, K, L, M, N, O, P, Q, R,
    //              S, T, U, V, X, Y, Z," " = 27};
    int i;
    enum char_set j;
    int* plaincode;
    int* keycode;
    int enc_length = strlen(plaintext);
    plaincode = calloc(enc_length, sizeof *plaincode);
    if(plaincode == NULL){
        perror("Error allocating memory for enc: ");
    }
    keycode = calloc(enc_length, sizeof *keycode);
    if(keycode == NULL){
        perror("Error allocating memory for enc: ");
    }

    for(i = 0; i < enc_length; i++){
        for(j = 'A'; j <= ' '; j++){
            if(plaintext[i] == j){
                plaincode[i] = j;
                break;
            }
        }
    }
    printf("%s[%d]\n", plaintext, enc_length);
    for(i = 0; i < enc_length; i++){
        printf("%d", plaincode[i]);
    }
    printf("[%d]\n", enc_length);
    
    free(plaincode);
    free(keycode);
    return NULL;
}
***********************************************************************/