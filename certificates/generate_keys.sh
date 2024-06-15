#!/bin/bash

NAME=nopass

# Generate keys with no passcode
openssl genrsa -out ${NAME}.key 2048

# Generate keys with passcode
openssl genrsa -des3 -out ${NAME}.key 2048

# Generate a CA certificate
#openssl req -x509 -sha256 -days 1825 -newkey rsa:2048 -keyout rootCA.key -out rootCA.crt

openssl req -key ${NAME}.key -new -out ${NAME}.csr
openssl x509 -req -CA rootCA.crt -CAkey rootCA.key -in ${NAME}.csr -out ${NAME}.crt -days 3650 -CAcreateserial
