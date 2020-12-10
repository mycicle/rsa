# rsa
RSA Encryption algorithm implemented in C++ using gmp to manage large numbers

### init.cc:
- Create a user's public and private keys. 
- Save public and private keys to text files

### encrypt.cc
- Load user's public key from disk
- Encrypt argv[1] and save the result to message.txt 

### decrypt.cc
- Load the encrypted message from message.txt
- Decrypt the message
- Print the decrypted message to stdout

### utils.cc
- Contains the definitions of constants
- Contains definitions of the structs used to store public and private keys

Michael DiGregorio

Collaboration with Firas Asfar and Alexander Zaino


