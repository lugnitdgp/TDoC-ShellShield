#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#define CGROUP_SUBTREE "/sys/fs/cgroup/cgroup.subtree_control"
#define REQ_CGROUP "/sys/fs/cgroup/cntr"
using namespace std;

void WRITE(const char* path,const char* value){
    int fd = open(path,O_WRONLY | O_APPEND);
    if(fd == -1){
        cout<<"Error in opening\n";
        exit(1);
    }
    ssize_t bytes = write(fd,value,strlen(value));
    if(bytes==-1){
        cout<<"error in writing\n";
        exit(1);
    }
    close(fd);

}

void makeCgroup(){
    WRITE(CGROUP_SUBTREE,"+cpu +memory +pids");
    mkdir(REQ_CGROUP,S_IRUSR | S_IWUSR);

}
int main(){
    cout<<"Parent Pid :"<<getpid();
    makeCgroup();
    
}