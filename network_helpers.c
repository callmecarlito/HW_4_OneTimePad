#include "network_helpers.h"

/**********************************************************************
 * 
 **********************************************************************/
bool VerifiedClient(int connection_fd, char* expected_id){
    char client_id[32];
    ssize_t chars_recvd = -5;

    memset(client_id, '\0', sizeof(client_id)); //clear memory
    chars_recvd = recv(connection_fd, client_id, sizeof(client_id) - 1, 0);
    
    if(strcmp(expected_id, client_id) == 0){
        return true;
    }
    else if(chars_recvd < 0){
        perror("Error receiving client ID: ");
    }
    else if(chars_recvd == 0){
        fprintf(stderr, "Error receiving client ID: Client closed connection\n");
    }
    else{
        fprintf(stderr, "Verification does not match: Rejecting connection\n");
    }
    close(connection_fd);
    return false;
}
/**********************************************************************
 * 
 **********************************************************************/
bool RecvMsg(int connection_fd, char* expected_msg){
    char client_msg[32];
    ssize_t chars_recvd = -5;

    memset(client_msg, '\0', sizeof(client_msg)); //clear memory
    chars_recvd = recv(connection_fd, client_msg, sizeof(client_msg) - 1, 0);
    
    if(strcmp(expected_msg, client_msg) == 0){
        return true;
    }
    else if(chars_recvd < 0){
        perror("Error receiving message: ");
    }
    else if(chars_recvd == 0){
        fprintf(stderr, "Error receiving message: Connection closed\n");
    }
    close(connection_fd);
    return false;
}
/**********************************************************************
 * 
 **********************************************************************/
void SendMsg(int connection_fd, char* message){
    ssize_t chars_sent;

    chars_sent = send(connection_fd, message, strlen(message), 0);
    if(chars_sent == -1){
        perror("Error sending message: ");
        close(connection_fd);
    }
}
/**********************************************************************
 * 
 **********************************************************************/
//char* RecvFileText(int connection_fd){
//    
//}
/**********************************************************************
 * 
 **********************************************************************/
//void SendFileText(int connection_fd, char* file_name){
//    
//}