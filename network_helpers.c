#include "network_helpers.h"

/**********************************************************************
 * 
 **********************************************************************/
bool VerifiedClient(int connection_fd, char* expected_id){
    char* client_id;
    size_t length = 16;
    ssize_t bytes_recvd = -5;

    client_id = (char*)calloc(length, sizeof(char)); //alloc memory for message
    if(client_id == NULL){ //check that memory allocation was successful
        perror("Error verifying client: ");
        return false;
    }
    //receive client info
    bytes_recvd = recv(connection_fd, client_id, sizeof(client_id) - 1, 0);
    //if client matches expected client
    if(strcmp(expected_id, client_id) == 0){
        free(client_id);
        return true;
    }
    else if(bytes_recvd < 0){
        perror("Error receiving client ID: ");
    }
    else if(bytes_recvd == 0){
        fprintf(stderr, "Error receiving client ID: Client closed connection\n");
    }
    else{
        fprintf(stderr, "Verification does not match: Rejecting connection\n");
    }
    free(client_id);
    return false;
}
/**********************************************************************
 * 
 **********************************************************************/
char* RecvMsg(int connection_fd){
    char* in_msg;
    size_t length = 256;
    ssize_t bytes_recvd = -5;

    in_msg = (char*)calloc(length, sizeof(char)); //allocate memory for message
    if(in_msg == NULL){ //check that memory allocation was successful
        perror("Error receiving message: ");
        return NULL;
    }
    //receive message
    bytes_recvd = recv(connection_fd, in_msg, sizeof(in_msg) - 1, 0);
    if(bytes_recvd > 0){ 
        return in_msg;
    }
    else if(bytes_recvd < 0){
        perror("Error receiving message: ");
    }
    else if(bytes_recvd == 0){
        fprintf(stderr, "Error receiving message: Connection closed\n");
    }
    return NULL;
}
/**********************************************************************
 * 
 **********************************************************************/
void SendMsg(int connection_fd, void* message){
    ssize_t bytes_sent;

    bytes_sent = send(connection_fd, message, strlen(message), 0);
    if(bytes_sent == -1){
        perror("Error sending message: ");
        close(connection_fd);
    }
}
/**********************************************************************
 * 
 **********************************************************************/
uint32_t FileSize(char* file){
    uint32_t length;
    FILE* textfile;

    textfile = fopen(file, "r");
    fseek(textfile, 0L, SEEK_END);
    length = ftell(textfile) - 1; //
    //fseek(textfile, 0L, SEEK_SET);

    fclose(textfile);
    return length; 
}
/**********************************************************************
 * 
 **********************************************************************/
uint32_t RecvSize(int connection_fd){
    uint32_t size = 0;
    ssize_t bytes_recvd = -5;

    bytes_recvd = recv(connection_fd, &size, sizeof(uint32_t), 0);
    if(bytes_recvd < 0){
        perror("Error receiving size: ");
    }
    return size;   
}
/**********************************************************************
 * 
 **********************************************************************/
void SendSize(int connection_fd, uint32_t* size){
    ssize_t bytes_sent;

    bytes_sent = send(connection_fd, size, sizeof(uint32_t), 0);
    if(bytes_sent == -1){
        perror("Error sending message: ");
        close(connection_fd);
    }
}
//char* RecvFileText(int connection_fd){
//    
//}
/**********************************************************************
 * 
 **********************************************************************/
//void SendFileText(int connection_fd, char* file_name){
//    
//}