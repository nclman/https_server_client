This is meant as a simple project for anyone to run a HTTPS server and client

Requirements:
1. This project was developed on Ubuntu. It should work for other Linux-based machines.
2. Python3
3. OpenSSL - to generate SSL certificates. Libraries and headers needed for the client.
4. make and g++ - to build the client

Step 1: Generate keys and certificates

Use the "generate_keys.sh" shell script to create the keys and certs.

Step 2: Run the HTTPS server

The server is written in Python. You will need Python3 installed.
Simply run the Python script as follows:
>./https_server.py --port \<choose_a_port_number\> --ssl true --key \<path_to_key_file\> --cert \<path_to_certficate_file\>

Once the server is started, it will wait for a connection from the client.

Step 3: Build the client

The client is based on cpp-httplib project. Therefore, it is coded in C++.
To build the client, you can use the Makefile provided. In the client folder, simply run:
>make

Make sure that you created the CA certificate beforehand.
Make sure the port number in the client matches the server.

Step 4: Run the client

If everything went well, you should see a response in the terminal of the server running, and a message should appear on the client terminal (sent by the server)
>./client
