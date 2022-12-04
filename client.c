#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>
#include <string.h>
void error(const char *msg)
{
    perror(msg);
    printf("closing connection with the server\n");
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256],client_name[20],server_name[20];
    char end[]="end connection";
    if (argc < 3) 
    {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);

    // create stream socket

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) 
    {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    //build server address structure

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    //connect to server

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    else{printf("\nConnected to server successfully.Say hi to server to start\n");}

    //messages
    printf("Enter your username : ");
    fgets(client_name,20,stdin);
    char* first_newline = strchr(client_name, '\n');
    if (first_newline)
      *first_newline = '\0';
      
    n = write(sockfd,client_name,strlen(client_name));
    if (n < 0)
    	error("ERROR writing to socket");
    
    n = read(sockfd, server_name, 20); 
    if (n < 0)
        error("ERROR reading from socket"); 
    printf("\nStart chat...\n");
 
    while(1)
    {
    	bzero(buffer,256);
    	printf("%s : ",client_name);
    	fgets(buffer,256,stdin);
    	n=write(sockfd,buffer,strlen(buffer));
    	if (n < 0)
    		error("ERROR writing to socket");
    	int i = strncmp("Bye",buffer,3);
        if(i==0)
            break;
    	bzero(buffer,256);
     	n=read(sockfd,buffer,256);

     	if (n < 0)
        	error("ERROR reading from socket"); 
        printf("%s : %s",server_name, buffer);
        
    }  
    printf("\nClosing connection from Server\n");
    close(sockfd);
    return 0;
}
