#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "network_helpers.h"

#define BACKLOG 5
#define MAX_PIDS 100

int main(int argc, char *argv[]){

    int port_number, socket_fd, connection_fd;
    socklen_t sz_client_info;
    //char msg_buf[256];
    struct sockaddr_in server_addr, client_addr;

    pid_t child_pid = -5;
    pid_t pids[MAX_PIDS];
    int child_pid_cnt = 0;


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
                    sleep(10);
                    close(connection_fd);
                        //if no match
                            //close connection socket
                            //print error to stderr
                            //break or continue
                    //send success message
                    //recv message containg plaintext file length
                    //send success message
                    //receive plaintext file contents from otp_enc
                    //send success message 
                        //receivee key file contents from otp_enc
                    //encode plaintext using key_file
                    //send ciphertext to otp_enc
                    //close connection socket
                default: //parent process
                    //sleep(1);
                    pids[child_pid_cnt++] = child_pid;
            }
        }
    }
    return 0;
}