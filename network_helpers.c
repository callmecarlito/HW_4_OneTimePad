#include "network_helpers.h"

/**********************************************************************
 * 
 **********************************************************************/
bool VerifiedClient(int connection_fd, char* expected_id){
    char* client_id;
    size_t buffer_sz = 16;
    ssize_t bytes_recvd = -5;

    client_id = (char*)calloc(buffer_sz, sizeof(char)); //alloc memory for message
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
    size_t buffer_sz = 256;
    ssize_t bytes_recvd = -5;

    in_msg = (char*)calloc(buffer_sz, sizeof(char)); //allocate memory for message
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
    if(textfile == NULL){
        perror("Error opening file: ");
        return 0;
    }
    fseek(textfile, 0L, SEEK_END);
    length = ftell(textfile); //
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
/**********************************************************************
 * 
 **********************************************************************/
char* RecvTextfile(int connection_fd, uint32_t size){
    ssize_t bytes_recv;
    uint32_t total_bytes = 0;
    char* text;
    size_t buffer_sz = 80000;

    text = (char*)calloc(buffer_sz, sizeof(char));
    while(total_bytes < size){
        bytes_recv = recv(connection_fd, &text[total_bytes], size - bytes_recv, 0);
        if(bytes_recv < 0){
            perror("Error receiving file: ");
            return NULL;
        }
        total_bytes += bytes_recv;
    }
    return text;
}
/**********************************************************************
 * 
 **********************************************************************/
void SendTextfile(int connection_fd, char* file_name, uint32_t size){
    ssize_t bytes_sent;
    uint32_t total_bytes = 0;
    char* text;
    size_t buffer_sz = 80000;
    FILE* textfile;

    text = (char*)calloc(buffer_sz, sizeof(char));
    textfile = fopen(file_name, "r");
    if(textfile == NULL){
        perror("Error opening file: ");
        return;
    }
    fgets(text, size, textfile);  
    if(text[size + 1] == '\n'){
        text[size + 1] = '\0';
    }
    while(size > 0){
        bytes_sent = send(connection_fd, &text[total_bytes], size, 0);
        if(bytes_sent == -1){
            perror("Error sending file: ");
            close(connection_fd);
            break;
        }
        total_bytes += bytes_sent;
        size -= bytes_sent;
    }
    fclose(textfile);
    free(text);
}
/**********************************************************************
 * 
 **********************************************************************/

/**********************************************************************
 * 
 **********************************************************************/