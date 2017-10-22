#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<sys/time.h>
#include<stdlib.h>
#include<unistd.h>
#define N 5
char procClockTimes[N][200];
int main(){
struct timeval gtodTimes[N];
int fd,bytes_read;
fd=open("/dev/my device",O_RDONLY);
if(fd<0){
printf("Failed to open the device!");
return -1;
}
for(int i=0;i<N;i++){
gettimeofday(&gtodTimes[i],NULL);
bytes_read=read(fd,procClockTimes[i],sizeof(procClockTimes));
if(bytes_read<0){
printf("Failed to read from device!");
return -1;
}
}
close(fd);
for(int i=0;i<N;i++){
printf("gettimeoftheday: \t%ld\t%ld %s\n",gtodTimes[i].tv_sec,gtodTimes[i].tv_usec*1000,procClockTimes[i]);
}
return 0;
}
