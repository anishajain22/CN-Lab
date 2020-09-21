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
using namespace std;
void handle_signal(int sig) {
    cout<<"ahaa 3"<<endl;
//    exit(EXIT_SUCCESS);
}

//int main() {
//    cout<<"podcast3"<<endl;
////    char myfif2[]="/Users/himanshukhichi/Documents/fifi3";
//    signal(SIGIO, handle_signal);
//    key_t key3 = ftok("shmfile3",66);
//    int shmid3= shmget(key3,1024,0666|IPC_CREAT);
//    int *str3 = (int*) shmat(shmid3,(void*)0,0);
//    *str3=getpid();
////    string s = to_string(a);
////    char const *pchar= s.c_str();
////    int fd=open(myfif2,O_WRONLY);
////    write(fd,&pchar,100);
////    close(fd);
//    cout<<*str3<<endl;
//    while(1);
//}
int main()
{
    signal(SIGIO,handle_signal);
    while(1);
}
