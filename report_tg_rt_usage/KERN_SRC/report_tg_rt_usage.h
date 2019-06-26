
#define DRIVER_NAME "report_tg_rt_usage"
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

int pid;
char *buf;
module_param(pid, int, 0);
MODULE_PARM_DESC(pid, "PID of proc to get task_struct");

//int read_proc(char *buf, char **start, off_t offset, int count, int *eof, void *data);
void report(int _pid);

struct task_group* get_task_group(int _pid);
