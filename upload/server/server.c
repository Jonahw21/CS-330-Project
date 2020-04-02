/*****************************************************************
 Sockets Daemon Program

  This code was modified from Nigel Horspools, "The Berkeley
  Unix Environment".

  A daemon process is started on some host.  A socket is acquired
  for use, and it's number is displayed on the screen.  For clients
  to connect to the server, they muse use this socket number.
*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>

/* Use port number 0 so that we can dynamically assign an unused
 * port number. */
#define PORTNUM         0

/* Set up the location for the file to display when the daemon (okay,
 * server for you religious types) is contacted by the client. */
#define BBSD_FILE       "./test.bbs.file"
/*"/nfs/net/share/ftp/pub/class/330/test.file" */

/* Display error message on stderr and then exit. */
#define OOPS(msg)       {perror(msg); exit(1);}

#define MAXLINE 512

int main()
{
  struct sockaddr_in saddr;       /* socket information */
  struct hostent *hp;     /* host information */
  char hostname[256];     /* host computer */
  socklen_t slen;         /* length socket address */
  int s;                  /* socket return value */
  int sfd;                /* socket descriptor returned from accept() */
  char ch[MAXLINE];       /* character for i/o */
  FILE *sf;               /* various file descriptors */
  int bbf;
  int num_char=MAXLINE;
  
  /*
   * Build up our network address. Notice how it is made of machine name + port.
   */

  /* Clear the data structure (saddr) to 0's. */
  memset(&saddr,0,sizeof(saddr));

  /* Tell our socket to be of the internet family (AF_INET). */
  saddr.sin_family = AF_INET;

  /* Aquire the name of the current host system (the local machine). */
  gethostname(hostname,sizeof(hostname));

  /* Return misc. host information.  Store the results in the
   * hp (hostent) data structure.  */
  hp = gethostbyname(hostname);

  /* Copy the host address to the socket data structure. */
  memcpy(&saddr.sin_addr, hp->h_addr, hp->h_length);

  /* Convert the integer Port Number to the network-short standard
   * required for networking stuff. This accounts for byte order differences.*/
  saddr.sin_port = htons(PORTNUM);
  
  /*
   * Now that we have our data structure full of useful information,
   * open up the socket the way we want to use it.
   */
  s = socket(AF_INET, SOCK_STREAM, 0);
  if(s == -1)
    OOPS("socket");

  /* Register our address with the system. */
  if(bind(s,(struct sockaddr *)&saddr,sizeof(saddr)) != 0)
    OOPS("bind");

  /* Display the port that has been assigned to us. */
  slen = sizeof(saddr);
  getsockname(s,(struct sockaddr *)&saddr,&slen);
  printf("Socket assigned: %d\n",ntohs(saddr.sin_port));

  /* Tell socket to wait for input.  Queue length is 1. */
  if(listen(s,1) != 0)
    OOPS("listen");

  /* Loop indefinately and wait for events. */
  for(;;)
  {
    /* Wait in the 'accept()' call for a client to make a connection. */
    sfd = accept(s,NULL,NULL);
    if(sfd == -1)
      OOPS("accept");


    int pid;
    pid = fork();
    if(pid == -1)
        OOPS("error in fork");
    if(pid > 0) //parent
    {
      int output;
      char filename[100];
      read(sfd, filename, 100);
      output = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
      char *buffer[10];
        int n_char, status;
        while ((n_char = read(sfd, buffer, 10))!=0)
        {
            n_char = write(output, buffer, n_char);
        }
        if (n_char==-1)
        {
            perror("Error reading from standard input.");
            exit (1);
        }
	close(output);
    } else { //child
        char *buffer[10];
        int n_char;
        while ((n_char = read(0, buffer, 10))!=0)
        {
            n_char = write(sfd, buffer, n_char);
        }
        if (n_char==-1)
        {
            perror("Error reading from standard input.");
            exit (1);
        }
    }
    close(sfd);
  }

  return 0;
} 
