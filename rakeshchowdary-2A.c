//SOCKET Code

#include<stdio.h>
#include<stdlib.h>

#include<string.h>

#include<sys/types.h>
#include<sys/socket.h>

#include<netinet/in.h>
#define buffersize 1024

char buffer[buffersize];
int main()
{
      char server_message[256]  = "You have reached the server!";

      //create server socket
      int server_socket;
      server_socket = socket(AF_INET,SOCK_STREAM,0);
      
      //define server address
      struct sockaddr_in server_address;
      server_address.sin_family = AF_INET;
      server_address.sin_port = htons(9002);
      server_address.sin_addr.s_addr = INADDR_ANY;

      //bind the socket to our specified IP and port
      bind(server_socket,(struct sockaddr*) &server_address, sizeof(server_address)); 
      
      listen(server_socket,5);
      
      int client_socket;
   client_socket = accept(server_socket,NULL,NULL);

    int p=1;
    
    while(p==1) {
    recv(client_socket,buffer,sizeof(buffer),0);
   
      if(!(strcmp(buffer,"Hello:B160710CS")))
      {
	      strcpy(buffer,"This number belongs to S6/S8 batch.");
           send(client_socket,buffer,sizeof(buffer),0);
              p=0;
      }

      else
      {
	        strcpy(buffer,"Ok");
		send(client_socket,buffer,sizeof(buffer),0);
		
      }
}
      //close the connection
      close(server_socket);

	return 0;

}





//CLIENT Code

#include<stdlib.h>
#include<stdio.h>

#include<string.h>

#include<sys/types.h>
#include<sys/socket.h>

#include<netinet/in.h>
#define buffersize 1024

char buffer[buffersize];


int main()
{
	//create a socket
        int network_socket;
	network_socket = socket(AF_INET, SOCK_STREAM, 0);
        
        //specify an address for the socket
	struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(9002);
        server_address.sin_addr.s_addr = INADDR_ANY;

        int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
        // check for error with the connection
	if(connection_status == -1)
	{
		printf("There was an making a connection to the remote server \n\n");
	}	
           
        int t=1,i=1;
	scanf("%s",buffer);
        while(t==1){	

        
	//sending data	
	send(network_socket,buffer,sizeof(buffer),0);
        

        //receive data
	recv(network_socket,buffer, sizeof(buffer),0);

        if(!(strcasecmp(buffer,"Ok")))
	{
		printf("%s\n",buffer);
		printf("Enter Input again\n");
		scanf("%s",buffer);
	}

        else
	{
	  printf("This number belongs to S6/S8 batch.\n");
	  t=0;
	}	 
	//print the data received from server

	// and then close the socket
}
	close(network_socket);

	return 0;
}
