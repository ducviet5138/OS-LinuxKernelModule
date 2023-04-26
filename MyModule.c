#include <linux/module.h> 
#include <linux/random.h> // get_random_bytes lib
#include <linux/types.h> // <Major, Minor>
#include <linux/fs.h> // Register character device
#include <linux/cdev.h> // Cdev structure
#include <linux/device.h> // Create device file
#include <linux/uaccess.h> // Copy to user
#include <linux/miscdevice.h> // Misc device

MODULE_LICENSE("GPL");
MODULE_AUTHOR("21127112 - 21127141 - 21127203"); 
MODULE_DESCRIPTION("A sample loadable kernel module"); 
MODULE_SUPPORTED_DEVICE("MyDevice"); 

// Struct
struct cdev *MyCdev;
struct class *MyClass;

// Rand Int
int RandInt(void)
{
    int Result = 0;
    get_random_bytes(&Result, sizeof(Result));

    return Result;
}

// File operation
static ssize_t MyModule_Read(struct file *file, char *buf, size_t count, loff_t *ppos)
{
    int Rand = RandInt();
    copy_to_user(buf, &Rand, sizeof(Rand));
    return 0;
}

static struct file_operations MyModule_fops =
{
    .owner = THIS_MODULE,
    .read = MyModule_Read,
};

// Chmod 666
static struct miscdevice MyModule_miscdev =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "MyModule",
    .fops = &MyModule_fops,
    .mode = 0666,
};


// Init, Free
static int __init init_MyModule(void)
{
    printk("Install MyModule\n");
    MyCdev = cdev_alloc();

    if (alloc_chrdev_region(&MyCdev->dev, 0, 1, "MyModule") < 0)
    {
        printk("Failed to register device number\n");
    } else 
    {
        MyClass = class_create(THIS_MODULE, "Class_MyModule");
        misc_register(&MyModule_miscdev);
        device_create(MyClass, NULL, MyCdev->dev, NULL, "MyModule");
    }

    
    MyCdev->owner = THIS_MODULE;
    MyCdev->ops = &MyModule_fops;
    
    if (cdev_add(MyCdev, MyCdev->dev, 1) < 0)
    {
        printk("Failed to add device\n");
    }

    return 0;
}

static void __exit exit_MyModule(void)
{
    unregister_chrdev_region(MyCdev->dev, 1);
    cdev_del(MyCdev);
    device_destroy(MyClass, MyCdev->dev);
    class_destroy(MyClass);
    misc_deregister(&MyModule_miscdev);
    
    printk("Remove MyModule\n");
}

module_init(init_MyModule);
module_exit(exit_MyModule);
