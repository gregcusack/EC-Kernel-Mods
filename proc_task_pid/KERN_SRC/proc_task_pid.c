/*
===============================================================================
Driver Name		:		proc_task_pid
Author			:		GREG C
License			:		GPL
Description		:		LINUX DEVICE DRIVER PROJECT
===============================================================================
*/

#include"proc_task_pid.h"

//int p_id;
struct pid *pid_struct;
struct task_struct *task;
struct task_group *tg;
struct cfs_bandwidth *cfs_b;
struct cgroup_subsys_state *_css;
struct cgroup_subsys *_ss;
struct cgroup *_cg;
struct nsproxy *_nsproxy;
struct cftype *_cftype, *_cftype_legacy;
//struct cpuacct_usage __percpu *_cpuusage;
const char * _name;
int *_id, *_throttled, *_cg_id, *_throttle_count, *_quota_cftype, tmp;
unsigned long long *_quota, *_rt_expires, *_vrt, *_sum_exec_rt, *_exec_start, *_utime, *_stime, *_cpuusage;
unsigned long long *_runtime;
unsigned long *_weight, *_cfs_weight, *_shares, *_se_weight;
signed long long *_rt_remaining, *_period;
unsigned int *_cpu, num_online_cpus;
int exit_flag;

////static struct proc_dir_entry *proc_write_entry;
//char *proc_name="task_by_pid"; //think this is the sys call we can use to call this mod from user space??
//
//int read_proc(char *buf, char **start, off_t offset, int count, int *eof, void *data) {
int read_proc(int _pid){
	int len = 0;
	pid_struct = find_get_pid(_pid);
	task = pid_task(pid_struct, PIDTYPE_PID);
	buf = (char *)kmalloc(1000*sizeof(char), GFP_KERNEL);
//	len = sprintf(buf, "\nname %s\n ", task->comm);
//	len = sprintf(buf, "cgroup_subsys name: %s\n", (*task->sched_task_group).css.ss->name);
	tg = task->sched_task_group;
	if (!tg) {
		printk(KERN_INFO "tg == NULL\n");
		return -1;
	}
	_css = &(tg->css);
	if(!_css) {
		printk(KERN_INFO "_css == NULL\n");
		return -1;
	}
	_ss = _css->ss;
	if(!_ss) {
		printk(KERN_INFO "_ss == NULL\n");
		return -1;
	}
	_id = &_ss->id;
	if(!_id) {
		printk(KERN_INFO "_id == NULL\n");
		return -1;
	}
	_name = _ss->name;
	if(!_name) {
		printk(KERN_INFO "_name == NULL\n");
		return -1;
	}
	printk(KERN_INFO "ss->id: %d\n", *_id);
	printk(KERN_INFO "ss->name: %s\n", _name);

	_cpu = &task->cpu;
	if(!_cpu) {
		printk(KERN_INFO "_cou == NULL\n");
		return -1;
	}
	printk(KERN_INFO "task on cpu: %d\n", *_cpu);

//	_cg = _css->cgroup;
//	if(!_cg) {
//		printk(KERN_INFO "_cg == NULL\n");
//		return -1;
//	}

	_cg_id = &_css->cgroup->id;
	if(!_cg_id) {
		printk(KERN_INFO "_cg_id == NULL\n");
		return -1;
	}
	printk(KERN_INFO "_cg_id: %d\n", *_cg_id);

	cfs_b = &(tg->cfs_bandwidth);
	if (!cfs_b) {
		printk(KERN_INFO "cfs_b == NULL\n");
		return -1;
	}

	_quota = &cfs_b->quota;
	_runtime = &cfs_b->runtime;
	_rt_expires = &cfs_b->runtime_expires;
	_period = &cfs_b->period;
	if(!_quota || !_runtime || !_rt_expires || !_period) {
		printk(KERN_INFO "_quota == NULL\n");
		return -1;
	}
	printk(KERN_INFO "quota: %lld\n", *_quota);
	printk(KERN_INFO "runtime: %lld\n", *_runtime);
	printk(KERN_INFO "rt_expires: %lld\n", *_rt_expires);
	printk(KERN_INFO "period: %lld\n", *_period);

	_throttled = &cfs_b->nr_throttled;
	if(!_throttled) {
		printk(KERN_INFO "_throttled == NULL\n");
		return -1;
	}
	printk(KERN_INFO "throttled: %d\n", *_throttled);


	_weight = &tg->se[0]->load.weight;
//	_cfs_weight = &tg->cfs_rq[0]->load.weight;
	_shares = &tg->shares;
	if(!_weight || !_shares) {
		printk(KERN_INFO "_weight == NULL\n");
		return -1;
	}
	printk(KERN_INFO "weight: %ld\n", *_weight);
	printk(KERN_INFO "tg shares: %ld\n", *_shares);

//	printk(KERN_INFO "# CPUS: %d\n", NR_CPUS);
	num_online_cpus = num_online_cpus();
	printk(KERN_INFO "# CPUS: %d\n", num_online_cpus);
	int k;
	for(k=0; k < num_online_cpus; k++) {
		_rt_remaining = &tg->cfs_rq[k]->runtime_remaining;
		printk(KERN_INFO "runtime remaining for CPU %d: %lld\n", k, *_rt_remaining);
	}

//	num_online_cpus = num_online_cpus();
////	printk(KERN_INFO "# online cpus: %d\n", num_online_cpus);
//
//	_nsproxy = task->nsproxy;
//	if(!_nsproxy) {
//		printk(KERN_INFO "_nsproxy == NULL\n");
//		return -1;
//	}

//	_se_weight = &tg->se[0]->cfs_rq->load.weight;
//	if(!_se_weight) {
//		printk(KERN_INFO "_se_weight == NULL\n");
//		return -1;
//	}
//	printk(KERN_INFO "se weight: %ld\n", *_se_weight);



//	_name = _nsproxy->cgroup_ns->root_cset->subsys[0]->ss->name;
//	printk(KERN_INFO "cgroup name: %s\n", _name);
//	printk(KERN_INFO "group_subsys_count: %d\n", CGROUP_SUBSYS_COUNT);

	/* GET NAMES OF CPU SUBSYS */
//	int k;
////	for(k=0; k < CGROUP_SUBSYS_COUNT; k++) {
//	for(k=0; k < 3; k++) {
//		_name = _nsproxy->cgroup_ns->root_cset->subsys[k]->ss->name;
//		printk(KERN_INFO "cgroup subsys names: %s\n", _name);
//		_css = _nsproxy->cgroup_ns->root_cset->subsys[k];
//		_ss = _nsproxy->cgroup_ns->root_cset->subsys[k]->ss;
//		_cftype = NULL;
//		list_for_each_entry(_cftype, &_ss->cfts, node) {
//			printk(KERN_INFO "name: %s\n", _cftype->name);
//		}
//	}

//	/* GET CPUACCT */
//	_name = _nsproxy->cgroup_ns->root_cset->subsys[6]->ss->name;
//	printk(KERN_INFO "cgroup subsys name: %s\n", _name);
//
////	_cpuusage = &_ss->cfts->read_u64
////	_name = _ss->cfts->name;
//	_css = _nsproxy->cgroup_ns->root_cset->subsys[6];
//	_ss = _nsproxy->cgroup_ns->root_cset->subsys[6]->ss;
//	_cftype = NULL;
//	list_for_each_entry(_cftype, &_ss->cfts, node) {
//		printk(KERN_INFO "name: %s\n", _cftype->name);
//	}
//
//	printk(KERN_INFO "done with names\n");
//
//	_cftype = NULL;
//	list_for_each_entry(_cftype, &_ss->cfts, node) {
//		printk(KERN_INFO "cpuusage: %lld\n", _cftype->read_u64(_css, _cftype));
//	}
//
//	printk(KERN_INFO "done with cpuusage\n");
//
//	_css = _nsproxy->cgroup_ns->root_cset->subsys[1];
//	_ss = _nsproxy->cgroup_ns->root_cset->subsys[1]->ss;
//	_cftype_legacy = _ss->legacy_cftypes;
//	tmp = _ss->legacy_cftypes->read_s64(_css, _cftype_legacy);
//	_quota_cftype = &tmp;
//	printk(KERN_INFO "cpu quota: %d\n", *_quota_cftype);

//	exit_flag= 0;
//	_sum_exec_rt = NULL;
//	_utime = NULL;
//	_stime = NULL;
//	_sum_exec_rt = &_nsproxy->cgroup_ns->root_cset->subsys[2]->cgroup->rstat_cpu->bstat.cputime.sum_exec_runtime;
//	_utime = &_nsproxy->cgroup_ns->root_cset->subsys[2]->cgroup->rstat_cpu->bstat.cputime.utime;
//	_stime = &_nsproxy->cgroup_ns->root_cset->subsys[2]->cgroup->rstat_cpu->bstat.cputime.stime;
//	if(!_sum_exec_rt) {
//		printk(KERN_INFO "sum_exec_rt == NULL\n");
//		exit_flag = 1;
//	}
////	if (!_utime) {
////		printk(KERN_INFO "utime == NULL\n");
////		exit_flag = 1;
////
////	}
//	if (!_stime) {
//		printk(KERN_INFO "stime == NULL\n");
//		exit_flag = 1;
//	}
//	if(exit_flag) {
//		return -1;
//	}
////	printk(KERN_INFO "utime: %p\n", _utime);
//	printk(KERN_INFO "stime: %p\n", _stime);
//	printk(KERN_INFO "sum_exec_rt: %p\n", _sum_exec_rt);

//	_ss = _nsproxy->cgroup_ns->root_cset->subsys[2]->ss;
//	if(!_ss->cpuusage) {
//		printk(KERN_INFO "_ss->cpuuage == NULL\n");
//		return -1;
//	}
//	_cpuusage = _nsproxy->cgroup_ns->root_cset->subsys[2]->ss->cpuusage;

//	int i;
//	for(i=0; i < num_online_cpus; i++) {
//		printk(KERN_INFO "--------------\nCPU: %d\n", i);
//		_exec_start = &tg->se[i]->exec_start;
//		_sum_exec_rt = &tg->se[i]->sum_exec_runtime;
//		_vrt = &tg->se[i]->vruntime;
//		_throttle_count = &tg->se[i]->my_q->throttle_count;
//		if(!_exec_start || !_sum_exec_rt || !_vrt || !_throttle_count) {
//			printk(KERN_INFO "exec_start, sum_exec_rt, or vrt == NULL");
//			return -1;
//		}
//		printk(KERN_INFO "_exec_start: %lld\n", *_exec_start);
//		printk(KERN_INFO "_sum_exec_rt: %lld\n", *_sum_exec_rt);
//		printk(KERN_INFO "_vrt: %lld\n", *_vrt);
//		printk(KERN_INFO "_throttle_count: %d\n", *_throttle_count);
//	}

	return len;
}

static int __init proc_task_pid_init(void)
{
	/* TODO Auto-generated Function Stub */
	printk(KERN_INFO "Sup, are looking for task_struct of pid: %d\n", pid);
//	int len;
	len = read_proc(pid);
	printk(KERN_INFO "length of task name: %d\n", len);
//	printk(KERN_INFO "this is the name of the task: %s\n", buf);
	return 0;
}

static void __exit proc_task_pid_exit(void)
{	
	/* TODO Auto-generated Function Stub */

	printk(KERN_INFO " Removing module...\n##########\n");
//	remove_proc_entry(proc_name, NULL);

}

module_init(proc_task_pid_init);
module_exit(proc_task_pid_exit);

