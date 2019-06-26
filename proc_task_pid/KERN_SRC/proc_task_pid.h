
#define DRIVER_NAME "proc_task_pid"
#define PDEBUG(fmt,args...) printk(KERN_DEBUG"%s:"fmt,DRIVER_NAME, ##args)
#define PERR(fmt,args...) printk(KERN_ERR"%s:"fmt,DRIVER_NAME,##args)
#define PINFO(fmt,args...) printk(KERN_INFO"%s:"fmt,DRIVER_NAME, ##args)
#include<linux/init.h>
#include<linux/module.h>
#include <linux/sched.h>
#include <linux/cgroup-defs.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>
#include <linux/pid.h>
#include <linux/pid_namespace.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include "/home/greg/kernel-dev/linux-stable/kernel/sched/sched.h"
#include "/home/greg/kernel-dev/linux-stable/include/linux/cgroup-defs.h"
#include <linux/nsproxy.h>


#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GREG C");

int pid, len;
char *buf;
module_param(pid, int, 0);
MODULE_PARM_DESC(pid, "PID of proc to get task_struct");

//int read_proc(char *buf, char **start, off_t offset, int count, int *eof, void *data);
int read_proc(int _pid);


//
//int write_proc(struct file *file, const char *buf, int count, void *data) {
//	int ret;
//	char *id;
//	id = (char*)kmalloc(1000*sizeof(char), GFP_KERNEL);
//	printk(KERN_INFO "buf passed %s", buf);
//	if(copy_from_user(id, buf, count)) {
//		return -EFAULT;
//	}
//	printk(KERN_INFO "id passed %s", id);
//	p_id = simple_strtoul(id, NULL, 0);
//	printk(KERN_INFO "pid %d ret %d", p_id, ret);
//	return sizeof(buf);
//}
//
//void create_new_proc_entry(void) {
//	proc_write_entry = create_proc_entry(proc_name, 0666, NULL);
//	if(!proc_write_entry) {
//		printk(KERN_INFO "Error creating proc entry");
//		return -ENOMEM;
//	}
//	proc_write_entry->read_proc = read_proc;
//	proc_write_entry->Write_proc = write_proc;
//	printk(KERN_INFO "proc initialized");
//}
