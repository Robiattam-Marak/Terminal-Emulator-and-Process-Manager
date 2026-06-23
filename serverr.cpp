#include<iostream>
#include<cstdlib>
#include<cstring>
#include<unistd.h>
#include<limits.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fstream>
#include<dirent.h>
#include "commands.h"
using namespace::std;

#define PORT 8080
#define MAXLEN 50

int keep_recv(int sockfd,string &recv_str){
    char buf[MAXLEN];
    int byt_recv=recv(sockfd,&buf[0],MAXLEN-1,0);
    buf[byt_recv] = '\0';
    if(byt_recv<0) return 0;
    recv_str+=buf;
    if(buf[byt_recv-1]=='\0'){
        return 0;
    }
    return 1;
}

string recv_msg(int sockfd){
    int status = 1;
    string recv_str="";
    while(status!=0){
        status = keep_recv(sockfd,recv_str);
    }
    return recv_str;
}

bool checkifuserexist(string filename,string &username){
    ifstream file(filename);
    string line;

    if(!file){
        cout<<"The file does not exist"<<endl;
        return false;
    }

    while(getline(file,line)){
        // cout<<"serching line:"<<line<<endl;
        if(line==username) return true;
    }
    return false;
} 


int main(){
    int sockfd,newsockfd;
    socklen_t clilen;
    struct sockaddr_in cli_addr, serv_addr;
    time_t now;

    sockfd = socket(AF_INET, SOCK_STREAM,0);
    if(sockfd<0){
        perror("Cannot create a socket at teh server");
        exit(EXIT_FAILURE);
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if(bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0){
        perror("Unable to bind");
        exit(EXIT_FAILURE);
    }

    listen(sockfd,5);

    while(true){
        clilen  = sizeof(cli_addr);
        newsockfd = accept(sockfd,(struct sockaddr*)&cli_addr,&clilen);

        if(newsockfd<0){
            perror("Accept error");
            exit(EXIT_FAILURE);
        }

        cout<<"Connection Established with the Client"<<endl;

        string input(MAXLEN,'\0');
        string output;
        output = "LOGIN";
        send(newsockfd,output.c_str(),output.size()+1,0);
        recv(newsockfd,&input[0],MAXLEN,0);
        input.erase(input.find('\0'));
        if(checkifuserexist("user.txt",input)) output = "FOUND";
        else output = "NOT-FOUND";
        send(newsockfd,output.c_str(),output.size()+1,0);
        if(output=="NOT-FOUND") continue;

        // string inital_root = ".";
        // cd(inital_root.c_str());
        while(true){
            string cmd = recv_msg(newsockfd);
            cout<<"Command at server:"<<cmd<<endl;
            string result;
            if(cmd=="pwd") result = pwd();
            else if(cmd=="dir") result = dir();
            else if(cmd.substr(0,2)=="cd"){
                string path;
                int j = 3;
                while(j<cmd.size() && cmd[j]==' ') j++; 
                if(j>cmd.size()) result = "####";
                else{
                    string path = cmd.substr(j,cmd.length()-j);
                    result = cd(path.c_str());
                    if(path[0]!='/') path="/"+path;
                    if(result=="####") result = cd((pwd()+path).c_str());
                }
            }
            else if(cmd=="exit") break;
            else result = "####";
            cout<<"Returning Result "<<result<<endl;
            send(newsockfd,result.c_str(),result.length()+1,0);
        }

        cout<<"Closing the connection on the server size"<<endl;
        close(newsockfd);

    }

    return 0;
}