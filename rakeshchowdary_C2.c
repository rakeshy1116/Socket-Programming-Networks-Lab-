          
#include<stdio.h>
#include<stdlib.h>

#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<netinet/in.h>
#include<ctype.h>
#define buffersize 1024

char buffer[buffersize];


int main(){
	//create a socket
	int network_socket;
	network_socket=socket(AF_INET,SOCK_STREAM,0);

	//specify an address for the socket
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8002);
	server_address.sin_addr.s_addr = INADDR_ANY;

	int connection_status = connect(network_socket,(struct sockaddr *) &server_address, sizeof(server_address));
	// check for error in connection
	if(connection_status == -1)
	{
		printf("There was an error making connection.\n");	
	}

	//sending file
	char c;
        int terminate=1;
        while(terminate)
   {       
        scanf("%c",c);
        if(c=='\n')
        {
        strcpy(buffer,"ENTER");
        send(network_socket,buffer,sizeof(buffer),0);
        recv(network_socket,buffer,sizeof(buffer),0);
        if(strcpy(buffer,"noinput")) {
        FILE *fp;
        fp=fopen("RollNumberC2.txt","a");
        fprintf(fp,"%s",buffer);
        fclose(fp);  		
        }
        else
         { 
        terminate=0;
         }
   }
}
	

	//receive data

	close(network_socket);

	return 0;
}
