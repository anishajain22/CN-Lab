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
    cout<<"ahaa 2"<<endl;
//    exit(EXIT_SUCCESS);
}
//
//int main() {
//    cout<<"podcast 2"<<endl;
////    char myfif1[]="/Users/himanshukhichi/Documents/fifi2";
//    signal(SIGIO,handle_signal);
//    key_t key2 = ftok("shmfile2",65);
//    int shmid2= shmget(key2,1024,0666|IPC_CREAT);
//    int *str2 = (int*) shmat(shmid2,(void*)0,0);
//    *str2=getpid();
//    cout<<*str2<<endl;
////    string s = to_string(a);
////    char const *pchar= s.c_str();
////    int fd=open(myfif1,O_WRONLY);
////    write(fd,&pchar,100);
////    close(fd);
//    int a;
//    cin>>a;
//    setpgid(*str2,a);
//    while(1)
//    {
//        cout<<getpgid(*str2)<<endl;
//        sleep(2);
//    }
//}
int main()
{
    signal(SIGIO,handle_signal);
    while(1);
}
