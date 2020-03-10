#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>



int main(int argc, char *argv[]){
    //set up address struct for process

    //set up socket

    //begin listening on socket

    //accept connection on socket
        //receive plaintext and key file names from otp_enc
        //encode plaintext using key_file
        //send ciphertext to otp_enc
        //close connection socket
    //close listening socket?? or leave open??
    return 0;
}