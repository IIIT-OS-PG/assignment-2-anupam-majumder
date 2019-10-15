// Client side C/C++ program to demonstrate Socket programming 
#include <bits/stdc++.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include<stdlib.h>
#include<termios.h>
#include<openssl/sha.h>
// #include<conio.h>

#define CHUNK_SIZE 512000 


#define BUFF_SIZE 1024
#define THREADS 20
using namespace std;

struct sockaddr_in  trackers[2];
int thread_no;
enum StringValue {login,create,download,share,list_file,logout};

unordered_map<string, StringValue> s_mapStringValues;


pthread_mutex_t lock1;
struct cluster
{
	int offset;
	char cluster_arr[512000];
};
struct address//For receiving
{
    char ip[15];
    int port;
};
struct chunk
{
	struct sockaddr_in fd;
	int offset;
	int size;
	char name[100];
};
string calc_sha()
{
	
}
void initialize()
{
    s_mapStringValues["login"]=login;

    s_mapStringValues["create"]=create;

    s_mapStringValues["download"]=download;
    s_mapStringValues["share"]=share;
    
    s_mapStringValues["list_file"]=list_file;
    s_mapStringValues["logout"]=logout;
}
// void receive_file(int sockfd,char argv[])
// {
// 	int n;
	
// 	int file_size_name=strlen(argv);
//     // cout<<"\nClient file name size \n"<<file_size_name<<endl;
//     // printf("%s\n",argv);
// 	send ( sockfd , &file_size_name, sizeof(file_size_name), 0);
	
// 	// send ( sockfd , &size, sizeof(size), 0);
// 	// int file_size_name;
// 	// recv(sockfd,&file_size_name,sizeof(file_size_name),0);
	
// 	// char file_name[file_size_name];
// 	send(sockfd,argv,file_size_name,0);

// 	char Buffer [BUFF_SIZE] ; 
// 	int file_size;
// 	recv( sockfd , &file_size , sizeof(file_size), 0);
	
	
	
	
	


// 	string file_des(argv);
	
// 	file_des="Test2/"+file_des;
// 	file_des.resize(file_size_name+6);

	

//     // cout<<"Client"<<endl;
//     // cout<<file_des<<endl;

// 	FILE *fp = fopen ( file_des.c_str() , "ab" );

// 	while ( ( n = recv( sockfd , Buffer ,   BUFF_SIZE, 0) ) > 0  &&  file_size > 0){
	
// 	fwrite (Buffer , sizeof (char), n, fp);
// 	memset ( Buffer , '\0', BUFF_SIZE);
// 	file_size = file_size - n;
// 	} 
	
// 	fclose ( fp );

// 	close( sockfd);
// }




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
		// cout<<word<<" "<<pt<<" "<<i<<endl;
		trackers[i].sin_family = AF_INET;
		trackers[i].sin_port = htons( pt );
    	trackers[i++].sin_addr.s_addr=inet_addr(word.c_str());

	}

}
void* send_file(void* args)
{
	// pthread_mutex_lock(&lock1);
	struct chunk c1;
	bzero(c1.name,100);
	// c1=*(struct chunk *)args;
	
	int sockfd=*(int *)args;
	read(sockfd,&c1,sizeof(struct chunk));
	int mutex_=0;
	while(write(sockfd,&mutex_,sizeof(int))==-1);
	// cout<<"Sockfd = "<<sockfd<<endl;
	// cout<<"Fd = "<<sockfd<<endl;
	// cout<<"File name = "<<c1.name<<endl;
	// cout<<"Offset = "<<c1.offset<<endl;
	// cout<<"Offset = "<<c1.offset<<" Chunk Size = "<<c1.size<<endl;
	// recv(sockfd,&file_size_name,sizeof(file_size_name),0);
	
	
	// recv(sockfd,file_name,file_size_name,0);
	// cout<<"Size of chunk = "<<sizeof(struct chunk);
	// cout<<endl;
	
	int file_size_name=strlen(c1.name);
	// char file_name[file_size_name+1];
	string file_des(c1.name);


	// cout<<"File name size = "<<file_size_name<<endl;
    // cout<<"File name:- "<<file_des<<endl;


	// string file_des(file_name);
	
	file_des="Test1/"+file_des;
	file_des.resize(file_size_name+6);

	// cout<<"\nServer"<<endl;
	// printf("Sending %s\n",file_name);
    // cout<<file_des<<endl;

	FILE *fp = fopen ( file_des.c_str() , "rb" );
	size_t n;

	fseek ( fp , (c1.offset)*CHUNK_SIZE , SEEK_SET);
  	int size = c1.size;
	

  	// rewind ( fp );
	

	
	// send ( sockfd , &size, sizeof(size), 0);
	
	char Buffer [BUFF_SIZE] ; 
	bzero ( Buffer , BUFF_SIZE);

	while (size > 0 )
	{
		if(size<BUFF_SIZE)
		{
			n = fread( Buffer , sizeof(char) , size , fp );
			write (sockfd , Buffer, n);
   	 			
		}
		else
		{
			n = fread( Buffer , sizeof(char) , BUFF_SIZE , fp );
			write (sockfd , Buffer, n);
   	 		bzero ( Buffer,BUFF_SIZE);
		}
		
		
		size = size - n ;
	}
	// pthread_mutex_unlock(&lock1);

	fclose ( fp );
	// close(sockfd);
	// printf("Sent %s\n",file_des);

	pthread_exit(NULL);
}
void* each_chunk(void* args)
{
	int sockfd;
	
	struct chunk c1=*(struct chunk *)args;
    int len_name,offset;
	// cout<<"Before lock\n";
	pthread_mutex_lock(&lock1);
	int mutex_=-1;
	while(mutex_==-1)
	{
		// cout<<"After lock\n";

		sockfd=socket(AF_INET,SOCK_STREAM,0);
		cout<<ntohs(c1.fd.sin_port)<<endl;
		cout<<c1.fd.sin_addr.s_addr<<endl;

		while(connect(sockfd,(struct sockaddr *)&c1.fd,sizeof(c1.fd))==-1);

		len_name=strlen(c1.name);

		write(sockfd,&c1,sizeof(struct chunk));

		read(sockfd,&mutex_,sizeof(int));
		offset=c1.offset;
		// cout<<"Fd = "<<sockfd<<endl;
		// // cout<<"File name = "<<c1.name<<endl;
		// cout<<"In thread Offset = "<<c1.offset<<endl;
		// cout<<"Chunk Size = "<<c1.size<<endl;
		// cout<<"In Thread Offset = "<<c1.offset<<" Chunk Size = "<<c1.size<<endl;
	}
	
	string name(c1.name);
	
	// cout<<"File name = "<<c1.name<<endl;
	pthread_mutex_unlock(&lock1);
	// cout<<"Unlock for Peer "<<c1.offset<<endl;
	

	// write(sockfd,&len_name,sizeof(int));

	// write(sockfd,c1.name,len_name);

	name="Test2/"+name;
	name.resize(len_name+6);
	// write(sockfd,&c1,sizeof(struct chunk));
	// cout<<"Here\n";
	FILE *fp=fopen(name.c_str(),"r+b");

	fseek(fp,offset*CHUNK_SIZE,SEEK_SET);

	// cout<<"Fseek Here\n";
	
	int size=c1.size;

	char Buffer[BUFF_SIZE];
	bzero(Buffer,BUFF_SIZE);
	ssize_t n;

	while(size > 0)
	{
		if(size<BUFF_SIZE)
		{
			n=read(sockfd,Buffer,size);
			// cout<<"Bytes= "<<n<<endl;

			fwrite(Buffer,sizeof(char),n,fp);
		}
		else
		{
			n=read(sockfd,Buffer,BUFF_SIZE);
			// cout<<"Bytes= "<<n<<endl;
			fwrite(Buffer,sizeof(char),n,fp);
			bzero(Buffer,BUFF_SIZE);		
		}
		fflush(fp);
		size=size-n;
	}
	fclose(fp);
	close(sockfd);
	// pthread_mutex_unlock(&lock1);

	pthread_exit(NULL);
}
void receiving_file(int sockfd_tracker)
{
    //Receive the no. of peers u have chosen cluster from tracker
    //Store in
    
	// int sockfd_tracker=*(int *)args;
    string file_name;
	cout<<"Enter the filename to be downloaded:- "<<endl;
	cin>>file_name;

	char file[1024];
	int filename_len=file_name.length();

	write(sockfd_tracker,&filename_len,sizeof(int));

	bzero(file,filename_len+1);
	strcpy(file,file_name.c_str());
	
	write(sockfd_tracker,file,filename_len);


	
    

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
	

	file_name="Test2/"+file_name;
	file_name.resize(filename_len+6);
    
    FILE *fp=fopen(file_name.c_str(),"wb");

	long long filesize;
	read(sockfd_tracker,&filesize,sizeof(long long));

	char Buffer[BUFF_SIZE];
	bzero(Buffer,BUFF_SIZE);
	long long filesize_=filesize;
	while(filesize_>0)
	{
		if(filesize_>=1024)
		{
			fwrite(Buffer,sizeof(char),1024,fp);
		}
		else
		{
			fwrite(Buffer,sizeof(char),filesize_,fp);
		}
		
		filesize_-=1024;
	}
	fclose(fp);

	//Comment
	
	// int sockfd[no_peers];
	pthread_t thr[no_peers];
	pthread_mutex_init(&lock1,NULL);
	// for(int i=0;i<no_peers;i++)
	// {
	// 	sockfd[i]=socket(AF_INET,SOCK_STREAM,0);
	// }
    struct chunk c1[no_peers];
	for(int i=0;i<no_peers;i++)
	{
		bzero(c1[i].name,100);
		strcpy(c1[i].name,file);
	}
		
	// cout<<"File name:- "<<c1[0].name<<endl;

    for(int i=0;i<no_peers;i++)
    {
        //int sockfd=socket(AF_INET,SOCK_STREAM,0);
		// cout<<"Peer:- "<<i<<endl;

        // bzero(cl.cluster_arr,CHUNK_SIZE);

        // strcpy(file_name,cl.cluster_arr);
        
        // while(connect(sockfd[i],(struct sockaddr *)&all_add[i],sizeof(all_add[i]))==-1);
        c1[i].fd=all_add[i];
        c1[i].offset=i;
		// cout<<"Offset in Peer "<<i<<" "<<c1[i].offset<<endl;
		// for(int i=0;i<strlen(file);i++)
		// {
		// 	c1.name[i]=file[i];
		// 	// cout<<c1.name[i];
		// }
        // write(sockfd,&cl,sizeof(cl));
        
        // bzero(cl.cluster_arr,512000);
		if(i < (no_peers-1))
		{
			c1[i].size=CHUNK_SIZE;
		}
		else
		{
			c1[i].size=filesize%CHUNK_SIZE;
		}
		// write(sockfd[i],&c1,sizeof(struct chunk));
		
		pthread_create(&thr[i],NULL,each_chunk,(void *)&c1[i]);
		// int mutex_=0;
		// write(sockfd[i],&mutex_,sizeof(int));
        // int cl_size;
        // read(sockfd[i],&cl_size,sizeof(int));

        // int n = read(sockfd[i],&cl.cluster_arr,cl_size);

        // fseek(fp,i*512000,SEEK_SET);

        // fwrite(cl.cluster_arr,sizeof(char),n,fp);
        
        // fflush(fp);
        // rewind(fp);
        
        // close(sockfd);
    }

	for(int i=0;i<no_peers;i++)
	{
		pthread_join(thr[i],NULL);
		// cout<<"Join "<<i<<endl;
	}
	// for(int i=0;i<no_peers;i++)
	// {
	// 	close(sockfd[i]);
	// }
	
	pthread_mutex_destroy(&lock1); 
}
void* server(void* args)
{
	// int pt=*(int*)args;
	int sockf[102400];
	int idx=0;
	int server_fd = socket (AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in   addr=*(struct sockaddr_in*)args;
	// struct sockaddr_in   cl_addr;
	
	// addr.sin_family = AF_INET;
	// addr.sin_port = htons( pt );
    // addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	int addrlen = sizeof(sockaddr);
	// pthread_mutex_init(&lock1, NULL);

	bind (server_fd  , (struct sockaddr *)&addr , sizeof ( addr ) );
	// cerr<<"HEllo";
	listen (server_fd, 5);
	
	pthread_t thr[THREADS];

	while(1)
	{
		sockf[idx] = accept ( server_fd , (struct sockaddr *)&addr , (socklen_t*)&addrlen);
	// pthread_mutex_lock(&lock1);

	// cout<<"Accepted fd= "<<sockf[idx]<<endl;

	if( sockf[idx] >= 0 )
	{
		// struct chunk c1;
		// bzero(c1.name,100);
		// read(sockfd,&c1,sizeof(struct chunk));
		// c1.fd=sockfd;
		pthread_create(&thr[thread_no], NULL, send_file, &sockf[idx]);
		idx=(idx+1)%102400;
		// int mutex_;
		// read(sockfd,&mutex_,sizeof(int));
		// cout<<"Mutex = "<<mutex_<<endl;
		thread_no=(thread_no+1)%THREADS;
		// send_file(sockfd);
	}
	else
	{
		cout<<"Connection not established"<<endl;
	}
	// pthread_mutex_unlock(&lock1);

	}
	// pthread_mutex_destroy(&lock1); 

    close(server_fd);
    pthread_exit(NULL);
}
void create_user(int sockfd)
{
	int len;
	string username;
	cout<<"Enter Username:- ";
	cin>>username;
	len=username.length();
	char user[1024];
	bzero(user,len+1);
	strcpy(user,username.c_str());
    write(sockfd,&len,sizeof(int));
    // char username[100];
    // bzero(username,len+1);
    write(sockfd,user,len);
    int ret;
	read(sockfd,&ret,sizeof(int));
    while(ret==-1)
    {
		cout<<"Username exists\n";
		cout<<"Enter Username:- ";
		cin>>username;
		len=username.length();
        bzero(user,len+1);
		strcpy(user,username.c_str());
    	write(sockfd,&len,sizeof(int));
    	// char username[100];
    	// bzero(username,len+1);
    	write(sockfd,user,len);
		read(sockfd,&ret,sizeof(int));
    }
    // write(sockfd,&ret,sizeof(int));
	string password;

	termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	
	cout<<"Enter password:- "; 
	cin>>password;
	len=password.length();
	char pass[1024];
    write(sockfd,&len,sizeof(int));
    // char password[100];
    bzero(pass,len+1);
	strcpy(pass,password.c_str());
    write(sockfd,pass,len);
	
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	cout<<endl;
    // string pass(password);
    // login_det[user]=pass;
    // active_usrs[user]=true;
    // client_name=user;
}
void login_user(int sockfd)
{
	int len;
	string username;
	cout<<"Enter Username:- ";
	cin>>username;
	len=username.length();
	char user[1024];
	bzero(user,len+1);
	strcpy(user,username.c_str());
    write(sockfd,&len,sizeof(int));
	write(sockfd,user,len);
    int ret=-1;
	while(ret==-1)
	{
		read(sockfd,&ret,sizeof(int));
		if(ret!=-1)
			continue;
		cout<<"Username Invalid"<<endl;
		cout<<"Enter Username:- ";
		cin>>username;
		len=username.length();
		
		bzero(user,len+1);
		strcpy(user,username.c_str());
    	write(sockfd,&len,sizeof(int));
		write(sockfd,user,len);
	}
	ret=-1;
	string password;

	termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	cout<<"Enter password:- "; 
	cin>>password;
	len=password.length();
	char pass[1024];
    write(sockfd,&len,sizeof(int));
    // char password[100];
    bzero(pass,len+1);
	strcpy(pass,password.c_str());
    write(sockfd,pass,len);
	
	while(ret==-1)
	{
		read(sockfd,&ret,sizeof(int));
		// cout<<ret<<endl;
		if(ret!=-1)
			continue;

		cout<<endl<<"Invalid Password"<<endl;	
		cout<<"Enter password:- "; 
		cin>>password;
		len=password.length();
		
    	write(sockfd,&len,sizeof(int));
    	// char password[100];
    	bzero(pass,len+1);
		strcpy(pass,password.c_str());
    	write(sockfd,pass,len);
	}
    
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	cout<<endl;
}

void share_file(int sockfd)
{
	cout<<"Enter the filename:- ";
	string filename;
	cin>>filename;

	int file_len=filename.length();

	write(sockfd,&file_len,sizeof(int));
	
	char file[1024];
	bzero(file,file_len+1);
	strcpy(file,filename.c_str());

	write(sockfd,&file,file_len);

	filename="Test1/"+filename;
	filename.resize(file_len+6);
	// cerr<<"Here"<<endl;
	FILE *fp=fopen(filename.c_str(),"rb");

	fseek(fp,0,SEEK_END);

	long long size=ftell(fp);
	write(sockfd,&size,sizeof(long long));

	fclose(fp);


}
void list_files(int sockfd)
{
	int no_files;
	read(sockfd,&no_files,sizeof(int));
	if(no_files>0)
	{
		cout<<"Files present are:- "<<endl;
		for(int i=0;i<no_files;i++)
		{
			int len;
			read(sockfd,&len,sizeof(int));
			char file[len+1];
			bzero(file,len+1);

			read(sockfd,file,len);
			string filename(file);

			cout<<filename<<endl;
		}
	}
	else
	{
		cout<<"No Files to display"<<endl;
	}
	
}
int main(int argc,char* argv[])
{
	initialize();

	int sockfd = socket( AF_INET, SOCK_STREAM, 0 );
	
	char port[10],ip[20];
    bzero(port,10);
    bzero(ip,20);
    int flag1=0;
    int j2=0,k2=0;
    for(int i=0;i<strlen(argv[1]);i++)
    {
        if(argv[1][i]==':')
        {
            flag1=1;
            continue;
        }
        if(flag1==0)
        {
            ip[j2++]=argv[1][i];
        }
        else if(flag1==1)
        {
            port[k2++]=argv[1][i];            
        }
    }
    int pt;
    
    string po(port);
    stringstream ss2(po);
    ss2>>pt;
    

    string i_p(ip);
    
    // struct sockaddr_in serv;

    // serv.sin_family=AF_INET;
    // serv.sin_addr.s_addr=inet_addr(i_p.c_str());
    // serv.sin_port=htons(pt);

	struct sockaddr_in    serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(pt);
    serv_addr.sin_addr.s_addr=inet_addr(i_p.c_str());;

	// int pt,serv;
    // cout<<"Enter server port"<<endl;
    // cin>>pt;
    
    // cout<<"Enter peer port"<<endl;

    // cin>>serv;
	pthread_t server_t;
	// pthread_mutex_init(&lock1, NULL);


	

	pthread_create(&server_t,NULL,server,&serv_addr);
	
	read_tracker(argv[2]);
    // struct sockaddr_in    serv_addr;
	// serv_addr.sin_family = AF_INET;
	// serv_addr.sin_port = htons( serv );
    // serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");


	/*Foe peer to peer
    string h;
    cin>>h;
	*/	
	int i=0;


	int si=sizeof(struct sockaddr_in);
	// write(sockfd,&si,sizeof(int));
	// cout<<si<<" "<<sizeof(serv_addr)<<endl;
	// exit(0);
	while(connect(sockfd,(struct sockaddr *)&trackers[i],sizeof(trackers[i]))==-1)
	{
		i=(i+1)%2;
	}
	cout<<"Commands are as follows:-"<<endl;
	cout<<"Create user:- create user"<<endl;
	cout<<"Login:- login"<<endl;
	cout<<"Share files:- share"<<endl;
	cout<<"Download files:- download"<<endl;
	cout<<"List files:- list_file"<<endl;
	cout<<"Logout:- logout"<<endl;

	
	
	
	
	// cout<<"Size = "<<
	write(sockfd,&serv_addr,16);//<<endl;
	
	while(true)
	{
		cout<<"> ";
		string cmd;
		getline(cin,cmd);
		if(cmd.length()==0)
		continue;
		// cout<<"Tracker:- "<<i+1<<endl;
		// cout<<cmd<<endl;
		char cmd_[cmd.length()+1];
		int cmd_len=cmd.length();
		for(int i=0;i<cmd_len;i++)
		{
			cmd_[i]=cmd[i];
		}
		// cout<<cmd<<endl;

		// cmd_[cmd.length()]='\0';
		// send(sockfd,&cmd_len,sizeof(int),0)
		ssize_t bytes=write(sockfd,&cmd_len,sizeof(int));
		
		// cout<<"Bytes = "<<bytes<<endl;
		
		while(bytes==-1)
		{
			i=(i+1)%2;

			connect(sockfd,(struct sockaddr *)&trackers[i],sizeof(trackers[i]));
			bytes=write(sockfd,&cmd_len,sizeof(int));
			
		}
		
		bytes=write(sockfd,cmd_,cmd_len);
		// cout<<"Bytes = "<<bytes<<endl;

		while(bytes==-1)
		{
			i=(i+1)%2;

			connect(sockfd,(struct sockaddr *)&trackers[i],sizeof(trackers[i]));
			bytes=write(sockfd,cmd_,cmd_len);
			
		}	
		vector <string> tokens; 
      
        // stringstream class check1 
        stringstream check1(cmd); 

        string intermediate; 

        // Tokenizing w.r.t. space ' ' 
        while(getline(check1, intermediate, ' ')) 
        { 
            tokens.push_back(intermediate); 
        } 

        // for(auto it=tokens.begin();it!=tokens.end();it++)
        // {
        //     cout<<*it<<endl;
        // }
		// cout<<tokens[0]<<endl;
        switch (s_mapStringValues[tokens[0]])
        {
			
        	case login:
			// cout<<"Login\n";
			login_user(sockfd);
			// cout<<"End\n";
        	break;

        	case create:
	
        	if(tokens[1].compare("user")==0)
        	{
				// cout<<"In create"<<endl;
        	    create_user(sockfd);

        	}
	
        	break;

        	case logout:
        	// active_usrs[client_name]=false;
        	break;

        	case download:
			
			int ret1;
			read(sockfd,&ret1,sizeof(int));
			// cout<<"Return status:- "<<ret1<<endl;
			if(ret1!=-1)
			receiving_file(sockfd);
			else
			cout<<"Login first\n";
        	
        	break;
	
        	case share:
			int ret;
			read(sockfd,&ret,sizeof(int));
			if(ret!=-1)
			share_file(sockfd);
			else
			cout<<"Login first\n";
        	break;

        	case list_file:
			list_files(sockfd);
        	break;
        
        	default:
			// cout<<"inside switch";
        	break;
        }

		if(cmd.compare("logout")==0)
		{
			// cout<<"In quit"<<endl;
			break;
		}	
		
		

	}
	
	
	/*For peer to peer
	// while(1)
    // {
    //     string s;
    //     cout<<"Enter a file name\n";
    //     cin>>s;
    //     if(s.compare("exit")==0)
    //     break;
    //     char file[s.length()+1];

    //     for(int j=0;j<s.length();j++)
    //         file[j]=s[j];
    //     file[s.length()]='\0';
        
    //     // strcpy(s.c_str(),file);
	    
    //     receive_file(sockfd,file);
    // }
	*/








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
	// pthread_mutex_destroy(&lock1); 

	close( sockfd);
	// pthread_mutex_destroy(&lock1); 


}


