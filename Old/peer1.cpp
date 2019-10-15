// Client side C/C++ program to demonstrate Socket programming 
#include <bits/stdc++.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include<stdlib.h>
// #include<conio.h>

// #define PORT 9090 
#define BUFF_SIZE 1024
#define THREADS 20
using namespace std;

struct sockaddr_in  trackers[2];
int thread_no;

struct cluster//For sending
{
    // int sockfd;
	int offset;
	char cluster_arr[512000];
};
struct address//For receiving
{
    char ip[15];
    int port;
};

char* conv_char(string s)
{
	
}

void send_request(struct sockaddr_in serv_addr,char *file_name,int sock)
{

}
void* send_file(void* args)
{
	int sockfd=*(int *)args;

	int file_size_name;
	recv(sockfd,&file_size_name,sizeof(file_size_name),0);
	
	char file_name[file_size_name];
	recv(sockfd,file_name,file_size_name,0);
	
	cout<<"File name size = "<<file_size_name<<endl;
    printf("File name%s\n",file_name);


	string file_des(file_name);
	
	file_des="Test1/"+file_des;
	file_des.resize(file_size_name+6);

	// cout<<"\nServer"<<endl;
	// printf("Sending %s\n",file_name);
    // cout<<file_des<<endl;

	FILE *fp = fopen ( file_des.c_str() , "rb" );
	size_t n;

	fseek ( fp , 0 , SEEK_END);
  	int size = ftell ( fp );
  	rewind ( fp );
	

	
	send ( sockfd , &size, sizeof(size), 0);
	
	char Buffer [BUFF_SIZE] ; 
	memset ( Buffer , '\0', BUFF_SIZE);

	while ( ( n = fread( Buffer , sizeof(char) , BUFF_SIZE , fp ) ) > 0  && size > 0 ){
		send (sockfd , Buffer, n, 0 );
   	 	memset ( Buffer , '\0', BUFF_SIZE);
		size = size - n ;
	}

	fclose ( fp );
	close(sockfd);
	// printf("Sent %s\n",file_des);

	pthread_exit(NULL);
}

void* receiving_file(void *args)
{
    //Receive the no. of peers u have chosen cluster from tracker
    //Store in
    int sockfd_tracker=*(int *)args;
    
    int no_peers;
    read(sockfd_tracker,&no_peers,sizeof(int));
    
    //Create an array of that many addresses
    struct sockaddr_in  all_add[no_peers];
    
    
    for(int i=0;i<no_peers;i++)
    {
        // int size_ip;
        read(sockfd_tracker,&all_add[i],sizeof(struct sockaddr_in));
        
        // bzero(ip_ad,size_ip+1);
        // read(sockfd,ip_ad,size_ip);
        // int port_ip;
        // read(sockfd,&port_ip,sizeof(int));
        // all_add[i].port=port_ip;
        // for(int j=0;j<sizeip;i++)
        // {
        //     all_add[i].ip[j]=ip_ad[j];
        // }
    }
    char file_name[]="Test2/Ep.mkv";
    
    FILE *fp=fopen("Test2/Ep.mkv","ab");
    for(int i=0;i<no_peers;i++)
    {
        int sockfd=socket(AF_INET,SOCK_STREAM,0);
        struct cluster cl;
        cl.offset=i;
        bzero(cl.cluster_arr,512000);

        strcpy(file_name,cl.cluster_arr);
        
        connect(sockfd,(struct sockaddr *)&all_add[i],sizeof(all_add[i]));
        
        // write(sockfd,&cl,sizeof(cl));
        
        bzero(cl.cluster_arr,512000);

        int cl_size;
        read(sockfd,&cl_size,sizeof(int));

        int n = read(sockfd,&cl.cluster_arr,cl_size);

        fseek(fp,i*512000,SEEK_SET);

        fwrite(cl.cluster_arr,sizeof(char),n,fp);
        
        fflush(fp);
        rewind(fp);
        
        close(sockfd);
    }

        
}
void receive_file(int sockfd,char argv[])
{
	int n;
	
	int file_size_name=strlen(argv);
    // cout<<"\nClient file name size \n"<<file_size_name<<endl;
    // printf("%s\n",argv);
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

	

    // cout<<"Client"<<endl;
    // cout<<file_des<<endl;

	FILE *fp = fopen ( file_des.c_str() , "ab" );

	while ( ( n = recv( sockfd , Buffer ,   BUFF_SIZE, 0) ) > 0  &&  file_size > 0){
	
	fwrite (Buffer , sizeof (char), n, fp);
	memset ( Buffer , '\0', BUFF_SIZE);
	file_size = file_size - n;
	} 
	
	fclose ( fp );

	close( sockfd);
}
void* server(void* args)
{
	int pt=*(int*)args;

	int server_fd = socket (AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in   addr;
	// struct sockaddr_in   cl_addr;
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons( pt );
    addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	int addrlen = sizeof(sockaddr);

	bind (server_fd  , (struct sockaddr *)&addr , sizeof ( addr ) );
	// cerr<<"HEllo";
	listen (server_fd, 3);
	
	pthread_t thr[THREADS];
	while(1)
	{
	int sockfd = accept ( server_fd , (struct sockaddr *)&addr , (socklen_t*)&addrlen);

	

	if( sockfd >= 0 )
	{
		pthread_create(&thr[thread_no], NULL, send_file, &sockfd);
		
		thread_no=(thread_no+1)%THREADS;
		// send_file(sockfd);
	}
	else
	{
		cout<<"Connection not established"<<endl;
	}
	}
    close(server_fd);
    pthread_exit(NULL);
}



void read_tracker(char tracker_file[])
{
	ifstream track(tracker_file);
	string line;
	int i=0;
	while(getline(track,line))
	{
		string word;int pt;
		stringstream ss(line);
		ss>>word;
		ss>>word;
		ss>>pt;
		cout<<word<<" "<<pt<<" "<<i<<endl;
		trackers[i].sin_family = AF_INET;
		trackers[i].sin_port = htons( pt );
    	trackers[i++].sin_addr.s_addr=inet_addr(word.c_str());

	}

}


int main(int argc,char* argv[])
{
	int sockfd = socket( AF_INET, SOCK_STREAM, 0 );

	int pt,serv;
    cout<<"Enter server port"<<endl;
    cin>>pt;
    
    cout<<"Enter peer port"<<endl;

    cin>>serv;
	pthread_t server_t;

	pthread_create(&server_t,NULL,server,&pt);
	
	// read_tracker(argv[1]);
    struct sockaddr_in    serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons( serv );
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");


	// Foe peer to peer
    string h;
    cin>>h;
	
	// int i=0;

	// while(connect(sockfd,(struct sockaddr *)&trackers[i],sizeof(trackers[i]))==-1)
	// {
	// 	i=(i+1)%2;
	// }

	// while(1)
	// {
	// 	string cmd;
	// 	cin>>cmd;
	// 	if(cmd.compare("quit")==0)
	// 		exit(0);
		
	// 	char cmd_[cmd.length()+1];
	// 	int cmd_len=cmd.length();
	// 	for(int i=0;i<cmd_len;i++)
	// 	{
	// 		cmd_[i]=cmd[i];
	// 	}
	// 	// cout<<cmd<<endl;

	// 	// cmd_[cmd.length()]='\0';
	// 	// send(sockfd,&cmd_len,sizeof(int),0)
	// 	ssize_t bytes=write(sockfd,&cmd_len,sizeof(int));
		
	// 	while(bytes==-1)
	// 	{
	// 		i=(i+1)%2;
	// 		connect(sockfd,(struct sockaddr *)&trackers[i],sizeof(trackers[i]));
	// 		bytes=write(sockfd,&cmd_len,sizeof(int));
			
	// 	}
		
	// 	bytes=write(sockfd,cmd_,cmd_len);

	// 	while(bytes==-1)
	// 	{
	// 		i=(i+1)%2;
	// 		connect(sockfd,(struct sockaddr *)&trackers[i],sizeof(trackers[i]));
	// 		bytes=write(sockfd,cmd_,cmd_len);
			
	// 	}	
	
		
		
		

	// }
	
	
	/*For peer to peer*/
	
	while(1)
    {
        string s;
        cout<<"Enter a file name\n";
        cin>>s;
        if(s.compare("exit")==0)
        break;
        char file[s.length()+1];

        for(int j=0;j<s.length();j++)
            file[j]=s[j];
        file[s.length()]='\0';
        
        // strcpy(s.c_str(),file);
	    
        receive_file(sockfd,file);
    }
	










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
	close( sockfd);

}


