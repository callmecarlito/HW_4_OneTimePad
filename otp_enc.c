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
    if(RecvMsg(socket_fd, "success")){
        printf("CONNECTION TO OTP_ENC_D SUCCESSFUL\n");
    }

    //send size of plaintext file
    //recv confirmation of size from otp_enc_d

    //send plaintext
    //recv success

    //send size of key
    //recv confirmation of size

    //send key
    //recv success

    //close connection
    return 0;
}