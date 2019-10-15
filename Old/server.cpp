// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h> 
#include <stdlib.h>
#include<bits/stdc++.h>
#include<pthread.h>

#define PORT 9090 
#define BUFF_SIZE 1024

using namespace std;


void* send_file(void* args)
{
	int sockfd=*(int *)args;

	int file_size_name;
	recv(sockfd,&file_size_name,sizeof(file_size_name),0);
	
	char file_name[file_size_name];
	recv(sockfd,file_name,file_size_name,0);
	
	


	string file_des(file_name);
	
	file_des="Test1/"+file_des;
	file_des.resize(file_size_name+6);

	
	printf("Sending %s\n",file_name);


	FILE *fp = fopen ( file_des.c_str() , "rb" );
	size_t n;

	fseek ( fp , 0 , SEEK_END);
  	int size = ftell ( fp );
  	rewind ( fp );
	

	
	send ( sockfd , &size, sizeof(size), 0);
	
	char Buffer [BUFF_SIZE] ; 
	
	while ( ( n = fread( Buffer , sizeof(char) , BUFF_SIZE , fp ) ) > 0  && size > 0 ){
		send (sockfd , Buffer, n, 0 );
   	 	memset ( Buffer , '\0', BUFF_SIZE);
		size = size - n ;
	}

	fclose ( fp );
	close(sockfd);
	printf("Sent %s\n",file_name);

	pthread_exit(NULL);
}


void receive_file(int sockfd)
{
	int n;
	
	char Buffer [BUFF_SIZE] ; 
	int file_size;
	recv( sockfd , &file_size , sizeof(file_size), 0);
	
	
	
	int file_size_name;
	recv(sockfd,&file_size_name,sizeof(file_size_name),0);
	
	char file_name[file_size_name];
	recv(sockfd,file_name,file_size_name,0);
	
	


	string file_des(file_name);
	file_des="Test2/"+file_des;
	file_des.resize(file_size_name+6);

	


	FILE *fp = fopen ( file_des.c_str() , "wb" );

	while ( ( n = recv( sockfd , Buffer ,   BUFF_SIZE, 0) ) > 0  &&  file_size > 0){
	
	fwrite (Buffer , sizeof (char), n, fp);
	memset ( Buffer , '\0', BUFF_SIZE);
	file_size = file_size - n;
	} 
	cout<<"Sending of "<<file_name<<"Finished"<<endl;
	fclose ( fp );

	close( sockfd);
}

int main(int argc, char const *argv[]) 
{ 
	int server_fd = socket (AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in   addr;
	// struct sockaddr_in   cl_addr;
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons( PORT );
    addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	int addrlen = sizeof(sockaddr);

	bind (server_fd  , (struct sockaddr *)&addr , sizeof ( addr ) );
	// cerr<<"HEllo";
	listen (server_fd, 3);

	while(1)
	{
	int sockfd = accept ( server_fd , (struct sockaddr *)&addr , (socklen_t*)&addrlen);

	pthread_t thr;

	if( sockfd >= 0 )
	{
		pthread_create(&thr, NULL, send_file, &sockfd);
		// send_file(sockfd);
	}
	else
	{
		cout<<"Connection not established"<<endl;
	}
	}
	// int n;
	// // FILE *fp = fopen ( argv[1]  , "wb" );
	// char Buffer [BUFF_SIZE] ; 
	// int file_size;
	// recv( sockfd , &file_size , sizeof(file_size), 0);
	
	
	
	// int file_size_name;
	// recv(sockfd,&file_size_name,sizeof(file_size_name),0);
	
	// char file_name[file_size_name];
	// recv(sockfd,file_name,file_size_name,0);
	
	// // char file_name[BUFF_SIZE];
	// // n=recv(sockfd,&file_name,BUFF_SIZE,0);

	// // // cerr<<"Hello\n";
	// // for(int i=0;i<file_size_name;i++)
	// // printf("%c",file_name[i]);
	
	// // cout<<endl<<file_size_name<<endl;


	// string file_des(file_name);
	// // cout<<file_des.length()<<endl;
	// file_des="Test2/"+file_des;
	// file_des.resize(file_size_name+6);

	// // cout<<file_des<<endl;
	// // cout<<file_des.length()<<endl;


	// FILE *fp = fopen ( file_des.c_str() , "wb" );

	// while ( ( n = recv( sockfd , Buffer ,   BUFF_SIZE, 0) ) > 0  &&  file_size > 0){
	
	// // for(int i=0;i<n;i++)
	// // printf("%c",Buffer[i]);
	
	// fwrite (Buffer , sizeof (char), n, fp);
	// memset ( Buffer , '\0', BUFF_SIZE);
	// file_size = file_size - n;
	// } 
	
	// fclose ( fp );

	// close( sockfd);
	close( server_fd);
	
} 
