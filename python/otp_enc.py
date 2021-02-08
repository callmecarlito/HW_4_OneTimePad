#!/usr/bin/env python3
import os
import random
import socket
import sys
import time
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

    def send_data(self, text):
        data = f'{len(text):<{HEADER_SIZE}}' + text
        print(f"[+]Data sent: {data}")
        self.conn_socket.send(bytes(data, "utf-8"))

    def recv_data(self):
        full_data = ''
        new_data = True

        while True:
            data = self.conn_socket.recv(BUFFER_SIZE)
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

    def valid_text(self, text):
        regex = re.compile("^[A-Z ]*$")
        return not regex.match(text) is None

    def make_key(self, text):
        char_set = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ ' #set of valid chars to create key
        key_file = "key.txt"
        key_len = len(text) #find len(text)
        key = ''.join(random.choice(char_set) for letter in text)
        print(f"[+]Key generated")

        #create key file and write the key to it
        try:
            with open(key_file, 'w') as f:
                print(f"[+]{key_file} opened")
                f.write(key)
                print(f"[+]{key_file} written")
                return key
        except OSError as e:
            print(f"[+]Failed to open/write {key_file}\nError: {e}")
            return None
        


        

#once connection is established, sends plaintext and key to otp_enc_d
    #needs to check plaintext and key for invalid characters
    #key should be the same size as the plaintext
#then receives ciphertext from otp_enc_d
    #outputs ciphertext

if __name__ == "__main__":
    HOST = socket.gethostname()
    PORT = 5757

    if len(sys.argv) != 2:
        print("[+]Invalid number of arguments. Exiting client")
        sys.exit(1)

    c = OTPEncodeClient(HOST, PORT) #instantiate OTPEncodeClient object
    c.start_client() #create client socket
    c.connect_to_server() #connect to server

    plaintext = c.read_file(sys.argv[1]) #plaintext file name passed to read_file()
    key = c.make_key(plaintext)

    #check text values are not none
    #check text values contain valid chars
    #check text values for same length
    
    c.send_data(plaintext)
    serv_recvd = int(c.recv_data())
    if serv_recvd == len(plaintext):
        print("[+]Server received plaintext. Sending key...")
        c.send_data(key)
    
    #receive ciphertext from server

