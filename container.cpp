#include "parser.h"
#include "header.h"


int isOK(int status,const char* msg){
    if(status==-1){
        perror(msg);
        exit(EXIT_FAILURE);
    }
    return status;
}


void WRITE(const char* path,const char* value){
    int fd = open(path,O_WRONLY | O_APPEND);
    if(fd == -1){
        std::cout<<"Error in opening\n";
        exit(1);
    }
    ssize_t bytes = write(fd,value,strlen(value));
    if(bytes==-1){
        std::cout<<"error in writing\n";
        exit(1);
    }
    close(fd);

}

//custom run function for the shell
int run(const char* name){
    char* _args[] = {(char*) name, (char*)0};
    return execvp(name, _args);
}


char* stack_mem(){
    const int stacksize = 65*1024;
    auto *stack = new (nothrow) char[stacksize];
    if(stack==nullptr){
        std::cout<<"Can't allocate memory";
        exit(EXIT_FAILURE);
    }
    return stack+stacksize;
}


void cloneProcess(int (*function)(void*),int flags){
    auto pid= clone(function,stack_mem(),flags,0);
    isOK(pid,"Clone Process Error");
    wait(nullptr);
}


void setupENV(){
    clearenv();
    setenv("TERM","xterm-256color",0);
    setenv("PATH","/bin/:/sbin/:/usr/sbin",0);
}


void setupRoot(const char* folder){
    isOK(chroot(folder),"cant't set root: ");
    isOK(chdir("/"),"chdir: ");
}

void setupJail(){
    map<string,string> c=parse("container_config.ini");
    const char* root = c["custom_root"].c_str();
    const char* cpuManage = c["cpu_manage"].c_str();
    const char* memory = c["memory"].c_str();
    const char* maxProcess = c["maxProcess"].c_str();
    const char* hostname = c["host_name"].c_str();
    WRITE(CGROUP_MEM,memory);
    WRITE(CGROUP_CPU,cpuManage);
    WRITE(concat(REQ_CGROUP,"/pid.max"),maxProcess);

    isOK(sethostname(hostname,strlen(hostname)),"hostname error");
    setupENV();
    setupRoot("./root");

}


int jail(void *args){
    setupJail();

    //attach the procfs to our main fail hierarchy
    mount("proc","/proc","proc",0,0);

    pid_t shellPid = fork();
    isOK(shellPid,"can't create fork: ");
    if(shellPid == 0)
    {
        run("/bin/bash");
        exit(0);
    }

    //wait till all child processes are finished
    while(wait(nullptr)>0);

    //unmount the procfs when all processes are finished
    umount("proc");

    return EXIT_SUCCESS;
}


void makeCgroup(){
    WRITE(CGROUP_SUBTREE,"+cpu +memory +pids");
    mkdir(REQ_CGROUP,S_IRUSR | S_IWUSR);
    const char* pid = to_string(getpid()).c_str();
    std::cout<<"Child Pid: "<<pid<<std::endl;
    WRITE(concat(REQ_CGROUP,"/cgroup.procs"),pid);
}


int main(){
    std::cout<<"Parent Pid :"<<getpid()<<endl;

    makeCgroup();
    
    cloneProcess(jail,CLONE_NEWPID | CLONE_NEWUTS| SIGCHLD);

    return EXIT_SUCCESS;

}