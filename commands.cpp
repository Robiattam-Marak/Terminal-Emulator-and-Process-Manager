#include<iostream>
#include<cstdlib>
#include<cstring>
#include<unistd.h>
#include<dirent.h>
#include<limits.h>
#include<commands.h>
using namespace std;

string cd(const char* newDir){
    if(chdir(newDir)==0){
        cout<<"Changed Successfully"<<endl;
        return pwd();
    }else{
        cerr<<"Error changing the directory"<<endl;
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