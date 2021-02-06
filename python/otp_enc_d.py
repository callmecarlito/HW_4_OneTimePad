#!/usr/bin/env python3
import socket
import sys
import tqdm

HEADER_SIZE = 10
BUFFER_SIZE = 8

class OTPEncodeServer():

    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.server_socket = None
    
    def start_server(self):
        try:
            #create AF_INET(IPv4), TCP socket stream
            self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            print("[+]Server socket created")
        except OSError as e:
            print(f"[+]Unable to create socket\n Error: {e}")
            self.server_socket = None
        
        try:
            self.server_socket.bind((self.host, self.port)) #bind socket to address
            print("[+]Binding server socket")
            self.server_socket.listen(5)  #establishes listening socket - limited to 5 connect requests
            print("[+]Listening for incoming connections")
        except OSError as e:
            print(f"[+]Unable to bind socket\n Error: {e[0]}, {e[1]}")
            self.server_socket = None
        
        if s is None:
            print("[+]Unable to establish server socket")
            sys.exit(1)

        while True:
            conn_socket, clientaddr = self.server_socket.accept() #accept incoming connections

            with conn_socket:
                print(f"[+]Connected to: {clientaddr}")

                #receive plaintext
                plaintext = self.recv_data(conn_socket)
                print(f"[+]Plaintext: {plaintext}")
                #receive key
                 
                #check len(plaintext) == len(key)   
                #encode plaintext -> ciphertext
                #send ciphertext     

    def recv_data(self, conn_socket):
        full_data = ''
        new_data = True

        while True:
            data = conn_socket.recv(BUFFER_SIZE)
            if new_data:
                print(f"[+]New message length: {data[:HEADER_SIZE]}")
                data_len = int(data[:HEADER_SIZE])
                new_data = False

            full_data += data.decode("utf-8")
            if len(full_data)-HEADER_SIZE == data_len:
                print(f"[+]Message received: {full_data[HEADER_SIZE:]}")
                full_data  = full_data[HEADER_SIZE:]
                break

        return full_data
    
    def send_data(self, text):
        pass
    def encode_text(self, text1, text2):
        pass    
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
    
    s = OTPEncodeServer(HOST, PORT)
    s.start_server()