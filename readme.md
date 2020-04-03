File Upload and Download
Jonah Wrubleski - 200361084

This project takes a look at using sockets to setup a file upload and download
server that clients can connect to. There is a separate server and client for
both uploading and downloading so that they can occupy different ports and be run
at the same time. In addition to sockets this project also uses file management to
open, read from, and write to the files being transferred. Forks and child processes
are used to manage the bi-directional communication between the client and the server.
Additionally pipes are used for communication within processes in the download client
to ensure that when new files are written the file name will match the downloaded file's
name.
The goal is to have a file hosting server written with what I have learned in class and the labs.

This project was written to run on the Linux machines in the CL115 lab.

While working on this project I had to overcome the issue of having a client and server communicate both
for uploading and for downloading. I set up two separate servers, one for uploads and
one for downloads, to avoid having an overly complex single server file trying to do everything.
Since there are two separate servers that means that there are two different ports that clients can
connect to. The upload client should connect to the upload server and the download client should
connect to the download server.

Another challenge was getting the filenames to match on both the client and server side. The
application relies on user input to specify the file to upload or download and when writing a
new file the name of the file to be created should match the file name on the opposite end.
For uploads this was easy as the filename could be sent to the server prior to the data from
the file. For downloads the client specifies the name of the file to download. This means
that the process that gets the name of the file to download from the user also needs to
communicate that file name to the process that will create the file. Using pipes I was able
to achieve this. Properly handling the file descriptors to ensure output was directed to the
correct place, such as returning the results of an 'ls' from and exec command over the wire
instead of to standard output was also important. Additionally do to the application's structure
some work had to be done to make sure that files were being uploaded to and downloaded from the
correct directories.

All .c files can be compiled with gcc and run.
Using the program:

Uploads: 
1. Compile and Run the upload server so that a client can connect to it.
2. Compile and Run the upload client, specifying the name of the file you want to upload as a command line argument
3. Connect to the upload server given its host and port number.
4. The file specified will be opened by the client and sent to the server.

Downloads:
1. Compile and Run the download server so that a client can connect to it
2. Compile and run the download client.
3. Connect to the server from the client via the server's hostname and port. Doing so will automatically display all files available for download
4. In the client enter the name of the file you want to download, then press CTRL-D.
5. The server will then retrieve the file specified and send it to the client. The file will be saved to the directory
 where the client was run. The client program will then close.