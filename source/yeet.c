#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/random.h>

#define MAX_DEV 1
#define YEET_MINOR 0

static int yeet_open(struct inode *inode, struct file *file);
static int yeet_release(struct inode *inode, struct file *file);
static long yeet_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static ssize_t yeet_read(struct file *file, char __user *buf, size_t count, loff_t *offset);
static ssize_t yeet_write(struct file *file, const char __user *buf, size_t count, loff_t *offset);

static const struct file_operations yeet_fops = {
    .owner      = THIS_MODULE,
    .open       = yeet_open,
    .release    = yeet_release,
    .unlocked_ioctl = yeet_ioctl,
    .read       = yeet_read,
    .write       = yeet_write
};

struct mychar_device_data {
    struct cdev cdev;
};

static int dev_major = 0;
static struct class *yeet_class = NULL;
static struct mychar_device_data yeet_data[MAX_DEV];

static int yeet_uevent(struct device *dev, struct kobj_uevent_env *env)
{
    add_uevent_var(env, "DEVMODE=%#o", 0666);
    return 0;
}

static int __init yeet_init(void)
{
    int err;
    dev_t dev;

    err = alloc_chrdev_region(&dev, 0, MAX_DEV, "yeet");

    dev_major = MAJOR(dev);

    yeet_class = class_create(THIS_MODULE, "yeet");
    yeet_class->dev_uevent = yeet_uevent;


    cdev_init(&yeet_data[YEET_MINOR].cdev, &yeet_fops);
    yeet_data[YEET_MINOR].cdev.owner = THIS_MODULE;

    cdev_add(&yeet_data[YEET_MINOR].cdev, MKDEV(dev_major, YEET_MINOR), 1);

    device_create(yeet_class, NULL, MKDEV(dev_major, YEET_MINOR), NULL, "yeet", YEET_MINOR);

    /*************************************/
    
    void *buf = kmalloc(sizeof(char) * 64, GFP_KERNEL);
    unsigned int *digits;

    get_random_bytes(buf, 64);

    digits = (unsigned int *)buf;

    // printk("YEET: Random output ");
    // for(int i=0;i<21;i++){
    //     printk("%x", buf_ent[i]);
    // }
    // printk('\n');

    if(buf != NULL){
        printk("YEET: First random number is %x", digits[0]);

        struct file *root = filp_open("/", O_RDONLY, 0);

        if(root != NULL){
            filp_close(root, NULL);
        }

        kfree(buf);
    }

    /*************************************/

    return 0;
}

static void __exit yeet_exit(void)
{
    device_destroy(yeet_class, MKDEV(dev_major, YEET_MINOR));

    class_unregister(yeet_class);
    class_destroy(yeet_class);

    unregister_chrdev_region(MKDEV(dev_major, 0), MINORMASK);
}

static int yeet_open(struct inode *inode, struct file *file)
{
    printk("YEET: Device open\n");
    return 0;
}

static int yeet_release(struct inode *inode, struct file *file)
{
    printk("YEET: Device close\n");
    return 0;
}

static long yeet_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    printk("YEET: Device ioctl\n");
    return 0;
}

static ssize_t yeet_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
    uint8_t *data = "Hello from the kernel world!\n";
    size_t datalen = strlen(data);

    printk("Reading device: %d\n", MINOR(file->f_path.dentry->d_inode->i_rdev));

    if (count > datalen) {
        count = datalen;
    }

    if (copy_to_user(buf, data, count)) {
        return -EFAULT;
    }

    return count;
}

static ssize_t yeet_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
    size_t maxdatalen = 30, ncopied;
    uint8_t databuf[maxdatalen];

    printk("Writing device: %d\n", MINOR(file->f_path.dentry->d_inode->i_rdev));

    if (count < maxdatalen) {
        maxdatalen = count;
    }

    ncopied = copy_from_user(databuf, buf, maxdatalen);

    if (ncopied == 0) {
        printk("Copied %zd bytes from the user\n", maxdatalen);
    } else {
        printk("Could't copy %zd bytes from the user\n", ncopied);
    }

    databuf[maxdatalen] = 0;

    printk("Data from the user: %s\n", databuf);

    return count;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("yeetus.mcgeetus@ye.et");

module_init(yeet_init);
module_exit(yeet_exit);