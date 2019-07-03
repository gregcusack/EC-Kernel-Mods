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
struct cfs_rq *cfs_rq, *test;
struct sched_entity *se;

struct cfs_rq **cfs_rq_p;
struct sched_entity **se_p;
struct cpumask *_cpus_allowed;

unsigned long *_shares, *_se_runnable_weight, *_se_weight;
unsigned int *_nr_running, *_se_on_rq;
unsigned long long *_quota, *_global_rt, *_cfs_b_rt_expires, *_cfs_rq_rt_expires, *_period;
unsigned long long *_se_sum_exec_runtime, *_se_vruntime;

signed long long *_rt_remaining;
int *_throttled_count;
int num_online_cpus;

unsigned int **_nr_running_p;
unsigned long long **_cfs_rq_rt_expires_p;
signed long long **_rt_remaining_p;

unsigned long **_se_core_weight_p;
unsigned long long **_se_core_vruntime_p, **_se_core_sum_exec_runtime_p;

unsigned short mask = 0;

struct sched_entity* get_sched_entity(struct task_struct *ts) {
	return &ts->se;
}

//struct sched_entity* get_per_cpu_se(struct task_group *tg) {
//	return &tg->
//}

struct task_group* get_task_group(int _pid) {
	pid_struct = find_get_pid(_pid);
	if(!pid_struct) {
		return NULL;
	}
	task = pid_task(pid_struct, PIDTYPE_PID);
	_cpus_allowed = &task->cpus_allowed;
	if(!_cpus_allowed) {
		printk(KERN_INFO "can't get cpus allowed\n");
		return NULL;
	}
	mask = sizeof(*_cpus_allowed);// & (struct cpumask)0xF;
	printk(KERN_INFO "cpus allowed: %d\n", mask);

	se = get_sched_entity(task);
	if(!se) {
		printk(KERN_INFO "no se in task...\n");
		return NULL;
	}

	return task->sched_task_group;
}

int report(int _pid) {
	tg = get_task_group(_pid);
	if(!tg) {
		return -1;
	}
	cfs_b = &tg->cfs_bandwidth;

	_shares = &tg->shares;

	_period = &cfs_b->period;
	_quota = &cfs_b->quota;
	_global_rt = &cfs_b->runtime;
	_cfs_b_rt_expires = &cfs_b->runtime_expires;

	/* for specific process */
	_se_runnable_weight = &se->runnable_weight;
	_se_weight = &se->load.weight;
	_se_on_rq = &se->on_rq;
	_se_sum_exec_runtime = &se->sum_exec_runtime;
	_se_vruntime = &se->vruntime;


	/* MAY WANT TO MAKE VARS IN LOOP BELOW INTO POINTERS TO ARRAYS OF SIZE #CPUS */
	int i;
	for(i=0;i < num_online_cpus; i++) {
		/* CFS_RQ */
		cfs_rq_p[i] = tg->cfs_rq[i];
		_nr_running_p[i] = &cfs_rq_p[i]->nr_running;
		_cfs_rq_rt_expires_p[i] = &cfs_rq_p[i]->runtime_expires;
		_rt_remaining_p[i] = &cfs_rq_p[i]->runtime_remaining;

		/* SE */
		se_p[i] = tg->se[i];
		_se_core_weight_p[i] = &se_p[i]->load.weight;
		_se_core_sum_exec_runtime_p[i] = &se_p[i]->sum_exec_runtime;
		_se_core_vruntime_p[i] = &se_p[i]->vruntime;

	}

	int k,j;
	j = 0;
	while(j < 20) {
		printk(KERN_INFO "-------Global vals-------\n");
		printk(KERN_INFO "shares: %ld\n", *_shares);
		printk(KERN_INFO "period: %lld\n", *_period);
		printk(KERN_INFO "global_rt: %lld\n", *_global_rt);
		printk(KERN_INFO "cfs_b rt_expire: %lld\n", *_cfs_b_rt_expires);

		printk(KERN_INFO "-------per CPU cfs_rq values-------\n");
		for(k=0;k < num_online_cpus; k++) {
			if(*_nr_running_p[k] < 1) continue;
			printk("#### CPU %d ####\n", k);
			printk(KERN_INFO "nr_running: %d\n", *_nr_running_p[k]);
			printk(KERN_INFO "cfs_rq rt_expires: %lld\n", *_cfs_rq_rt_expires_p[k]);
			printk(KERN_INFO "rt_remaining: %lld\n", *_rt_remaining_p[k]);

			printk(KERN_INFO "weight: %ld\n", *_se_core_weight_p[k]);
			printk(KERN_INFO "sum_exec_runtime: %lld\n", *_se_core_sum_exec_runtime_p[k]);
			printk(KERN_INFO "vruntime: %lld\n", *_se_core_vruntime_p[k]);

		}
		printk(KERN_INFO "-----------Proc specific vals---------------\n");

//		printk(KERN_INFO "se runnable weight: %ld\n", *_se_runnable_weight);
		printk(KERN_INFO "se load weight: %ld\n", *_se_weight);
//		printk(KERN_INFO "se on rq: %d\n", *_se_on_rq);

		msleep(5);
		printk(KERN_INFO "finished loop: %d\n", j);
		j += 1;
	}

	return 0;




}

void alloc_mem(int n_cpus) {
	cfs_rq_p = (struct cfs_rq**)kmalloc(n_cpus*sizeof(struct cfs_rq*), GFP_KERNEL);
	_nr_running_p = (unsigned int **)kmalloc(n_cpus*sizeof(unsigned int*), GFP_KERNEL);
	_cfs_rq_rt_expires_p = (unsigned long long **)kmalloc(n_cpus*sizeof(unsigned long long*), GFP_KERNEL);
	_rt_remaining_p = (signed long long **)kmalloc(n_cpus*sizeof(signed long long*), GFP_KERNEL);

	se_p = (struct sched_entity**)kmalloc(n_cpus*sizeof(struct sched_entity*), GFP_KERNEL);
	_se_core_weight_p = (unsigned long **)kmalloc(n_cpus*sizeof(unsigned long*), GFP_KERNEL);
	_se_core_sum_exec_runtime_p = (unsigned long long **)kmalloc(n_cpus*sizeof(unsigned long long*), GFP_KERNEL);
	_se_core_vruntime_p = (unsigned long long **)kmalloc(n_cpus*sizeof(unsigned long long*), GFP_KERNEL);
}

static int __init report_tg_rt_usage_init(void)
{
	/* TODO Auto-generated Function Stub */

	printk(KERN_INFO "Sup. You are looking to find info about %d's task_group.\n", pid);
	num_online_cpus = num_online_cpus();
	printk(KERN_INFO "num online cpus: %d\n", num_online_cpus);
	alloc_mem(num_online_cpus);

	if (report(pid) < 0) {
		printk(KERN_INFO "PID not found.\n");
	}

	return 0;
}

static void __exit report_tg_rt_usage_exit(void)
{	
	/* TODO Auto-generated Function Stub */

	PINFO("---------------------EXIT-----------------\n");

}

module_init(report_tg_rt_usage_init);
module_exit(report_tg_rt_usage_exit);

