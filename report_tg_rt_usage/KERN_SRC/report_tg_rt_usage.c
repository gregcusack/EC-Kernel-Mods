/*
===============================================================================
Driver Name		:		report_tg_rt_usage
Author			:		GC
License			:		GPL
Description		:		LINUX DEVICE DRIVER PROJECT
===============================================================================
*/

#include"report_tg_rt_usage.h"

struct pid *pid_struct;
struct task_struct *task;
struct task_group *tg;
struct cfs_bandwidth *cfs_b;

unsigned long *_shares, *_nr_running;
unsigned long long *_quota, *_global_rt, *_cfs_b_rt_expires, *_cfs_rq_rt_remaining, *_period;
signed long long *_rt_remaining;
int *_throttled_count;

struct task_group* get_task_group(int _pid) {
	pid_struct = find_get_pid(_pid);
	task = pid_task(pid_struct, PIDTYPE_PID);
	return task->sched_task_group;
}

void report(int _pid) {
	tg = get_task_group(_pid);
	cfs_b = &tg->cfs_bandwidth;

}

static int __init report_tg_rt_usage_init(void)
{
	/* TODO Auto-generated Function Stub */

	printk(KERN_INFO "Sup. You are looking to find info about %d's task_group.\n", pid);
	report(pid);

	return 0;
}

static void __exit report_tg_rt_usage_exit(void)
{	
	/* TODO Auto-generated Function Stub */

	PINFO("EXIT\n");

}

module_init(report_tg_rt_usage_init);
module_exit(report_tg_rt_usage_exit);

