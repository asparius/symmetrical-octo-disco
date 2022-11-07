#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/pid.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/sched/signal.h>
// Meta Information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ME");
MODULE_DESCRIPTION("A module that knows how to greet");


int rootpid;
module_param(rootpid,int,0);
/*
 * module_param(foo, int, 0000)
 * The first param is the parameters name
 * The second param is it's data type
 * The final argument is the permissions bits,
 * for exposing parameters in sysfs (if non-zero) at a later stage.
 */

void dfs_traversal(struct task_struct *root);

// A function that runs when the module is first loaded
int simple_init(void) {
  struct task_struct *ts;

  ts = pid_task(find_get_pid(rootpid), PIDTYPE_PID);

  printk(KERN_INFO "Hello from the MyModule");
  if(ts == NULL){
	printk(KERN_INFO "Pid is not found");
	return 1;
	

  }

  dfs_traversal(ts);
  
  return 0;
}

// A function that runs when the module is removed
void simple_exit(void) {
  printk("Goodbye from the MyModule");
}

void dfs_traversal(struct task_struct * root){

	struct list_head *list;
	struct task_struct * tmp;
	printk(KERN_INFO "%d,%d,%llu",root->pid,root->parent->pid,root->start_time);
	
	list_for_each(list,&root->children){

		tmp = list_entry(list,struct task_struct, sibling);
		dfs_traversal(tmp);
	}
}

module_init(simple_init);
module_exit(simple_exit);
