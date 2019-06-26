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
struct cfs_rq *cfs_rq;

unsigned long *_shares;
unsigned int *_nr_running;
unsigned long long *_quota, *_global_rt, *_cfs_b_rt_expires, *_cfs_rq_rt_expires, *_period;
signed long long *_rt_remaining;
int *_throttled_count;
int num_online_cpus;

struct task_group* get_task_group(int _pid) {
	pid_struct = find_get_pid(_pid);
	task = pid_task(pid_struct, PIDTYPE_PID);
	return task->sched_task_group;
}

void report(int _pid) {
	tg = get_task_group(_pid);
	cfs_b = &tg->cfs_bandwidth;


	_shares = &tg->shares;


	_period = &cfs_b->period;
	_quota = &cfs_b->quota;
	_global_rt = &cfs_b->runtime;
	_cfs_b_rt_expires = &cfs_b->runtime_expires;

	/* MAY WANT TO MAKE VARS IN LOOP BELOW INTO POINTERS TO ARRAYS OF SIZE #CPUS */
	int i;
	for(i=0;i < num_online_cpus; i++) {
		cfs_rq = tg->cfs_rq[i];
		_nr_running = &cfs_rq->nr_running;
		_cfs_rq_rt_expires = &cfs_rq->runtime_expires;
		_rt_remaining = &cfs_rq->runtime_remaining;
	}




}

static int __init report_tg_rt_usage_init(void)
{
	/* TODO Auto-generated Function Stub */

	printk(KERN_INFO "Sup. You are looking to find info about %d's task_group.\n", pid);
	num_online_cpus = num_online_cpus();
	//alloc those cfs_rq variable arrays here to num_online cpus
	//also have to redfine to pointer to pointer (e.g. **runtime_remaining)
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

