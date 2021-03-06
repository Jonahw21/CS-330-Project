/*****************************************************************
 * Client for downloads. It will receive a list of all files
 * available for download. The user can then specify which file to
 * download. If a file with the same name exists in the same directory
 * as the client then that file will be overwritten.
*****************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

/* Display error message on stderr and then exit. */
#define OOPS(msg) \
  {               \
    perror(msg);  \
    exit(1);      \
  }

#define MAXLINE 512

int main(int argc, char *argv[])
{
  const int MAX_FILENAME_LENGTH = 100;
  struct sockaddr_in bba; /* socket information */
  struct hostent *hp;     /* host information */
  int slen;               /* host computer */
  int s;                  /* length socket address */
  int rfd;
  char ch[MAXLINE];       /* character for i/o */
  int num_char = MAXLINE; /* number of characters */
  int port;               /* port to connect to */

  char portnum[20];
  char hostname[20];
  printf("\n hostname:");
  scanf("%s", hostname);

  printf("\n port number:");
  scanf("%s", portnum);

  /* Clear the data structure (saddr) to 0's. */
  memset(&bba, 0, sizeof(bba));

  /* Tell our socket to be of the internet family (AF_INET). */
  bba.sin_family = AF_INET;

  /* Acquire the ip address of the server */
  hp = gethostbyname(hostname);

  /* Acquire the port #. */
  port = atoi(portnum);

  /* Copy the server's address to the socket address data structure. */
  memcpy(&bba.sin_addr, hp->h_addr, hp->h_length);

  /* Convert the integer Port Number to the network-short standard
   * required for networking stuff. This accounts for byte order differences.*/
  bba.sin_port = htons(port);

  /* Now that we have our data structure full of useful information,
   * open up the socket the way we want to use it.
   */
  s = socket(AF_INET, SOCK_STREAM, 0);
  if (s == -1)
    OOPS("socket");
  if (connect(s, (struct sockaddr *)&bba, sizeof(bba)) != 0)
    OOPS("connect");

  int pid, status;
  int fd[2];
  char filename[MAX_FILENAME_LENGTH];

  pipe(fd);
  pid = fork();
  if (pid == -1)
    OOPS("problem in fork");

  //Child Process to retrieve file list from server
  if (pid == 0) //child
  {
    char *buffer[10];
    int n_char;
    while ((n_char = read(s, buffer, 10)) != 0)
    {
      n_char = write(1, buffer, n_char);
      if (n_char < 10)
      {
        exit(0);
      }
    }
    if (n_char == -1)
    {
      perror("Error reading from standard input.");
      exit(1);
    }
    exit(0);
  }
  else
  { //parent process to download a specified file
    wait(&status);  //need to wait to retrieve file list from server
    char *buffer[10];
    int n_char, file;

    //Child process to get and send name of file to be downloaded to server
    if (fork() == 0)
    {
      close(fd[0]);
      printf("\nType the name of the file you would like to download, then press CTRL-D\n");
      if ((n_char = read(fileno(stdin), buffer, MAX_FILENAME_LENGTH)) != 0)
      {
        n_char = write(s, buffer, n_char);
        write(fd[1], buffer, n_char * sizeof(char));
      }
      close(fd[1]);
      exit(0);
    }

    wait(&status);  //Waiting for name of file to be downloaded to be sent to the server
    close(fd[1]);
    read(fd[0], &filename, sizeof(filename));
    file = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (file == -1)
    {
      perror("Error opening file\n");
    }
    while ((n_char = read(s, buffer, 10)) != 0)
    {
      n_char = write(file, buffer, n_char);
    }
    if (n_char == -1)
    {
      perror("Error reading from standard input.");
      exit(1);
    }
    printf("\nFile has been downloaded\n");
    exit(0);
  }

  /* read from the socket, write to the screen */
  while ((num_char = read(s, ch, MAXLINE)) > 0)
    if (write(1, ch, num_char) < num_char)
      OOPS("writing");
  close(s);

  return 0;
}
