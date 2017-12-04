#include<linux/miscdevice.h>
#include<linux/module.h>
#include<linux/slab.h>
#include<linux/init.h>
#include<linux/device.h>
#include<linux/kernel.h>
#include<linux/time.h>
#include<linux/fs.h>
#include<asm/uaccess.h>
#include<linux/semaphore.h>
int bufferSize;
module_param(bufferSize,int,0);
MODULE_LICENSE("GPL");
int noOfTimes=0;
char *message;
int count=0,read_count;
static int message_size;
DEFINE_SEMAPHORE(mutex);
DEFINE_SEMAPHORE(empty);
DEFINE_SEMAPHORE(full);
static int my_open(struct inode *, struct file *);
static int my_close(struct inode *,struct file *);
static ssize_t my_read(struct file *,char *,size_t,loff_t *);
static ssize_t my_write(struct file *,const char *, size_t, loff_t *);
static struct file_operations myfops={
.open=my_open,
.release=my_close,
.read=my_read,
.write=my_write,
};
static struct miscdevice my_misc_device={
.minor=MISC_DYNAMIC_MINOR,
.name="concurrency",
.fops=&myfops,
};
static int __init my_module_init(void){
int error,i;
error=misc_register(&my_misc_device);
if(error){
printk("Failed to register device");
return error;
}
printk("Device registered successfully!");
sema_init(&mutex,1);
sema_init(&empty,bufferSize);
sema_init(&full,0);
message=kmalloc(sizeof(char *)*bufferSize+1,GFP_KERNEL);
//for(i=0;i<bufferSize;i++){
//message[i]=kmalloc(sizeof(char *)*100,GFP_KERNEL);
//}
return 0;
}
static void __exit my_exit(void){
misc_deregister(&my_misc_device);
printk("Device un-registered!");
}
static int my_open(struct inode *filenode,struct file *filemode){
noOfTimes++;
printk("Concurrency device opened %d times",noOfTimes);
return 0;
}
static ssize_t my_read(struct file *file,char *out,size_t size,loff_t *offset){
int error=0;
printk("Message:%s",message);
if(!access_ok(VERIFY_WRITE,out,sizeof(out))){
return -EFAULT;
}
down_interruptible(&full);
down_interruptible(&mutex);
if(read_count<=bufferSize){
error=copy_to_user(out,message,message_size);
if(error!=0){
printk("Read not successfull!");
return -EFAULT;
}
printk("Message read successfully:%s",message);
count--;
up(&mutex);
up(&empty);
return strlen(message);
}
else
return -1;
}
static ssize_t my_write(struct file *file,const char *out,size_t size,loff_t *offset){
down_interruptible(&empty);
down_interruptible(&mutex);
if(count<=bufferSize){
if(copy_from_user(message,out,size)!=0){
return -EFAULT;
}
else{
message_size=strlen(message);
printk("Writing characters %s to user!",message);
count++;
up(&mutex);
up(&full);
return size;
}
}
else
return -1;
}
static int my_close(struct inode *filenode, struct file *filemode){
noOfTimes--;
printk("Device closed!");
return 0;
}
module_init(my_module_init);
module_exit(my_exit);
