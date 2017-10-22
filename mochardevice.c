#include<linux/miscdevice.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/device.h>
#include<linux/kernel.h>
#include<linux/time.h>
#include<linux/fs.h>
#include<asm/uaccess.h>
MODULE_LICENSE("GPL");
int noOfTimes=0;
static int my_open(struct inode *, struct file *);
static int my_close(struct inode *,struct file *);
static ssize_t my_read(struct file *,char *,size_t,loff_t *);
static struct file_operations myfops={
.open=my_open,
.release=my_close,
.read=my_read,
};
static struct miscdevice my_misc_device={
.minor=MISC_DYNAMIC_MINOR,
.name="my device",
.fops=&myfops,
};
static int __init my_module_init(void){
int error;
error=misc_register(&my_misc_device);
if(error){
printk("Failed to register device");
return error;
}
printk("Device registered successfully!");
return 0;
}
static void __exit my_exit(void){
misc_deregister(&my_misc_device);
printk("Device un-registered!");
}
static int my_open(struct inode *filenode,struct file *filemode){
noOfTimes++;
printk("Device opened %d times",noOfTimes);
return 0;
}
static ssize_t my_read(struct file *file,char *out,size_t size,loff_t *off){
int error=0;
struct timespec time,time1;
time=current_kernel_time();
getnstimeofday(&time1);
char buf[100];
if(!access_ok(VERIFY_WRITE,out,sizeof(out))){
return -EFAULT;
}
sprintf(buf,"\ncurrent_kernel_time:\t%ld \t%ld \ngetnstimeofday:\t\t%ld \t%ld \n",time.tv_sec,time.tv_nsec,time1.tv_sec,time1.tv_nsec);
error=copy_to_user(out,buf,sizeof(buf)+1);
if(error==0){
printk("Time read successfully!");
printk("Time in buffer:%s",buf);
return 0;
}
else{
printk("Read operation failed!");
return -EFAULT;
}
}
static int my_close(struct inode *filenode, struct file *filemode){
noOfTimes--;
printk("Device closed!");
return 0;
}
module_init(my_module_init);
module_exit(my_exit);
