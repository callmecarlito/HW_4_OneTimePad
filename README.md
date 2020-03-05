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
### otp_enc_d
- This will run in the **background** as a daemon
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
  - All error text must be output to stderr
  - Should not crash or exit out, except when error occurs during start-up
  - If given bad input, it should recognize the error, report error to stderr, and continue running
    - Bad input should be handled in the client first though
- Use "localhost" as the target IP address/host
### otp_enc
- Connects to  otp_enc_d and and asks it to perform a one-time padstyle encryption
  - otp_enc doesn't do the encryption by itself - otp_enc_d does
- Upon receiving ciphertext from otp_enc_d, it should output it out to stdout
- Syntax: `otp_enc [PLAINTEXT] [KEY] [PORT]` where `[PLAINTEXT]` is the name of a file in the current directory that contains the plaintext to be encrypted. `[KEY]` contains the encryption key to use for encryption. `[PORT]` is the port that otp_enc should use to connect to otp_enc_d
- Can be excecuted in the following methods:
```
otp_enc myplaintext mykey 57171
otp_enc myplaintext mykey 57171 > ciphertext
otp_enc myplaintext mykey 57171 > ciphertext &
```
- Error Handling:
  - If key or plaintext files with any bad characters or does not match the size of the plaintext
    - It should terminate 
    - Send error text to stderr
    - Set exit value to 1
 - Should NOT be able to connecto to otp_dec_d
   - Must have programs reject each other
   - If this occurs, otp_enc should report rejection to stderr and terminate itself
     - otp_enc at this point should not be able to connect even to otp_enc_d
       - Report error to stderr with attempted port
       - Set exit value to 2
- Upon successful execution and termination, otp_enc should set exit value to 0
### otp_dec_d
- Function and usage similar to otp_enc_d
- Decrypts ciphertext it's given using a passed in key
- Returns plaintext to otp_dec
### otp_dec
- Function and usage similar to otp_enc
- Connects to otp_dec_d
- Asks to decrypt ciphertext using a passed-in ciphertext and key
- Can be excecuted in the following methods:
```
otp_dec ciphertext mykey 57171
otp_dec ciphertext mykey 57171 > plaintext
otp_dec ciphertext mykey 57171 > plaintext &
```
- Should not be able to connect to otp_enc_d
  - Should prevent/handle inadvertent connections similar to otp_enc
### keygen
- Creates key file of specified length
- Should only contain any of the 27 allowed characters
- Last character output by keygen should be `\n`
- All errortext must be output to stderr
- Syntax: `keygen [KEYLENGTH]` where `[KEYLENGTH]` is the lenght of the key file in characters
- Keygen should output to stdout
## Files
1.
2.
3.
4.
5.
6.
7.
## Compilation
For this project we are utilizing a bash test script to compile all five programs. File name: `compileall`

