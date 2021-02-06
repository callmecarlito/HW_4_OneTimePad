#!/usr/bin/env python3
import os
import socket
import sys
import tqdm

HEADER_SIZE = 10
BUFFER_SIZE = 8

class OTPEncodeClient():

    def __init__(self, host, port):
        self.server_addr = (host, port)
        self.conn_socket = None
    
    def start_client(self):
        try:
            #create AF_INET(IPv4), TCP socket stream
            self.conn_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            print("[+]Client socket created")
        except OSError as e:
            print(f"[+]Unable to create socket\nError: {e}")
            self.conn_socket = None
    
    def connect_to_server(self):
        try:
            #connect to server
            self.conn_socket.connect(self.server_addr)
            print("[+]Connected to server")
        except OSError as e:
            print(f"[+]Unable to connect to server\nError: {e}")
            self.conn_socket.close()
            self.conn_socket = None
        if self.conn_socket is None:
            print("[+]Unable to establish connection with server - Exiting")
            sys.exit(1)

    def read_file(self, file_name):
        file_contents = None

        if os.path.exists(file_name): #checks if file exists
                try:
                    with open(file_name, 'r') as f: #opens file as read only
                        print(f"[+]{file_name} opened")
                        file_contents = f.read() #reads the file text
                        print(f"[+]{file_name} read")
                except OSError as e:
                    print(f"[+]Failed to open/read {file_name}\nError: {e}")
        
        return file_contents


    def valid_text(self, text):
        regex = re.compile("^[A-Z ]*$")
        return not regex.match(text) is None

    def send_data(self, text):
        with self.conn_socket:
            data = f'{len(text):<{HEADER_SIZE}}' + text
            print(f"[+]Data sent: {data}")
            self.conn_socket.send(bytes(data, "utf-8"))

#once connection is established, sends plaintext and key to otp_enc_d
    #needs to check plaintext and key for invalid characters
    #key should be the same size as the plaintext
#then receives ciphertext from otp_enc_d
    #outputs ciphertext

if __name__ == "__main__":
    HOST = socket.gethostname()
    PORT = 5757

    c = OTPEncodeClient(HOST, PORT) #instantiate OTPEncodeClient object
    c.start_client() #create client socket
    c.connect_to_server() #connect to server

    plaintext = c.read_file(sys.argv[1]) #plaintext file name passed to read_file()
    #key = c.read_file(sys.argv[2]) #key file name
    ciphertext = None

    #check text values are not none
    #check text values contain valid chars
    #check text values for same length
    
    c.send_data(plaintext)
    
    #receive ciphertext from server

