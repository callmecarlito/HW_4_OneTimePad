#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "network_helpers.h"



int main(int argc, char *argv[]){
    int port_number, socket_fd;
    struct sockaddr_in server_addr;
    struct hostent* server_info;
    char* plaintext = argv[1];
    char* keytext = argv[2];
    char* msg_buffer;
    uint32_t text_size;
    uint32_t recv_size;
    char* text;

    if(argc < 4){
        fprintf(stderr, "Invalid number of arguments\n");
        exit(1);        
    }

    port_number = atoi(argv[3]); //convert user entered port number to int

    //check that value entered can be converted to an int
    if(port_number == 0){
        fprintf(stderr, "otp_enc: Invalid port number entered\n");
        exit(1);
    } 
    //set up address struct for otp_enc_d
    memset((char*)&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_number);
    server_info = gethostbyname("localhost");
    if(server_info == NULL){
        fprintf(stderr,"otp_enc: Invalid host\n");
        exit(1);
    }
    //copy in address to struct for otp_enc_d
    memcpy((char*)&server_addr.sin_addr.s_addr, (char*)server_info->h_addr_list[0], server_info->h_length);

    //set up socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0); //create socket
    if(socket_fd < 0){
        perror("otp_enc listening socket set up error: ");
        exit(1);
    }

    //connect to otp_enc_d
    if(connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("otp_enc error connecting to server: ");
        exit(1);
    }

    //send verifying information to otp_enc_d
    SendMsg(socket_fd, "OTP_ENC");
    //receive response from otp_enc_D
    msg_buffer = RecvMsg(socket_fd);
    if(strcmp(msg_buffer, "success") == 0){
        free(msg_buffer);
    }
    else{
        free(msg_buffer);
        close(socket_fd);
        exit(1);
    }
    //send size of plaintext file
    text_size = FileSize(plaintext);
    if(text_size > 0){
        //send size of plaintext file to otp_enc_d
        SendSize(socket_fd, &text_size);
    }
    else{
        fprintf(stderr, "Invalid file size\n");
        close(socket_fd);
        exit(1);
    }
    //otp_enc_d confirms size of plaintext file to be sent
    //recv_size = RecvSize(socket_fd);
    if(RecvSize(socket_fd) == text_size){
        SendTextfile(socket_fd, plaintext, text_size);
    }
    else{
        fprintf(stderr, "otp_enc_d did not return correct file size\n");
        close(socket_fd);
        exit(1);
    }
    //receive message from otp_enc_d confirming receipt of file contents
    msg_buffer = RecvMsg(socket_fd);
    if(strcmp(msg_buffer, "success") == 0){
        free(msg_buffer);
    }
    else{
        free(msg_buffer);
        close(socket_fd);
        exit(1);
    }
    //send size of key_file
    text_size = FileSize(keytext);
    if(text_size > 0){
        //send size of keytext file to otp_enc_d
        SendSize(socket_fd, &text_size);
    }
    else{
        fprintf(stderr, "Invalid file size\n");
        close(socket_fd);
        exit(1);
    }
    //otp_enc_d confirms size of keytext file to be sent
    //recv_size = RecvSize(socket_fd);
    if(RecvSize(socket_fd) == text_size){
        SendTextfile(socket_fd, keytext, text_size);
    }
    else{
        fprintf(stderr, "otp_enc_d did not return correct file size\n");
        close(socket_fd);
        exit(1);
    }
    //receive message from otp_enc_d confirming receipt of file contents
    msg_buffer = RecvMsg(socket_fd);
    if(strcmp(msg_buffer, "success") == 0){
        free(msg_buffer);
    }
    else{
        free(msg_buffer);
        close(socket_fd);
        exit(1);
    }
    //receive encrypted plaintext message
    close(socket_fd);
    //send size of plaintext file
    //else
        //close connection
        //exit(1);

    //recv confirmation of size from otp_enc_d
        //if correct
            //send plaintext
        //else
            //close connection
            //exit(1);

    //recv success
        //if succcess
            //send size of key
    //recv confirmation of size

    //send key
    //recv success

    //close connection
    return 0;
}
