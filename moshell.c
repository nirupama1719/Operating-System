#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>
#include<stdbool.h>
void child_handler(){
int status;
waitpid(-1,&status,WNOHANG);
}
void signal_handler(){
printf("\n");
myShell();
}
void change_directory(char *dir){
char curr[100];
getcwd(curr,sizeof(curr));
strcat(curr,"/");
strcat(curr,dir);
chdir(curr);
}
int main(){
myShell();
return 0;
}
void myShell(){
char s[100],*proc_status;
bool background;
int child_pid[1000],num_child=0;
char *split,path[100],*copy[100];
signal(SIGCHLD,child_handler);
while(1){
signal(SIGINT,signal_handler);
printf("minish>");
fgets(s,100,stdin);
if(strlen(s)==0){
printf("Enter a valid command");
}else{
split=strtok(s," \n");
if(strcmp(split,"exit")==0){
exit(0);
}
int i=0;
while(split!=NULL){
copy[i]=split;
split=strtok(NULL," \n");
i++;
}
copy[i]=NULL;
strcpy(path,"/bin/");
strcat(path,copy[0]);
background=false;
if(strcmp(copy[0],"cd")==0){
change_directory(copy[1]);
}
if(strcmp(copy[0],"listjobs")==0){
for(int i=0;i<num_child;i++){
if(kill(child_pid[i],0)==0){
proc_status="RUNNING";
}
else{
proc_status="FINISHED";
}
printf("Process ID: %d Status : %s\n",child_pid[i],proc_status);
}
}
int pid=fork();
if(strcmp(copy[i-1],"&")==0){
copy[i-1]=NULL;
background=true;
}
if(pid<0){
printf("Fork function call failed!");
exit(-1);
}
if(pid==0){
execvp(path,copy);
exit(0);
}
if(!background){
wait(NULL);
}
else{
child_pid[num_child]=pid;
num_child++;
}
}
}
}
