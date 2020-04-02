The plan for this project is to create a file hosting database where files can be both uploaded and downloaded.

First a basic server will be needed that will receive data and write it to a file.
The server will also need to read from local files to send to the client.

A client program will also be needed to connect to the server. The client will send and recieves files from the server.

Future improvements include:
- being able to specify a user to store files under
- having password credentials for user
- server sending a list of available files
- support multiple connections at once.

The goal is to have a file hosting server written with what I have learned in class.


Using the program:

Uploads: 
1. Run the upload server so that a client can connect to it.
2. Run the upload client specifying the file you want to upload as a command line argument
3. Connect to the upload server given its host and port number.
4. The file specified will be opened by the client and sent to the server.

Downloads:
1. Run the server so that a client can connect to it
2. Connect to the server with a client. Doing so will automatically display all files available for download
3. In the client enter the name of the file you want to download, then press CTRL-D.
4. The server will then retrieve the file specified and send it to the client.
5. CTRL-D in the client will close the connection and the client application.