// Client side C/C++ program to demonstrate Socket programming 
#include <bits/stdc++.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include<stdlib.h>
// #include<conio.h>

#define PORT 9090 
#define BUFF_SIZE 1024

using namespace std;

char* conv_char(string s)
{
	
}

void send_request(struct sockaddr_in serv_addr,char *file_name,int sock)
{

}

// int main(int argc, char const *argv[]) 
// { 
// 	int sock = 0, valread; 
	
// 	struct sockaddr_in serv_addr; 
	
// 	char *file_name = "a.txt"; 
// 	char buffer[1024] = {0}; 
	
// 	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
// 	{ 
// 		printf("\n Socket creation error \n"); 
// 		return -1; 
// 	} 

// 	serv_addr.sin_family = AF_INET; 
// 	serv_addr.sin_port = htons(PORT); 
	
// 	// Convert IPv4 and IPv6 addresses from text to binary form 
// 	// if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
// 	// { 
// 	// 	printf("\nInvalid address/ Address not supported \n"); 
// 	// 	return -1; 
// 	// } 

// 	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
// 	{ 
// 		printf("\nConnection Failed \n"); 
// 		return -1; 
// 	} 
	
	
// 	send_request(serv_addr,file_name,sock);

// 	// send(sock , hello , strlen(hello) , 0 ); 
	
// 	// printf("Hello message sent\n"); 
	
// 	// recv( sock , buffer, 1024); 
// 	// printf("%s\n",buffer ); 
	
// 	return 0; 
// } 

// void send_file(int sockfd)
// {
// 	FILE *fp = fopen ( argv[1]  , "rb" );
// 	size_t n;

// 	fseek ( fp , 0 , SEEK_END);
//   	int size = ftell ( fp );
//   	rewind ( fp );
	

	
// 	send ( sockfd , &size, sizeof(size), 0);
	
	


// 	// string s(argv[1]);
// 	char file_name[strlen(argv[1])];

// 	int j=0;
// 	for(int i=0;i<strlen(argv[1]);i++)
// 	{
// 		if(argv[1][i]=='/')
// 		j=i+1;
// 		// file_name[i]=argv[1][i];
// 	}

// 	int file_name_size=strlen(argv[1])-j;

// 	send(sockfd,&file_name_size,sizeof(int),0);

// 	for(int i=0;j<strlen(argv[1]);j++)
// 	{
// 		file_name[i++]=argv[1][j];
// 	}

// 	send (sockfd,&file_name,strlen(file_name),0);




// 	char Buffer [BUFF_SIZE] ; 
	
// 	while ( ( n = fread( Buffer , sizeof(char) , BUFF_SIZE , fp ) ) > 0  && size > 0 ){
// 		send (sockfd , Buffer, n, 0 );
//    	 	memset ( Buffer , '\0', BUFF_SIZE);
// 		size = size - n ;
// 	}

// 	fclose ( fp );
// 	close(sockfd);
// }

void receive_file(int sockfd,char argv[])
{
	int n;
	
	int file_size_name=strlen(argv);
	send ( sockfd , &file_size_name, sizeof(file_size_name), 0);
	
	// send ( sockfd , &size, sizeof(size), 0);
	// int file_size_name;
	// recv(sockfd,&file_size_name,sizeof(file_size_name),0);
	
	// char file_name[file_size_name];
	send(sockfd,argv,file_size_name,0);

	char Buffer [BUFF_SIZE] ; 
	int file_size;
	recv( sockfd , &file_size , sizeof(file_size), 0);
	
	
	
	
	


	string file_des(argv);
	
	file_des="Test2/"+file_des;
	file_des.resize(file_size_name+6);

	


	FILE *fp = fopen ( file_des.c_str() , "wb" );

	while ( ( n = recv( sockfd , Buffer ,   BUFF_SIZE, 0) ) > 0  &&  file_size > 0){
	
	fwrite (Buffer , sizeof (char), n, fp);
	memset ( Buffer , '\0', BUFF_SIZE);
	file_size = file_size - n;
	} 
	
	fclose ( fp );

	close( sockfd);
}

int main(int argc,char* argv[])
{
	int sockfd = socket( AF_INET, SOCK_STREAM, 0 );

	struct sockaddr_in    serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons( PORT );
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");


	connect ( sockfd  , (struct sockaddr *)&serv_addr  , sizeof(serv_addr) );

	receive_file(sockfd,argv[1]);
	// FILE *fp = fopen ( argv[1]  , "rb" );
	// size_t n;

	// fseek ( fp , 0 , SEEK_END);
  	// int size = ftell ( fp );
  	// rewind ( fp );
	

	
	// send ( sockfd , &size, sizeof(size), 0);
	
	


	// // string s(argv[1]);
	// char file_name[strlen(argv[1])];

	// int j=0;
	// for(int i=0;i<strlen(argv[1]);i++)
	// {
	// 	if(argv[1][i]=='/')
	// 	j=i+1;
	// 	// file_name[i]=argv[1][i];
	// }

	// int file_name_size=strlen(argv[1])-j;

	// send(sockfd,&file_name_size,sizeof(int),0);

	// for(int i=0;j<strlen(argv[1]);j++)
	// {
	// 	file_name[i++]=argv[1][j];
	// }

	// send (sockfd,&file_name,strlen(file_name),0);




	// char Buffer [BUFF_SIZE] ; 
	
	// while ( ( n = fread( Buffer , sizeof(char) , BUFF_SIZE , fp ) ) > 0  && size > 0 ){
	// 	send (sockfd , Buffer, n, 0 );
   	//  	memset ( Buffer , '\0', BUFF_SIZE);
	// 	size = size - n ;
	// }

	// fclose ( fp );
	// close( sockfd);

}


