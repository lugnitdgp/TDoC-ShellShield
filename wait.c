#include<stdio.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
    //create a child
    pid_t child = fork();
    int exitStatus,childPid;

    if(child == 0)
    {
        //display child running and put the child to sleep
        printf("Child: running and PID: %d\n",getpid());

        sleep(5);

        //exit the child
        exit(40);
    }else if(child > 0)
    {
        //parent
        printf("Parent: running\n");

        childPid = wait(&exitStatus);

        printf("Parent: Child finished! It had PID: %d, exitStatus: %d\n",childPid,WEXITSTATUS(exitStatus));
    }else{
        printf("can't fork");
        exit(1);
    }
    return 0;
}