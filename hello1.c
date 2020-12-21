#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <hello1.h>
#include <linux/slab.h>
#define DEBUG

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("HelloWorld");
MODULE_AUTHOR("Peter Polovinkin <>");

EXPORT_SYMBOL(print_hello);
static struct my_list_head *head;

int clear_list(void)
{
    struct my_list_head* temp_first;
    struct my_list_head* temp_second;

    temp_first = head;
    if(temp_first == NULL){
    	temp_first = temp_first->next;
    }
 
    while(temp_first != NULL) {
	temp_second = temp_first->next;
	kfree(temp_first);
	temp_first = temp_second;
    }
    
    return 0;
}
int print_hello(uint count)
{
    int i;
    struct my_list_head *temp_head_1;
    struct my_list_head *temp_head_2;
    BUG_ON(count > 10);

    head = kmalloc(sizeof(struct my_list_head*), GFP_KERNEL);

    temp_head_1 = head;

    if (count == 0)
    {
        pr_warn("WARNING\ncount = 0\n");
        return 0;
    }
    else if (count >= 5 && count <= 10)
    {
        pr_warn("WARNING\ncount is between 5 and 10\n");
    }
    
    //BUG_ON instead
    /*else if (count > 10)
    {
        pr_err("ERROR\n count > 10");
        return -EINVAL;
    }*/
    
    for (i = 0; i < count; i++)
    {
        temp_head_1->next = kmalloc(sizeof(struct my_list_head*), GFP_KERNEL);
        if(i == 8){
        	temp_head_1 = NULL;
        }
        
      	if(temp_head_1 == NULL){
      		goto clear;
      	}
        temp_head_1->time = ktime_get();
        pr_info("Hello, world\n");
        temp_head_1->post_time = ktime_get();
        temp_head_2 = temp_head_1;
        temp_head_1 = temp_head_1->next;
    }
    
    return 0;
    
    clear:
    	pr_err("ERROR\t OUT OF MEMORY");
    	clear_list();
    	return -ENOMEM;

}

static int __init hello1_init(void)
{
    pr_info("hello1 init");
    return 0;
}

static void __exit hello1_exit(void)
{
    struct my_list_head* temp_1 = head;
    struct my_list_head* temp_2 = temp_1;
    pr_debug("hello1 exit start\n");
    while(temp_1 != NULL)
    {
        pr_debug("Time spent is %lld", temp_1->post_time - temp_1->time);
        temp_2 = temp_1;
        temp_1=temp_2->next;
        kfree(temp_2);
    }
    
    pr_debug("hello1 exit end\n");
}

module_init(hello1_init);
module_exit(hello1_exit);
