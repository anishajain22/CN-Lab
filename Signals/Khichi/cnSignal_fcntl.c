#include<bits/stdc++.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<bits/stdc++.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include<poll.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include<sys/ipc.h>
#include <sys/shm.h>
#include <termios.h>
using namespace std;
//int fd[2],fd1[2],fd2[2];
//
//void handle_signal(int sig)
//{
////        return;
//    char buffer[100];
//    read(fd[0],buffer,100);
//    cout<<buffer<<"from 1"<<endl;
////    read(fd1[0],buffer,100);
////    cout<<buffer<<"from 2"<<endl;
////    read(fd2[0],buffer,100);
////    cout<<buffer<<"from 3"<<endl;
//}
//
//int main() {
//    cout<<"podcast"<<endl;
//    int pid1,pid2,pid3;
//    cin>>pid1>>pid2>>pid3;
////    key_t key2 = ftok("shmfile2",65);
//  setpgrp();
////    int shmid2= shmget(key2,1024,0666|IPC_CREAT);
////    int *str2 = (int*) shmat(shmid2,(void*)0,0);
////    key_t key3 = ftok("shmfile3",66);
////    int shmid3= shmget(key3,1024,0666|IPC_CREAT);
////    int *str3 = (int*) shmat(shmid3,(void*)0,0);
////    key_t key4 = ftok("shmfile4",67);
////    int shmid4= shmget(key4,1024,0666|IPC_CREAT);
////    int *str4 = (int*) shmat(shmid4,(void*)0,0);
//    signal(SIGIO, handle_signal);
//    pipe(fd);
//    pipe(fd1);
//    pipe(fd2);
//setpgid(pid1,getpid());
//setpgid(pid2,getpid());
//    fcntl(fd[0],F_SETFL, O_ASYNC | fcntl(fd[0], F_GETFL));
////    fcntl(fd1[0],F_SETFL, O_ASYNC | fcntl(fd1[0], F_GETFL));
////    fcntl(fd2[0],F_SETFL, O_ASYNC | fcntl(fd2[0], F_GETFL));
//    while(1)
//    {
//    write(fd[1],"hi",100);
////    write(fd1[1],"hi1",100);
////    write(fd2[1],"hi2",100);
//    fcntl(fd[0], F_SETOWN,getpid());
////    fcntl(fd1[0], F_SETOWN,pid2);
////   fcntl(fd2[0], F_SETOWN,pid3);
//    }
////    while(1);
//}
////    char myfif1[]="/Users/himanshukhichi/Documents/fifi2";
////    char myfif2[]="/Users/himanshukhichi/Documents/fifi3";
////    char myfif3[]="/Users/himanshukhichi/Documents/fifi4";
////    mkfifo(myfif1,0666);
////    mkfifo(myfif2,0666);
////    mkfifo(myfif3,0666);
////    int pid1,pid2,pid3;
////    char buffer[100];
////    int ffd=open(myfif1,O_RDONLY);
////    read(ffd,buffer,100);
////    pid1=atoi(buffer);
////    cout<<endl;
////    close(ffd);
////    int ffd1=open(myfif2,O_RDONLY);
////    cout<<endl;
////    read(ffd1,buffer,100);
////    pid2=atoi(buffer);
////    close(ffd1);
////    int ffd2=open(myfif2,O_RDONLY);
////    cout<<endl;
////    read(ffd2,buffer,100);
////    pid3=atoi(buffer);
////    close(ffd2);
//void hfn1(int sid)
//{
//    cout<<"podcast 1"<<endl;
//}
//void hfn2(int sid)
//{
//    cout<<"podcast 2"<<endl;
//}
int main()
{
    int c=0,d=0;
    c=fork();
    if(c==0)
    {
//        signal(SIGUSR1,hfn1);
        setpgid(getpid(),getppid());
        pid_t pid1=getppid();
//        cout<<getpid()<<"child"<<endl;
//        cout<<getpgrp()<<"child"<<endl;
        d=fork();
        if(d>0)
        {
//            signal(SIGUSR1,hfn2);
            setpgid(getpid(),pid1);
//            cout<<getpid()<<"child2"<<endl;
//            cout<<getpgrp()<<"cc"<<endl;
            execv("./cnSignal_fcntl3",NULL);
        }
        execv("./cnSignal_fcntl2",NULL);
    }
    else if(c>0){
        setpgrp();
//        cout<<getpgrp()<<endl;
//        cout<<getpid()<<"parent"<<endl;
//        int fd=dup(0);
//         ioctl(fd, TIOCSCTTY );
//        cout<<getpgrp()<<endl;
        sleep(5);
//        killpg(getpgrp(),SIGUSR1);
//        tcsetpgrp(fd,getpgrp());
        int fd[2];
        pipe(fd);
        cout<<"start"<<endl;
        fcntl(fd[0],F_SETFL, O_ASYNC | fcntl(fd[0], F_GETFL));
        while(1)
        {
        fcntl(fd[0], F_SETOWN,-1*getpgrp());
            sleep(2);
        }
    }
        
}
