#include<linux/linkage.h>
#include<linux/export.h>
#include<linux/time.h>
#include<linux/uaccess.h>
#include<linux/printk.h>
#include<linux/slab.h>
asmlinkage long int sys_mosystemcall(struct timespec *currentTime){
struct timespec t;
t=current_kernel_time();
if(!access_ok(VERIFY_WRITE,currentTime,sizeof(currentTime))){
return -EFAULT;
}
copy_to_user(currentTime,&t,sizeof(t));
printk(KERN_ALERT"Current System Time=%ld",(t.tv_sec*1000000000)+t.tv_nsec);
return (t.tv_sec*1000000000)+t.tv_nsec;
}
EXPORT_SYMBOL(sys_mosystemcall);
