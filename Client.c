/*****************************************************************
  Sockets Client Program 

  This code is a modified version taken from Nigel Horspool's "The Berkeley
  Unix Environment".

  This client connects to a specified server (host) and receives
  information from it.
*****************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/* Display error message on stderr and then exit. */
#define OOPS(msg)       {perror(msg); exit(1);}

#define MAXLINE 512

int main(int argc, char *argv[])
{
  struct sockaddr_in bba; /* socket information */
  struct hostent *hp;     /* host information */
  int slen;               /* host computer */
  int s;                  /* length socket address */
  int rfd;
  char ch[MAXLINE];       /* character for i/o */
  int num_char=MAXLINE;   /* number of characters */
  int port;               /* port to connect to */

  char portnum[20];
  char hostname[20];
  printf("\n hostname:");
  scanf("%s", hostname);

  printf("\n port number:");
  scanf("%s", portnum);

  /* Clear the data structure (saddr) to 0's. */
  memset(&bba,0,sizeof(bba));

  /* Tell our socket to be of the internet family (AF_INET). */
  bba.sin_family = AF_INET;

  /* Acquire the ip address of the server */
  hp=gethostbyname(hostname);

  /* Acquire the port #. */
  port=atoi(portnum);

  /* Copy the server's address to the socket address data structure. */
  memcpy(&bba.sin_addr, hp->h_addr, hp->h_length);

  /* Convert the integer Port Number to the network-short standard
   * required for networking stuff. This accounts for byte order differences.*/
  bba.sin_port=htons(port);
  
  /* Now that we have our data structure full of useful information,
   * open up the socket the way we want to use it.
   */
  s = socket(AF_INET, SOCK_STREAM, 0);
  if(s == -1)
    OOPS("socket");
  if(connect(s,(struct sockaddr *)&bba,sizeof(bba)) != 0)
    OOPS("connect");

  int pid;
  pid = fork();
  if(pid == -1)
    OOPS("problem in fork");
  if(pid > 0) //parent
  {
    char *buffer[10];
    int n_char;
    while ((n_char = read(s, buffer, 10))!=0)
    {
        n_char = write(1, buffer, n_char);
    }
    if (n_char==-1)
    {
        perror("Error reading from standard input.");
        exit (1);
    }
} else { //child
    char *buffer[10];
    int n_char;
    while ((n_char = read(0, buffer, 10))!=0)
    {
        n_char = write(s, buffer, n_char);
    }
    if (n_char==-1)
    {
        perror("Error reading from standard input.");
        exit (1);
    }
}

  /* read from the socket, write to the screen */
  while( (num_char=read(s,ch,MAXLINE)) > 0 )
    if ( write(1,ch,num_char) < num_char)
      OOPS("writing");
    close(s);

  return 0;
}
