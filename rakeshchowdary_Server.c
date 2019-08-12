#include<stdio.h>
#include<stdlib.h>

#include<string.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<netinet/in.h>
#define buffersize 1024
#define BUFFERSIZE1 1000

void deleteLine(FILE *srcFile, FILE *tempFile,int line)
{
    char buffer1[BUFFER_SIZE1];
    int count = 1;

    while ((fgets(buffer, BUFFER_SIZE1, srcFile)) != NULL)
    {
        /* If current line is not the line user wanted to remove */
        if (line != count)
            fputs(buffer, tempFile);

        count++;
    }
}


char buffer[buffersize];
int main()
{
      //char server_message[256]  = "You have reached the server!";

      //create server socket
      
      int pid=fork();
      int server_socket1;
      if(pid==0) {
      
      server_socket1 = socket(AF_INET,SOCK_STREAM,0);
      //define server address
      struct sockaddr_in server_address1,client_address1;
      server_address1.sin_family = AF_INET;
      server_address1.sin_port = htons(8001);
      server_address1.sin_addr.s_addr = INADDR_ANY;

      //bind the socket to our specified IP and port
      bind(server_socket1,(struct sockaddr*) &server_address1, sizeof(server_address1));

      listen(server_socket1,5);

      int client_socket1;
    //client_socket = accept(server_socket,NULL,NULL);
    //int client_length=sizeof(client_address);
    client_socket1 = accept(server_socket1,NULL,NULL);
    int terminate=1;
    while(terminate) {
    recv(client_socket1,buffer,sizeof(buffer),0);
    if(!strcpy(buffer,"ENTER"))
  {
    FILE *f,*fptr;
    f=fopen("RollS.txt","r");
    int ch=0,i;
    fptr=f;
    char b[9],cha,c,d;
  if((d=getc(f))!=EOF)
 {
    rewind(f);
    fscanf(f,"%s",b);
    send(client_socket1,b,sizeof(b),0);
 rewind(f);
    deleteline(f,fptr,1);
    remove(f);
    rename("temp.txt",f);
}

 else      //close the connection
    {
terminate=0;
     strcpy(buffer,"noinput");
     send(client_socket1,b,sizeof(b),0);
      //close(client_socket1);
    }
}
}

}

 else{
    int server_socket;
      server_socket = socket(AF_INET,SOCK_STREAM,0);
   struct sockaddr_in server_address,client_address;
      server_address.sin_family = AF_INET;
      server_address.sin_port = htons(8002);
      server_address.sin_addr.s_addr = INADDR_ANY;

      //bind the socket to our specified IP and port
      bind(server_socket,(struct sockaddr*) &server_address, sizeof(server_address));

      listen(server_socket,5);

      int client_socket;
      int client_length=sizeof(client_address);
    client_socket = accept(server_socket,(struct sockaddr*) &client_address,&client_length);

     int terminate=1;
     while(terminate) {
    recv(client_socket,buffer,sizeof(buffer),0);
    if(!strcpy(buffer,"ENTER"))
  {
    FILE *f,*fptr;
    fptr=fopen("temp.txt","w");
    f=fopen("RollS.txt","r");
    int ch=0,i;
    fptr=f;
    char b[9],cha,c,d;
  if((d=getc(f))!=EOF)
 {
    rewind(f);
    fscanf(f,"%s",b);
    send(client_socket,b,sizeof(b),0);
    rewind(f);
    deleteline(f,fptr,1);
    remove(f);
    rename("temp.txt",f);
 //   while((c=getc(f))!=EOF)
   // {
     //   putc(c,fptr);
    // }
 }

 else      //close the connection
    {
     terminate=0;
     strcpy(buffer,"noinput");
     send(client_socket,b,sizeof(b),0);
      //close(client_socket);
    }
}
}
}
 
       close(server_socket1);
        return 0;
      
}

