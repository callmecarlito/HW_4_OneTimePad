#!/usr/bin/env python3
import signal
import socket
import sys
import tqdm

MAX_CONNECTION_REQ = 5
HEADER_SIZE = 10
BUFFER_SIZE = 8

class OTPEncodeServer():

    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.server_socket = None
        self.conn_socket = None
    
    def start_server(self):
        try: 
            #addres family: AF_INET(IPv4) 
            #socket type: SOCK_STREAM (TCP)
            self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #create new socket
            self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) #makes address reusable
            print("[+]Server socket created")
        except OSError as e:
            print(f"[+]Unable to create socket: {e}")
            self.server_socket = None
        else:        
            try:
                self.server_socket.bind((self.host, self.port)) #bind socket to address
                print("[+]Binding server socket")
                self.server_socket.listen(MAX_CONNECTION_REQ)  #establishes listening socket - limited to 5 connect requests
                print("[+]Listening for incoming connections")
            except OSError as e:
                print(f"[+]Unable to bind socket: {e}")
                self.server_socket = None
        
        if self.server_socket is None:
            print("[+]Unable to establish server socket")
            sys.exit(1)

    def recv_data(self):
        full_data = ''
        new_data = True

        while True:
            data = self.conn_socket.recv(BUFFER_SIZE)
            if new_data:
                #print(f"[+]New message length: {data[:HEADER_SIZE]}")
                data_len = int(data[:HEADER_SIZE])
                new_data = False

            full_data += data.decode("utf-8")
            if len(full_data)-HEADER_SIZE == data_len:
                #print(f"[+]Message received: {full_data[HEADER_SIZE:]}")
                full_data  = full_data[HEADER_SIZE:]
                break

        return full_data
    
    def send_data(self, text):
        data = f'{len(text):<{HEADER_SIZE}}' + text
        #print(f"[+]Data sent: {data}")
        self.conn_socket.send(bytes(data, "utf-8"))

    def encode_text(self, plaintext, key):
        enc_key = {
            'A': 0, 'B': 1,  'C': 2,  'D': 3,  'E': 4,  'F': 5,  'G': 6,  'H': 7,  'I': 8,
            'J': 9, 'K': 10, 'L': 11, 'M': 12, 'N': 13, 'O': 14, 'P': 15, 'Q': 16, 'R': 17,
            'S': 18,'T': 19, 'U': 20, 'V': 21, 'W': 22, 'X': 23, 'Y': 24, 'Z': 25, ' ': 26, 
        }

        #convert each char into their numerical value
        ptext = [enc_key[letter] for letter in plaintext]
        ktext = [enc_key[letter] for letter in key]
        #take the sum of the plaintext char and the key char, modula 27, to get the value of the ciphertext char
        ctext = [ (p+k)%len(enc_key) for p,k in zip(ptext, ktext) ]
        #print(f"[+]Cipher values: {ctext}")

        #convert numerical representation to string
        ciphertext = []
        for val in ctext:
            #appends the key that matches the numerical value (val)
            ciphertext.append(list(enc_key.keys())[list(enc_key.values()).index(val)])
        ciphertext = ''.join(ciphertext) #converts list of chars to string
        #print(f"[+]Ciphertext: {ciphertext}")

        return ciphertext

    def handle_connection(self):
        connected = True
        while connected:
            self.conn_socket, clientaddr = self.server_socket.accept() #accept incoming connection
            print(f"[+]Connected to {clientaddr[0]}: {clientaddr[1]}")

            plaintext = self.recv_data() #receives plaintext message from client
            #print(f"[+]Plaintext: {plaintext}")

            self.send_data(str(len(plaintext))) #sends size of plaintext to client as a confirmation of receipt

            key = self.recv_data() #receives key from client
            #print(f"[+]Key: {key}")

            if len(plaintext) == len(key): #verify size of plaintext and key are equal
                print("[+]Begin encoding...")
                ciphertext = self.encode_text(plaintext, key)
                self.send_data(ciphertext) #send ciphertext to client
                print("[+]Encrypted message sent")
            else:
                print("[+]Unable to encode. Closing connection")
            self.conn_socket.close()    
            connected = False
    
    def sigint_handler(self, sig, frame):
        print("\n[+]Shutting down server")
        #self.server_socket.shutdown(socket.SHUT_RDWR)
        self.server_socket.close()
        print("[+]Goodbye")
        sys.exit(0)

if __name__ == "__main__":
    HOST = socket.gethostname()
    PORT = 5757  
    
    s = OTPEncodeServer(HOST, PORT)
    s.start_server()
    
    signal.signal(signal.SIGINT, s.sigint_handler)
    print("[+]Press Ctrl+C to shutdown server")
    
    while True:
        s.handle_connection()