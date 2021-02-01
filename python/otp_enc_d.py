#!/usr/bin/env python3
import socket
import sys
import time

HEADERSIZE = 10

class ServerSocket():

    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.server_socket = None
    
    def start_server(self):

        try:
            #create AF_INET(IPv4), TCP socket stream
            self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            print("Server socket created")
        except OSError as e:
            print(f"Unable to create socket\n Error: {e}")
            self.server_socket = None
        
        try:
            self.server_socket.bind((self.host, self.port)) #bind socket to address
            print("Binding server socket")
            self.server_socket.listen(5)  #establishes listening socket - limited to 5 connect requests
            print("Listening for incoming connections")
        except OSError as e:
            print(f"Unable to bind socket\n Error: {e[0]}, {e[1]}")
            self.server_socket = None
        
        if s is None:
            print("Unable to establish server socket")
            sys.exit(1)

        while True:
            client_socket, clientaddr = self.server_socket.accept() #accept incoming connections
            with client_socket:
                print(f"Connected to: {clientaddr}")
                
                msg = "Welcome to the server!"
                # :<10 is the header length, left aligned(<)
                print(f'{len(msg):<{HEADERSIZE}}' + msg)
                # curly brackets allow us to embed a varialbe within another variable
                msg = f'{len(msg):<{HEADERSIZE}}' + msg
                
                client_socket.send(bytes(msg, "utf-8"))
                
                while True:
                    time.sleep(3)
                    msg = f'The time is: {time.time()}'
                    msg = f'{len(msg):<{HEADERSIZE}}' + msg
                    client_socket.send(bytes(msg, "utf-8"))        
        
#should use command line arg for port number
    #don't forget to cast as int
#should run in background or as a daemon
#should output error if there's a network error

#after connection is made:
    #verify client is otp_enc
    #then receive plaintext file and key from otp_enc
    #verify key passed should be equal in size to the plaintext

#server encrypts the plaintext and returns the ciphertext

if __name__ == "__main__":
    HOST = socket.gethostname()
    PORT = 5757  
    
    s = ServerSocket(HOST, PORT)
    s.start_server()