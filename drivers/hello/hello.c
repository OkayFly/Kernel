#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>

#include "hello.h"

/*主设备和从设备变量*/
static int hello_major = 0;
static int hello_minor = 0;

/*设备类别和设备变量*/
static struct class* hello_class = NULL;
static struct hello_android_dev *hello_dev = NULL;

/*传统设备文件操作方法 open release read write */
static int hello_open(struct inode *inode, struct file *filp);
static int hello_release(struct inode *inode, struct file *filp);
static ssize_t hello_read(struct file *filp, char __user *buf, size_t count , loff_t *f_pos);
static ssize_t hello_write(struct file *flip, const char __user *buf, size_t count, loff_t *f_pos);

/* 设备文件操作方法表*/
static struct file_operations hello_fops = {
	.owner = THIS_MODULE,
	.open = hello_open,
	.release = hello_release,
	.read = hello_read,
	.write = hello_write,
};
/*访问设置属性方法*/
static ssize_t hello_val_show(struct device *dev, struct device_attribute *attr, char *buf);
static ssize_t hello_val_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);

/* 定义设备属性*/
static DEVICE_ATTR(val, S_IRUGO|S_IWUSR, hello_val_show, hello_val_store);



/*打开设备方法*/
static int hello_open(struct inode *inode, struct file *filp)
{
	struct hello_android_dev *dev;

	
	/*将自定义设备结构体保存在文件指针的私有数据域中，以便访问设备时拿出来*/
	dev = container_of(inode->i_cdev, struct hello_android_dev, dev);
	filp->private_data = dev;
	return 0;
}

/*TODO设备释放时候调用*/
static int hello_release(struct inode *inode, struct file *filp)
{	
	return 0;

}

/* read */
static ssize_t hello_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	ssize_t err = 0;

	struct hello_android_dev *dev = filp->private_data;

	/*同步访问*/
	if(down_interruptible(&(dev->sem)))
	{	
		return -ERESTARTSYS;
	}
	
	if(count < sizeof(dev->val))
	{
		goto out;
	}

	/*将寄存器val的值拷贝到用户提供的缓冲区*/
	if(copy_to_user(buf, &(dev->val), sizeof(dev->val)))
	{
		err = -	EFAULT;
		goto out;
	}
	
	err = sizeof(dev->val);
	
out:
	up(&(dev->sem));

return err;

}

/*write */
static ssize_t hello_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	struct hello_android_dev *dev = filp->private_data;
	ssize_t err = 0;
	
	/*同步访问*/
	if(down_interruptible(&(dev->sem)))
	{
		return -ERESTARTSYS;	
	}
	
	if(count != sizeof(dev->val))
	{
		goto out;
	}
	
	if(copy_from_user(&(dev->val), buf, count)) 
	{
		err = -EFAULT;
		goto out;

	}
	
	err = sizeof(dev->val);

out:
	up(&(dev->sem));

	return err;

}

/* devfs show/store/get/set */
static ssize_t __hello_get_val(struct hello_android_dev *dev, char *buf)
{
	int val = 0;

	/*同步访问*/
	if(down_interruptible(&(dev->sem)))
	{
		return -ERESTARTSYS;	
	}
	
	val = dev->val;
	up(&(dev->sem));

	return snprintf(buf, PAGE_SIZE, "%d/n", val);
}


static ssize_t __hello_set_val(struct hello_android_dev *dev, const char *buf, size_t count)
{
	int val = 0;
	/*str->num*/
	val = simple_strtol(buf, NULL, 10);

	/*同步访问*/
	if(down_interruptible(&(dev->sem)))
	{
		return -ERESTARTSYS;	
	}
	
	dev->val = val;
	up(&(dev->sem));
	
	return count;	
}

static ssize_t hello_val_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct hello_android_dev *hdev = (struct hello_android_dev*)dev_get_drvdata(dev);
	return __hello_get_val(hdev, buf);
}

static ssize_t hello_val_store(struct device *dev, struct device_attribute *attr, const char* buf, size_t count)
{
	struct hello_android_dev *hdev = (struct hello_android_dev*) dev_get_drvdata(dev);
	return __hello_set_val(hdev, buf, count);
	
}

/* proc read write create remove*/
static ssize_t hello_proc_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	if(off > 0)
	{
		*eof = 1;
		return 0;
	}
	return __hello_get_val(hello_dev, page);
 }
 static ssize_t hello_proc_write(struct file *flip, const char __user *buff, unsigned long len, void *data)
 {
 	int err = 0; 
	char *page = NULL;
	 if(len > PAGE_SIZE)
	 {
		printk(KERN_ALERT"The buff is too larg:%lu./n", len); 
		return -EFAULT;	
	} 
	page = (char*)__get_free_page(GFP_KERNEL); 	
	if(!page) 
	{ 
		printk(KERN_ALERT"Failed to alloc page./n"); 
		return -ENOMEM;
	}

	/*USER BUFF -> KERNER BUFF*/
	if(copy_from_user(page, buff, len))
	{
		printk(KERN_ALERT"Failed to copy buff from user./n");
		err = -EFAULT;
		goto out;
	}
	
	err = __hello_set_val(hello_dev, page, len);
out:
	free_page((unsigned long)page);
	return err;
}

/*
static const struct file_operations  hello_fops_proc = 
{
	.owner = THIS_MODULE,
	.read = hello_proc_read,
	.write = hello_proc_write,

};
*/

/* create proc/hello file */
static void hello_create_proc(void)
{
	struct proc_dir_entry *entry;
	entry = proc_create(HELLO_DEVICE_PROC_NAME, S_IRUGO | S_IFREG, NULL, NULL);
	if(entry)
	{
		entry->owner = THIS_MODULE;
		entry->read_proc = hello_proc_read;
		entry->write_proc = hello_proc_write;
	}

	

}

static void hello_remove_proc(void)
{
	remove_proc_entry(HELLO_DEVICE_PROC_NAME, NULL); 
}


static int __hello_setup_dev(struct hello_android_dev *dev)
{
	int err;
	dev_t devno = MKDEV(hello_major, hello_minor);
	memset(dev, 0, sizeof(struct hello_android_dev));
	
	cdev_init(&(dev->dev), &hello_fops);
	dev->dev.owner = THIS_MODULE;
	dev->dev.ops = &hello_fops;

	/*注册字符设备*/
	err = cdev_add(&(dev->dev), devno, 1);
	if(err)
	{
		return err;
	}

	/*init */
	init_MUTEX(&(dev->sem));
	dev->val = 0;
	
	return 0;
}


static int __init hello_init(void)
{
	int err = -1;
	dev_t dev = 0;
	struct device *temp = NULL;
	
	printk(KERN_ALERT"Initializing hello device./h");

	err = alloc_chrdev_region(&dev, 0, 1, HELLO_DEVICE_NODE_NAME);
	if(err < 0)
	{
		printk(KERN_ALERT"Failed to alloc char dev region./n");
		goto fail;
	}

	hello_major = MAJOR(dev);
	hello_minor = MINOR(dev);

	hello_dev = kmalloc(sizeof(struct hello_android_dev), GFP_KERNEL);
	if(!hello_dev)
	{
		err = -ENOMEM;
		printk(KERN_ALERT"Failed to alloc hello_dev./n");
		goto unregister
	}

	err = __hello_setup_dev(hello_dev);
	if(err)
	{
		printk(KERN_ALERT"Failed to setup dev:%d.\n",err);
		goto cleanu;

	}

	hello_class = class_create(THIS_MODULE, HELLO_DEVICE_CLASS_NAME);
	if(IS_ERR(hello_class)
	{
	
		printk(KERN_ALERT"Failed to create hello class./n");
		goto destroy_cdev;

	}

	temp = device_create(hello_class, NULL, dev, "%s", HELLO_DEVICE_FILE_NAME);
	
	if(IS_ERR(temp))
	{
		err = PTR_ERR(temp);
		printk(KERN_ALERT"Failed t o create hello device.\n");
		goto destroy_class;
	}
	
	
	err = device_create_file(temp, &dev_attr_val);
	if(err < 0)
	{
		printk(KERN_ALERT"Failed to create attribute val.\n");

		goto destroy_device;
	}

	dev_set_drvdata(temp, hello_dev);

	hello_create_proc();	

	printk(KERN_ALERT"Succedded to initialize hello device./n");
	
	return 0;

destroy_device:
	device_destroy(hello_class, dev);

destroy_class:
	class_destryo(hello_class);
destroy_cdev:
	cdev_del(&(hello_dev->dev));

cleanup:
	kfree(hello_dev);

unregister:
	unregister_chrdev_region(MKDEV(hello_major, hello_minor),1);

fail:
	return err;
}

static void __exit hello_exit(void)
{
	dev_t devno = MKDEV(hello_major, hello_minor);
	
	printk(KERN_ALERT"Destroy hello device./n");

	hello_remove_proc();

	if(hello_class)
	{
		device_destroy(hello_class, MKDEV(hello_major, hello_minor));
		class_destroy(hello_class);
	}
	
	if(hello_dev)
	{
		cdv_del(&(hello_dev->dev));
		kfree(hello_dev);
	}
	unregiser_chrdev_region(devno, 1);
}


module_init(hello_init);
module_exit(hello_exit);
}

MODULE_LICENS("GPL");
MODULE_DESCRIPTION("First Android Driver");
}

