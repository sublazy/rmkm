#include <linux/module.h>
#include <linux/kernel.h>

/* Kernel Module Boilerplate
 * -------------------------------------------------------------------------- */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cezary Salbut");
MODULE_DESCRIPTION("Rolling Median Kernel Module");
MODULE_VERSION("0.1");

int init_module(void)
{
    printk(KERN_INFO "RMKM: Hello!\n");
    return 0;
}

void cleanup_module(void)
{
    printk(KERN_INFO "RMKM: Cleaning up.\n");
}
