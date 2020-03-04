# HW_4_OneTimePad
This was our capstone project in our Operating Systems (CS344) course which combined the topics of multiprocessing and socket-based inter-process communication. In addition, the program will also handle encryption and decryption of information using a one-time pad-like system.

reference: [One-Time Pad](http://en.wikipedia.org/wiki/One-time_pad)
##### Definitions
- **Plaintext** - human readable information that we wish to have encrypted for protection
- **Ciphertext** - the encrypted version of **Plaintext** that is not human readable or cracked if OTP is used correctly
- **Key** - random sequence of characters that will be used to convert **Plaintext** to **Ciphertext** and back. The key must also not be reused or else the encryption could be compromised.  

### Encryption Method
The program will encrpyt and decrypt plaintext in ciphertext using a key based on modulo 27 operations. The 27 characters are the 26 capital letters and the space character. 
```
A(0)   B(1)   C(2)   D(3)   E(4)   F(5)   G(6)   H(7)   I(8)  
J(9)   K(10)  L(11)  M(12)  N(13)  O(14)  P(15)  Q(16)  R(17)
S(18)  T(19)  U(20)  V(21)  W(22)  X(23)  Y(24)  Z(25)  " "(26)

encoding/encryption: message + key = ciphertext
decoding/decryption: ciphertext - key = message
```
### Structure
5 programs:
  1. otp_enc_d
  2. otp_enc
  3. otp_dec_d
  4. otp_dec
  5. keygen
#### otp_enc_d
- This will run in the **background** as a daemon.
- Must output error if it cannot run due to a network error
- Performs the actual encoding
- Listens on a particular port/socket assigned on execution
- Upon connection, calls `accept()` to create the socket used for communication
- Then executes a separate process to handle the rest of the transaction
  - Child Process:
    - Must first check to make sure it's communicating with otp_enc
    - After verifying connection, child process receives plaintext from and key from otp_enc via communication socket(not the listening socket)
    - Child process will then encrypt and write back the ciphertext to otp_enc
    - Make sure key passed is equal in size to the the plaintext
- Must support up to 5 concurrent socket connections running simultaneously
- Continuously listens for new connections
- Syntax for executing otp_enc_d: `otp_enc_d [LISTENING_PORT_NUM] &`
- Error Handling:
  - 
#### otp_enc
- 
#### otp_dec_d
#### otp_dec
#### keygen
