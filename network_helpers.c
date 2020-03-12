#include "network_helpers.h"

/**********************************************************************
 * 
 **********************************************************************/
bool VerifyConnection(int connection_fd, char* verification_msg){
    char client_msg[32];
    int chars_recvd = -5;

    memset(client_msg, '\0', sizeof(client_msg)); //clear memory
    chars_recvd = recv(connection_fd, client_msg, sizeof(client_msg) - 1, 0);
    
    if(strcmp(verification_msg, client_msg) == 0){
        char* message = "Connection successful";
        return true;
    }
    else if(chars_recvd < 0){
        perror("Error verifying connection: ");
    }
    else if(chars_recvd == 0){
        fprintf(stderr, "Unable to recv: Client closed connection\n");
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
void SendMsg(int connection_fd, char* message){

}
/**********************************************************************
 * 
 **********************************************************************/

/**********************************************************************
 * 
 **********************************************************************/