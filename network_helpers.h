#ifndef NETWORK_HELPERS
#define NETWORK_HELPERS

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

bool VerifiedClient(int connection_fd, char* client_id);
char* RecvMsg(int connection_fd);
void SendMsg(int connection_fd, void* message);
uint32_t FileSize(char* file);
uint32_t RecvSize(int connection_fd);
void SendSize(int connection_fd, uint32_t* size);

//char* RecvFileText(int connection_fd);
//void SendFileText(int connection_fd, char* file_name);
//void EncryptText();
//void DecryptText();

#endif