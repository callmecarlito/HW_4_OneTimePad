#ifndef NETWORK_HELPERS
#define NETWORK_HELPERS

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

bool VerifiedClient(int connection_fd, char* client_id);
bool RecvMsg(int connection_fd, char* expected_msg);
void SendMsg(int connection_fd, char* message);
//char* RecvFileText(int connection_fd);
//void SendFileText(int connection_fd, char* file_name);

#endif