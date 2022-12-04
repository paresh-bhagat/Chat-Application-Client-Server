#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>
#define SERVER_ADDR "127.0.1.1"
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno,temp;
     socklen_t clilen;
     char buffer[256],server_name[20],client_name[20];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);

     //build server address structure

     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
     serv_addr.sin_port = htons(portno);

     //bind local port number

     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
              error("ERROR on binding");

     //specify number of concurrent
     //clients to listen for

     listen(sockfd,5);
     while(1)
     {
     	printf("\nWaiting for a new client connection on [TCP port %d]\n",portno);

     	//Wait for client

     	clilen = sizeof(cli_addr);
     	newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
     	if (newsockfd < 0)
        	error("ERROR on accept");
     	else
        	printf("Received connection from host [IP %s TCP port %d] \n",inet_ntoa(cli_addr.sin_addr),ntohs(cli_addr.sin_port));
        	
     	
     	n=read(newsockfd,client_name,20);
     	if (n < 0)
            	error("ERROR reading from socket");
            	
        printf("Enter your username : ");
    	fgets(server_name,20,stdin);
    	char* first_newline = strchr(server_name, '\n');
    	if (first_newline)
      		*first_newline = '\0';
      		

        n=write(newsockfd,server_name,strlen(server_name));
        if (n < 0)
            	error("ERROR writing to socket");
        //send and receive data from client
        
        
        printf("\nStart chat...\n");
        while(1)    
        {
        	bzero(buffer,256);
        	n=read(newsockfd,buffer,256);
        	if (n < 0)
            		error("ERROR reading from socket");
            	printf("%s : %s",client_name,buffer);
            	bzero(buffer,256);
            	printf("%s : ",server_name);
            	fgets(buffer,256,stdin);
        	n=write(newsockfd,buffer,strlen(buffer));
        	if (n < 0)
            		error("ERROR writing to socket");
            	int i = strncmp("Bye",buffer,3);
        	if(i==0)
        	{
        		close(newsockfd);
                       printf("Client disconnected\n");
            		break;
            	}
        }
     }
     
     return 0;
}

