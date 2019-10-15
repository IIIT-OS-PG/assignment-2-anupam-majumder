#include <bits/stdc++.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include<stdlib.h>
#define THREADS 20
#define CHUNK_SIZE 512000
using namespace std;


struct args_of_fun
{
    int fd;
    struct sockaddr_in addr;
};
enum StringValue {login,create,download,share,list_file,logout};

map<string, StringValue> s_mapStringValues;

void initialize()
{
    s_mapStringValues["login"] =login;

    s_mapStringValues["create"] =create;

    s_mapStringValues["download"] =download;
    s_mapStringValues["share"] =share;
    
    s_mapStringValues["list_file"] =list_file;
    s_mapStringValues["logout"] =logout;
}

struct sockaddr_in   this_tracker;
struct sockaddr_in   other_tracker;
// bool master_status;
int thread_no;

unordered_map<string,string> login_det;
unordered_map<string,bool> active_usrs;
unordered_map<string,struct sockaddr_in> user_ip;

unordered_map<string,vector<pair<string,long long>>> file_user;

void init_inf()
{
    ifstream user_file("logs/login.txt");
    string line,word1,word2;
    while(getline(user_file,line))
    {
        stringstream ss(line);
        ss>>word1;
        ss>>word2;

        login_det[word1]=word2;

    }
    user_file.close();
}
void init_tracker(char path[],char tr_no[])
{
    ifstream tracker(path);
    string line, word;
    int pt;
    
    

    while(getline(tracker,line))
    {
        // cerr<<line<<endl;
        if(line[0]==tr_no[0])
        {
            // cout<<line<<endl;
            stringstream ss(line);
            ss>>word;
            ss>>word;
            ss>>pt;
            this_tracker.sin_family = AF_INET;
	        this_tracker.sin_port = htons( pt );
            this_tracker.sin_addr.s_addr=inet_addr(word.c_str());
            continue;
        }
        
        stringstream ss1(line);
        ss1>>word;
        ss1>>word;
        ss1>>pt;

        other_tracker.sin_family = AF_INET;
	    other_tracker.sin_port = htons( pt );
        other_tracker.sin_addr.s_addr=inet_addr(word.c_str());



    }
	// int sockfd = socket( AF_INET, SOCK_STREAM, 0 );

    // if(connect(sockfd, (struct sockaddr *)&other_tracker  , sizeof(other_tracker)))
    // {
    //     master_status=true;
    // }
    
    // close(sockfd);
    // cout<<master_status<<endl;
    
    tracker.close();
    

}
string create_user(int sockfd)
{
    int len;
    read(sockfd,&len,sizeof(int));
    
    char username[100];
    bzero(username,len+1);
    read(sockfd,username,len);
    
    string user(username);
    // cout<<user<<endl;
    int ret=0;
    while(login_det.count(user)>0)
    {
        // for(auto it=login_det.begin();it!=login_det.end();it++)
            // cout<<it->first<<" "<<it->second<<endl;
        
        ret=-1;
        write(sockfd,&ret,sizeof(int));
        read(sockfd,&len,sizeof(int));
        bzero(username,len+1);
        read(sockfd,username,len);
        
        string user_(username);
        user=user_;
        
        ret=0;
    }
    // cout<<ret<<endl;
    write(sockfd,&ret,sizeof(int));
    read(sockfd,&len,sizeof(int));
    
    char password[100];
    bzero(password,len+1);
    read(sockfd,password,len);
    
    string pass(password);
    login_det[user]=pass;
    // active_usrs[user]=true;
    // return user;
}
string login_user(int sockfd)
{
    int len;
    read(sockfd,&len,sizeof(int));
    
    char username[100];
    bzero(username,len+1);
    read(sockfd,username,len);
    
    string user(username);
    // cout<<user<<endl;
    int ret=0;
    // cout<<active_usrs[user]<<endl;
    // cout<<login_det.count(user)<<endl;

    while(login_det.count(user)==0 || active_usrs[user]==true)
    {
        // for(auto it=login_det.begin();it!=login_det.end();it++)
            // cout<<it->first<<" "<<it->second<<endl;
        
        ret=-1;
        write(sockfd,&ret,sizeof(int));
        read(sockfd,&len,sizeof(int));
        bzero(username,len+1);
        read(sockfd,username,len);
        
        string user_(username);
        user=user_;
        
        
    }
    ret=0;
    // cout<<ret<<endl;
    write(sockfd,&ret,sizeof(int));
    
    read(sockfd,&len,sizeof(int));
    
    char password[100];
    bzero(password,len+1);
    read(sockfd,password,len);
    
    
    string pass(password);
    // cout<<pass<<endl;
    // cout<<login_det[user]<<endl;
    

    while(login_det[user].compare(pass)!=0)
    {
        ret=-1;
        write(sockfd,&ret,sizeof(int));
        read(sockfd,&len,sizeof(int));
        bzero(password,len+1);
        read(sockfd,password,len);
        
        string pass_(password);
        pass=pass_;
        // cout<<pass<<endl;
        // cout<<login_det[user]<<endl;

        
    }
    ret=0;
    write(sockfd,&ret,sizeof(int));

    // login_det[user]=pass;
    active_usrs[user]=true;
    return user;
}
void share_file(int sockfd,string client)
{
    int len;
    char file[1024];
    long long filesize;

    read(sockfd,&len,sizeof(int));
    bzero(file,len+1);
    read(sockfd,file,len);
    read(sockfd,&filesize,sizeof(long long));

    string file_(file);
    cout<<"File:- "<<file_<<endl;
    cout<<"File Size:- "<<filesize<<endl;
    cout<<"Server Address:- "<<user_ip[client].sin_addr.s_addr<<endl;
    
    cout<<"Server Port:- "<<ntohs(user_ip[client].sin_port)<<endl;
    file_user[file_].push_back(make_pair(client,filesize));
    // file_user[file_].second=filesize;

}
void download_file(int sockfd,string client)
{
    char file_name[1024];
    int file_name_len;
    read(sockfd,&file_name_len,sizeof(int));

    bzero(file_name,file_name_len+1);

    read(sockfd,file_name,file_name_len);

    string file(file_name);
    
    cout<<file<<endl;

    vector<struct sockaddr_in> peer;
    long long f_size;
    int i1=0;
    for(auto it=file_user.begin();it!=file_user.end();it++)
    {
        string f=it->first;
        cout<<"File name:- "<<f<<endl;
        if(f.compare(file)==0)
        {
            for(auto it2=it->second.begin(); it2 != it->second.end(); ++it2)
            {
                cout<<it2->first<<" "<<it2->second<<endl;
                // if(it2->second.compare(client)==0)
                string client1=it2->first;
                if(client1.compare(client)==0)
                    continue;

                if(active_usrs[it2->first]==true)
                {
                    peer.push_back(user_ip[it2->first]);
                    f_size=it2->second;
                    // peer
                }

            }
        }
        
    }
    int no_peers;
    if(f_size % CHUNK_SIZE==0)
    {
        no_peers=f_size/CHUNK_SIZE;
    }
    else
    {
        no_peers=(f_size/CHUNK_SIZE) + 1;
    }

    write(sockfd,&no_peers,sizeof(int));
    cout<<"No of peers= "<<no_peers<<endl;

    int size_peers=peer.size();
    cout<<"How many actually sharing:- "<<size_peers<<endl;
    for(int i=0;i<no_peers;i++)
    {
        write(sockfd,&peer[i%size_peers],sizeof(struct sockaddr_in));
    }
    write(sockfd,&f_size,sizeof(long long));

}
void* functionalities(void* args)
{
    // struct args_of_fun client=*(struct args_of_fun *)args;
    int sockfd=*(int *)args;
    
    struct sockaddr_in client_serv;
    int si;
    // read(sockfd,&si,sizeof(int));
    // cout<<"Sizeof socket address:- "<<si<<endl;
    cout<<read(sockfd,&client_serv,16)<<endl;




    cout<<"Socket fd:- "<<sockfd<<endl;
    
    cout<<"Server Address:- "<<client_serv.sin_addr.s_addr<<endl;
    
    cout<<"Server Port:- "<<ntohs(client_serv.sin_port)<<endl;

    // int cmd_length;
    // recv(sockfd,&cmd_length,sizeof(int),0);
    int line_len;
    char line[1024];
    // recv(sockfd,&line_len,sizeof(int),0);
    string client_name="";
    
    while(true)
    {
        
        // cout<<"Here"<<endl;
        read(sockfd,&line_len,sizeof(int));
        bzero(line,line_len+1);
        
        // printf("Command length %d\n",line_len);
        // cout<<"Command length:- "<<cmd_len<<endl;

        recv(sockfd,line,line_len,0);
        string line_(line);
        
        cout<<"Command is "<<line_<<endl;
        // printf("Command is %s\n",line_);

        // stringstream ss(line_);

        // if(line_len==0)
        // continue;
        vector <string> tokens; 
      
        // stringstream class check1 
        stringstream check1(line_); 

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

        switch (s_mapStringValues[tokens[0]])
        {
        case login:
            cout<<"Login\n"<<endl;
            client_name=login_user(sockfd);
            user_ip[client_name]=client_serv;

        break;

        case create:
        
        if(tokens[1].compare("user")==0)
        {
            
            // client_name=
            create_user(sockfd);
            user_ip[client_name]=client_serv;
        }
        
        break;

        case logout:
        active_usrs[client_name]=false;
        break;

        case download:
        
        int ret1;
        if(client_name.compare("")==0)
        {
            ret1=-1;
            cout<<"No client name"<<endl;
            write(sockfd,&ret1,sizeof(int));
        }
        else
        {
            write(sockfd,&ret1,sizeof(int));
            download_file(sockfd,client_name);
        }
        
        break;
        
        case share:
        int ret;
        if(client_name.compare("")==0)
        {
            ret=-1;
            cout<<"No client name"<<endl;
            write(sockfd,&ret,sizeof(int));
        }
        else
        {
            ret=0;
            cout<<"Client name:- "<<client_name<<endl;
            write(sockfd,&ret,sizeof(int));
            share_file(sockfd,client_name);
        }
        
        break;
        
        case list_file:

        break;
        
        default:
            break;
        }
        // string cmd;
        // ss>>cmd;



        if(line_.compare("logout")==0)
            {
                cout<<"In logout"<<endl;
                break ;
            }
        // printf("Command %s\n",line_);
        // cout<<line_<<endl;
    }
    
    // cout<<"Out of loop1"<<endl;
    
   
    // cout<<"Out of loop1"<<endl;

    close(sockfd);
    pthread_exit(NULL);

}
void* server_tracker(void *args)
{
    // cerr<<"Hello"<<endl;

    int tracker_fd = socket (AF_INET, SOCK_STREAM, 0);
    int sockfd[102400];
    int idx=0;

    int addrlen = sizeof(sockaddr);
    struct args_of_fun client;

	while(bind (tracker_fd , (struct sockaddr *)&this_tracker , sizeof ( this_tracker ) ));
        // cout<<"Not Binded correctly"<<endl;
	
	while(listen (tracker_fd, 3));
        // cout<<"Not listening"<<endl;
	
    pthread_t thr1[THREADS];
    
    while(1)
	{
	    sockfd[idx] = accept ( tracker_fd , (struct sockaddr *)&client.addr , (socklen_t*)&addrlen);
        
        cout<<"Accepted "<<sockfd<<endl;
	    if( sockfd[idx] >= 0 )
	    {
            pthread_create(&thr1[thread_no],NULL,functionalities,&sockfd[idx]);
            idx=(idx+1)%102400;
            thread_no=(thread_no+1)%THREADS;
        }
        else
        {
            printf("Connection not Established with tracker\n");
        }
        // close(sockfd);
    }
    pthread_exit(NULL);
}


void sync_tracker()
{

}

void write_inf()
{
    ofstream user_file;
    
    user_file.open ("logs/login.txt", std::ofstream::out );

    string line,word1,word2;   
    for(auto it=login_det.begin();it!=login_det.end();it++)
    {
        user_file<<it->first<<" "<<it->second<<endl;

    }

    user_file.close();
}

int main(int argc,char* argv[])
{
    init_tracker(argv[1],argv[2]);
    initialize();
    init_inf();

    pthread_t serv;
    pthread_create(&serv,NULL,server_tracker,NULL);

    // for(auto it=login_det.begin();it!=login_det.end();it++)
    // {
    //     cout<<it->first<<" "<<it->second<<endl;
    // }
    // cout<<login_det["Anupam123"]<<endl;
    while(1)
    {
        string s;
        cin>>s;
        // cout<<s<<endl;
        if(s.compare("quit")==0)
        {
            // if(master_status)
            //     sync_tracker();
            // break;
            write_inf();
            exit(0);
            
        }
    }
    pthread_join(serv,NULL);
}