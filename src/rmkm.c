#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <stdbool.h>
#include <linux/ctype.h>

#include "int_parser.h"

/* Kernel Module Boilerplate
 * -------------------------------------------------------------------------- */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cezary Salbut");
MODULE_DESCRIPTION("Rolling Median Kernel Module");
MODULE_VERSION("0.1");

/* Function Prototypes
 * -------------------------------------------------------------------------- */
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

/* Constants
 * -------------------------------------------------------------------------- */
#define SUCCESS 0

// Dev name as it appears in /proc/devices.
#define DEVICE_NAME "median"

/* Private Data
 * -------------------------------------------------------------------------- */
// Descriptor of our character device.
static struct cdev dev_descr;

 // Device class descriptor for our character device.
static struct class *dev_class_descr;

// Major/Minor numbers of our char device.
static dev_t dev_id;

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release,
};

// Buffer to hold the answer (median calculation result).
static char ans_buf[32];

// Buffer to hold the input data.
#define INPUT_BUF_SIZE   32
static char input_buf[INPUT_BUF_SIZE];

/* Function Definitions
 * -------------------------------------------------------------------------- */
int init_module(void)
{
    printk(KERN_INFO "RMKM: Hello!\n");

    if (alloc_chrdev_region(&dev_id, 0, 1, DEVICE_NAME) < 0) {
        return -1;
    }

    dev_class_descr = class_create(THIS_MODULE, "chardrv");
    if (dev_class_descr == NULL) {
        unregister_chrdev_region(dev_id, 1);
        return -1;
    }

    if (device_create(dev_class_descr, NULL, dev_id, NULL, DEVICE_NAME)
                                                                    == NULL) {
        class_destroy(dev_class_descr);
        unregister_chrdev_region(dev_id, 1);
        return -1;
    }

    cdev_init(&dev_descr, &fops);

    if (cdev_add(&dev_descr, dev_id, 1) == -1) {
        device_destroy(dev_class_descr, dev_id);
        class_destroy(dev_class_descr);
        unregister_chrdev_region(dev_id, 1);
        return -1;
    }

    return SUCCESS;
}

void cleanup_module(void)
{
    printk(KERN_INFO "RMKM: Cleaning up.\n");
    cdev_del(&dev_descr);
    device_destroy(dev_class_descr, dev_id);
    class_destroy(dev_class_descr);
    unregister_chrdev_region(dev_id, 1);
}

static int
device_open(struct inode *inode, struct file *file)
{
    return SUCCESS;
}

static int
device_release(struct inode *inode, struct file *file)
{
    return SUCCESS;
}

static ssize_t
device_read(struct file *filp, char __user *buf, size_t len, loff_t *offset)
{
    // int response for now, apparently floats in drivers are non-trivial.
    int ans = 42;
    int status = 0;
    static size_t ans_len = 0;

    sprintf(ans_buf, "%d\n", ans);
    ans_len = strlen(ans_buf);
    *offset += ans_len;

    status = copy_to_user(buf, ans_buf, ans_len);
    if (status != 0) {
        int bytes_not_copied = status;
        *offset += ans_len - bytes_not_copied;
        return -EFAULT;
    }

    if (*offset > ans_len) {
        ans_len = 0;
    }

    return ans_len;
}

static bool isvalid(char c)
{
    if (isdigit(c) || isspace(c) || c == '-')
        return true;
    else
        return false;
}

static void median_calc_feed(int number)
{
    (void) number;
}

static ssize_t
device_write(struct file *filp, const char __user *buf, size_t recvd_data_len,
                loff_t *offset)
{
    int status = copy_from_user(input_buf, buf, recvd_data_len);
    if (status != 0) {
        return -EFAULT;
    }

    char *p = input_buf;

    while (p - input_buf < recvd_data_len) {

        if (!isvalid(*p))
            return -EINVAL;

        int_parser_feed(*p);

        if (int_parser_is_num_ready()) {
            int new_input_num = int_parser_get_num();
            median_calc_feed(new_input_num);
            printk(KERN_INFO "RMKM: input num: %d\n", new_input_num);
        }

        p++;
    }

    return recvd_data_len;
}

