#include<stdio.h>
#include<unistd.h>
#include<sys/time.h>
#include<linux/unistd.h>
int main(){
struct timespec currentTime;
long int time=syscall(326,&currentTime);
printf("System call sys-mosystemcall returned:%ld\n",(currentTime.tv_sec*1000000000)+currentTime.tv_nsec);
return 0;
}
