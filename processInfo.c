
/*
	COMP 304 Spring 2017 Project-1
	Implemented by:
	Zafer Cavdar - 49995
	Cisem Altan - 49824
*/


#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/sched.h>

int processID = 0, processPrio = 0;
module_param(processID,int,0);
module_param(processPrio,int,0);

void print_details(struct task_struct *t);

int struct_task_init(void){
		printk(KERN_INFO "Loading Module\n");
		printk(KERN_INFO "%d %d\n",processID,processPrio);
		print_details(current);
		return 0;
}

void struct_task_exit(void) {
	printk(KERN_INFO "Removing Module\n");
}

void print_details(struct task_struct *t){
	struct task_struct *task = t;
	struct task_struct *temp_task;
	struct task_struct *temp_task2;
	struct list_head *list;
	struct list_head *list2;

	for_each_process(task) {
		if (task->pid == processID){
			//this pointlessly prints the name and PID of each task
			printk(KERN_INFO "PID: [%d]\n", task->pid);
			printk(KERN_INFO "Parent PID: [%d]\n", (task->parent)->pid);
			printk(KERN_INFO "Executable Name: [%s]",task->comm);

			list_for_each(list, &current->children) {
				temp_task = list_entry(list, struct task_struct, sibling);
				printk(KERN_INFO "\tChild PID: [%d]\n",temp_task->pid);
				printk(KERN_INFO "\tChild Executable Name: [%s]\n",temp_task->comm);
			}

			list_for_each(list2,&task->thread_group){
				temp_task2 = list_entry(list2, struct task_struct, sibling);
				printk(KERN_INFO "\tThread Group List Member PID: [%d]\n",temp_task2->pid);
				printk(KERN_INFO "\tThread Group List Member Name: [%s]\n",temp_task2->comm);
			}

			printk(KERN_INFO "user ID: %d\n",task->cred->uid);
			printk(KERN_INFO "Process Voluntary Context Switch Count: %lu\n",task->nvcsw);
			printk(KERN_INFO "Process Involuntary Context Switch Count: %lu\n",task->nivcsw);
			printk(KERN_INFO "Priority: %d\n",task->prio);
			printk(KERN_INFO "vruntime: %llu\n",(task->se).vruntime);
			task->prio = processPrio;
			printk(KERN_INFO "Updated Priority: %d\n",task->prio);
		}
	}

}

module_init(struct_task_init);
module_exit(struct_task_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Project 1 for COMP304");
MODULE_AUTHOR("Zafer Cavdar & Cisem Altan");
