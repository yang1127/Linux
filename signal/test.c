#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

void sigcb(int signo)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

int main()
{
    signal(SIGCHLD, sigcb); //SIGCHLD
    int pid = fork();
    if (pid == 0) 
    {
        sleep(3);
        exit(0);
    }
    while(1)
    {
        printf("hahahah~~\n");
        sleep(1);
    }
    return 0;
}
