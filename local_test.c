#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc,char* argv[]){

  char *args[2];
  char *env[1];

  /**** Shamelessly borrowed from CMU's 15-213 course site ****/
  // Setup the socket
  int parentfd; /* parent socket */
  int childfd; /* child socket */
  int portno; /* port to listen on */
  int clientlen; /* byte size of client's address */
  struct sockaddr_in serveraddr; /* server's addr */
  struct sockaddr_in clientaddr; /* client addr */
  int optval; /* flag value for setsockopt */


  if(argc != 3){
    fprintf(stderr,"USAGE: %s PROGRAM PORT\n",argv[0]);
    return -1;
  }

  args[0] = argv[1]; args[1] = NULL;

  // If you NEED env stuff, fix this
  env[0] = NULL;

  // stdout buffering may need to be fixed in child as well...
  setbuf(stdout, NULL);

  portno = atoi(argv[2]);

  if(portno <= 0 || portno > 65535){
    fprintf(stderr,"ERROR: Invalid port number:%i\n",portno);
    return -1;
  }

  parentfd = socket(AF_INET, SOCK_STREAM, 0);
  if (parentfd < 0) 
    error("ERROR opening socket");

  /* setsockopt: Handy debugging trick that lets 
   * us rerun the server immediately after we kill it; 
   * otherwise we have to wait about 20 secs. 
   * Eliminates "ERROR on binding: Address already in use" error. 
   */
  optval = 1;
  setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR, 
	     (const void *)&optval , sizeof(int));

  /*
   * build the server's Internet address
   */
  bzero((char *) &serveraddr, sizeof(serveraddr));

  /* this is an Internet address */
  serveraddr.sin_family = AF_INET;

  /* let the system figure out our IP address */
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

  /* this is the port we will listen on */
  serveraddr.sin_port = htons((unsigned short)portno);

  if (bind(parentfd, (struct sockaddr *) &serveraddr, 
	   sizeof(serveraddr)) < 0) {
    fprintf(stderr,"Socket error in bind\n");
    return -1;
  }

  if (listen(parentfd, 1) < 0){ /* allow 1 request to queue up */ 
    fprintf(stderr,"Socket error in bind\n");
    return -1;
  }

  clientlen = sizeof(clientaddr);
  while (1) {

    /* 
     * accept: wait for a connection request 
     */
    childfd = accept(parentfd, (struct sockaddr *) &clientaddr, &clientlen);
    if (childfd < 0) {
      error("ERROR on accept");
      continue;
    }

    // dup some FDs
    dup2(childfd,STDIN_FILENO);
    dup2(childfd,STDOUT_FILENO);

    // Not duping stderr...

    // Exec some target
    if(0 > execve(argv[1],args,env)){
      fprintf(stderr,"execve \"%s\" failed.\n",argv[1]);
      return -1;
    }
  }
  return 0;
	    
}