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
using namespace::std;

#define PORT 8080

bool checkifuserexist(string &filename,string &username){
    ifstream file(filename);
    string line;

    if(!file){
        cout<<"The file does not exist"<<endl;
        return false;
    }

    while(getline(file,line)){
        if(line==username) return true;
    }
    return false;
}

string pwd(){
    char cwd[PATH_MAX];
    if(getcwd(cwd,sizeof(cwd))!=NULL){
        return cwd;
    }
    else {
        cerr<<"Error getting the current directory"<<endl;
        return "####";
    }
}

string dir(){
    DIR *dir = opendir(pwd().c_str());
    if(dir==nullptr){
        cerr<<"Error : Could not open the directory"<<endl;
        return "####";
    }
    
    struct dirent *entry;
    string ans="";
    while((entry=readdir(dir))!=nullptr){
        ans+=entry->d_name;
        ans+="\n";
    }
    closedir(dir);
    return ans;
}   

string cd(const char* newDir){
    if(chdir(newDir)==0){
        cout<<"Changed Successfully"<<endl;
        return pwd();
    }else{
        cerr<<"Error changing the directory"<<endl;
        return "####";
    }
}

int main(){
    // string init = "/home/faiz";
    // cout<<cd(init.c_str())<<endl;;
    cout<<pwd()<<endl;
    string path = "/home/faiz/Desktop/Networking";
    cout<<cd(path.c_str())<<endl;
    // path = pwd()+"/..";
    path = "/Lab";
    string new_path = cd(path.c_str());
    if(new_path =="####") new_path = cd((pwd()+path).c_str());
    cout<<new_path<<endl;
    return 0;
}