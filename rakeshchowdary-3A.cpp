//Server Code

#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<fstream>
using namespace std;

#define MAXLINE 4096
#define backlog 5
#define portcommand 20021
#define portfile 20020
#define portmsg 20023


int create_socket(int port)
{
int listenfd;
struct sockaddr_in dataservaddr;


//Create a socket for the soclet
//If sockfd<0 there was an error in the creation of the socket
if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
cerr<<"Problem in creating the data socket"<<endl;
exit(2);
}


//preparation of the socket address
dataservaddr.sin_family = AF_INET;
dataservaddr.sin_addr.s_addr = htonl(INADDR_ANY);
dataservaddr.sin_port = htons(port);

if ((bind (listenfd, (struct sockaddr *) &dataservaddr, sizeof(dataservaddr))) <0) {
cerr<<"Problem in binding the data socket"<<endl;
exit(2);
}

 //listen to the socket by creating a connection queue, then wait for clients
 listen (listenfd, 1);

return(listenfd);
}

int accept_conn(int sock)
{
int dataconnfd;
socklen_t dataclilen;
struct sockaddr_in datacliaddr;

dataclilen = sizeof(datacliaddr);
  //accept a connection
if ((dataconnfd = accept (sock, (struct sockaddr *) &datacliaddr, &dataclilen)) <0) {
cerr<<"Problem in accepting the data socket"<<endl;
exit(2);
}

return(dataconnfd);
}


int main(int argc,char **argv) 
{
 int socketfd,clientfd,n;
 pid_t childpid;
 socklen_t clilen;
 char buf[MAXLINE];
 struct sockaddr_in clientaddr,serveraddr;
  
if(argc!=2) {
  cerr<<"Give input as: ./server <portnumber>"<<endl;
  exit(1);
}

if((socketfd=socket(AF_INET,SOCK_STREAM,0))<0) {  //creation of socket
  cerr<<"Socket not created successfully."<<endl;
  exit(2);
}

//assign address to socket
serveraddr.sin_family=AF_INET;
serveraddr.sin_addr.s_addr=htonl(INADDR_ANY);
if(atoi(argv[1])!=portcommand) {
  cerr<<"Port number must be equal to "<<portcommand<<endl;
  exit(2);
}
serveraddr.sin_port=htons(atoi(argv[1]));

//bind the socket
bind(socketfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr));

//listen for connections
listen(socketfd, backlog);

cout<<"Server is running..waiting for connections."<<endl;

for( ; ; )  {
 
 clilen=sizeof(clientaddr);
 //accept connections
 clientfd =accept(socketfd,(struct sockaddr*)&clientaddr,&clilen);
 cout<<"Connection established..."<<endl;

if((childpid=fork())==0) { //child process
cout<<"Child process created"<<endl;
close(socketfd);

while((n=recv(clientfd,buf,MAXLINE,0)) > 0) {
cout<<"Input received from client: "<<buf;
char *token,*dummy;
dummy=buf;
token=strtok(dummy," ");

if(strcmp("RETR",token)==0) {
  char port[MAXLINE],buffer[MAXLINE],char_num_blks[MAXLINE],char_num_last_blk[MAXLINE];
  int datasock,lsize,num_blks,num_last_blk,i; 
  FILE *fp;
  token=strtok(NULL," \n");
  cout<<"Filename given is: "<<token<<endl;
  
  sprintf(port,"%d",portfile);
  datasock = create_socket(portfile);
  send(clientfd, port,MAXLINE,0
);
  datasock=accept_conn(datasock);
  if((fp=fopen(token,"r"))!=NULL)
 {
                //size of file
		send(clientfd,"1",MAXLINE,0);
		fseek(fp,0,SEEK_END);
		lsize=ftell(fp);
		rewind(fp);
		num_blks = lsize/MAXLINE;
		num_last_blk = lsize%MAXLINE; 
		sprintf(char_num_blks,"%d",num_blks);
		send(clientfd,char_num_blks,MAXLINE,0);
                sprintf(char_num_last_blk,"%d",num_last_blk);
                send(clientfd,char_num_last_blk,MAXLINE,0);
		
                for(i=0;i<num_blks;i++) {  
                     fread(buffer,sizeof(char),MAXLINE,fp);
                     send(datasock,buffer,MAXLINE,0);
                }
                if(num_last_blk > 0) {
                     fread(buffer,sizeof(char),num_last_blk,fp);
                     send(datasock,buffer,MAXLINE,0);
                 }
          fclose(fp);
          cout<<"File transfer done.\n";      

   }
  else
  { 
         send(clientfd,"0",MAXLINE,0);
         close(datasock);
         sprintf(port,"%d",portfile);
         datasock = create_socket(portmsg);
         send(clientfd,port,MAXLINE,0);
         datasock=accept_conn(datasock);
         strcpy(buffer,"<SERVER TIME>: FILE NOT FOUND AT CURRENT WORKING DIRECTORY");
         send(datasock,buffer,MAXLINE,0);
         close(datasock);
  
  }


}

if(strcmp("GOODBYE SERVER",buf)==0) {
char buffer[MAXLINE];
   strcmp(buffer,"THANK YOU,ALL CONNECTIONS SUCCESSFULLY TERMINATED");
   send(clientfd,buffer,MAXLINE,0);
   //close(datasock);
   close(clientfd);
}

exit(0);
}

close(clientfd);
}
}
}


--------------------------------------------------------------------------------------------------------------------------------------------

//Client Code

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>
#include <string.h>
#include <arpa/inet.h>
using namespace std;

int create_socket(int port,char *addr)
{
 int sockfd;
 struct sockaddr_in servaddr;


 //Create a socket for the client
 //If sockfd<0 there was an error in the creation of the socket
 if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
  cerr<<"Problem in creating the socket"<<endl;
  exit(2);
 }

 //Creation of the socket
 memset(&servaddr, 0, sizeof(servaddr));
 servaddr.sin_family = AF_INET;
 servaddr.sin_addr.s_addr= inet_addr(addr);
 servaddr.sin_port =  htons(port); //convert to big-endian order

 //Connection of the client to the socket
 if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
  cerr<<"Problem in creating data channel"<<endl;
  exit(3);
 }

return sockfd;
}

#define MAXLINE 4096
#define backlog 5
#define portcommand 20021
#define portfile 20020
#define portmsg 20023


int main(int argc, char **argv)
{
 int sockfd,i,j;
 struct sockaddr_in servaddr;
 char sendline[MAXLINE], recvline[MAXLINE];

 if (argc !=3) {
  cerr<<"Give input as : ./client <IP address of the server> <port number>"<<endl;
  exit(1);
 }

 //Create a socket for the client
 if ((sockfd=socket(AF_INET,SOCK_STREAM,0))<0) {
  cerr<<"Problem in creating the socket"<<endl;
  exit(2);
 }

 //Creation of the socket
 memset(&servaddr,0,sizeof(servaddr));
 servaddr.sin_family=AF_INET;
 servaddr.sin_addr.s_addr=inet_addr(argv[1]);
 servaddr.sin_port=htons(atoi(argv[2])); 

 //Connection of the client to the socket
 if (connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0) {
  cerr<<"Problem in connecting to the server"<<endl;
  exit(3);
 }

 //cout<<"ftp>";

 while(fgets(sendline,MAXLINE,stdin)!=NULL) {

  send(sockfd,sendline,MAXLINE,0);
  char *token,*dummy;
  dummy=sendline;
  token=strtok(dummy," ");

  if(strcmp("RETR",token)==0) {
   	char port[MAXLINE], buffer[MAXLINE],char_num_blks[MAXLINE],char_num_last_blk[MAXLINE],message[MAXLINE];
	int data_port,datasock,lSize,num_blks,num_last_blk,i;
	FILE *fp;
	recv(sockfd,port,MAXLINE,0);
        
	data_port=atoi(port);
        datasock=create_socket(data_port,argv[1]);
	token=strtok(NULL," \n");
	recv(sockfd,message,MAXLINE,0);
	if(strcmp("1",message)==0) {
            printf("HI client3\n");
		if((fp=fopen(token,"w"))==NULL)
			cout<<"Error in creating file\n";
		else
		{ 
                   printf("HI client4\n");
			recv(sockfd,char_num_blks,MAXLINE,0);
			num_blks=atoi(char_num_blks);
                        recv(sockfd,char_num_last_blk,MAXLINE,0);
			num_last_blk=atoi(char_num_last_blk);
                  printf("HI client5\n");
			for(i=0;i<num_blks;i++) { 
				recv(datasock,buffer,MAXLINE,0);
				fwrite(buffer,sizeof(char),MAXLINE,fp);
			}
			if(num_last_blk>0) { 
				recv(datasock, buffer, MAXLINE,0);
			   fwrite(buffer,sizeof(char),num_last_blk,fp);
			}
			fclose(fp);
			cout<<"File transfer done. client"<<endl;
		}
	}
	if(strcmp("0",message)==0) {
                recv(sockfd,port,MAXLINE,0);
	        //char m[5];
                //for(j=0;j<5;j++)
                //m[j]=port[5+j];
	        data_port=atoi(port);
	        datasock=create_socket(data_port,argv[1]);
                recv(sockfd,buffer,MAXLINE,0);
		cerr<<"Error in opening file. Check filename"<<endl;
	}
}

if(strcmp("GOODBYE SERVER",sendline)==0) {
     //strcpy(buffer,"GOODBYE SERVER"); 
     char buffer[MAXLINE];
     recv(sockfd,buffer,MAXLINE,0);
     exit(0);
}
}

}



