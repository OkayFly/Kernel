#include <dt-bindings/gpio/gpio.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/wait.h>
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/err.h> 



struct led_data {//led设备属性
	int major;//主设备号
	int minor;//次设备号
	dev_t devid;//设备id
	struct cdev cdev; //字符设备
	struct class *class;//设备类
	struct device *device;//设备
	struct device_node *node;//设备节点
	int pin;
};

static struct led_data led;//申明一个驱动实例

static int led_open(struct inode *inode, struct file *file) {
	file->private_data = &led;//
	return 0;
}

static ssize_t led_write(struct file *file, const char __user *buf, size_t size, loff_t *lof) {
	char  data[2];
	int res = 0;
	res = copy_from_user(data, buf, size);
	if(res < 0)
	{
		printk("Kernel write err\n");
		return -EFAULT;
	}else {
		printk("copy_from_user is ok, buf =%d\n", data[0]);
	}

	if(data[0] == 1) {
		gpio_set_value(led.pin, 1);
	}else if(data[0] == 0) {
		gpio_set_value(led.pin,0);
	}

	return 0;
}

static int led_release(struct inode *inode, struct file *fiel) {
	return 0;
}

static int gpio_led; //定义一个GPIO变量，用gpio_led表示

static ssize_t gpio_led_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", gpio_get_value(gpio_led)); //获取GPIO的电平，1为高电平，0为低电平
}


static ssize_t gpio_led_store(struct device *dev, struct device_attribute *attr,
						const char *buf, size_t size)
{
	int val;
	int ret;

	ret = kstrtoint(buf, 10, &val); //输入一个字符然后转换成10进制整形数

        if (ret) {
		printk("%s: kstrtoint error return %d\n", __func__, ret);
		return -1;
        }

	if (val== 1) { //如果echo 1 到节点则调用

		printk("-czd-: _%s_ :gpio pull up\n", __func__);
		gpio_direction_output(gpio_led, val); //设置GPIO电平为高电平

	} else if (val == 0) { //如果echo 0 到节点则调用

		printk("-czd-: _%s_ :gpio pull down\n", __func__);
		gpio_direction_output(gpio_led, val); //设置GPIO电平为低电平

	} else {

		printk("I only support 0 or 1 to ctrl led\n");

	}
	return size;
}

static DEVICE_ATTR(gpio_led, 0664, gpio_led_show, gpio_led_store); //调用DEVICE_ATTR创建设备节点


/*File operations struct for character device*/
static const struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.open  = led_open,
	.write = led_write,
	.release = led_release,
};


static int led_ctrl_probe(struct platform_device *pdev) //compatible的属性和dts的compatible一致，就会调用probe函数
{

	struct device_node *led_ctrl_node = pdev->dev.of_node;
	enum of_gpio_flags flags;
	int gpio_value;
	int err;
        printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
        printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
        printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
        printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
        printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printk("[%d] enter %s start..\n", __LINE__, __func__); //printk打印，kernel一般调用这个函数打印log

	

	//
	alloc_chrdev_region(&led.devid, 0, 1, "led_zx");
	led.major = MAJOR(led.devid);
	cdev_init(&led.cdev, &led_fops);
	cdev_add(&led.cdev, led.devid, 1);
	led.class = class_create(THIS_MODULE, "led_zx");
	if(IS_ERR(led.class)) {
		return PTR_ERR(led.class);
	}
	
	led.device = device_create(led.class, NULL, led.devid, NULL, "led_zx");

	if(IS_ERR(led.device)){
		return PTR_ERR(led.device);
	}

	//
	gpio_led = of_get_named_gpio_flags(led_ctrl_node, "gpio_led", 0, &flags); //解析dts的gpio
	led.pin = gpio_led;
	printk("gpio_led is %d --\n", gpio_led);

	gpio_value = (flags == GPIO_ACTIVE_HIGH)? 1:0;

	if (!gpio_is_valid(gpio_led)) { //判断GPIO是否合法能用
		printk("gpio_led: %d is invalid\n", gpio_led);
		return -ENODEV;
	}

	if (gpio_request(gpio_led, "gpio_led")) { //申请GPIO口资源
		printk("gpio_led: %d request failed!\n", gpio_led);
		gpio_free(gpio_led); //如果申请失败，要释放GPIO的占用
		return -ENODEV;
	}

	gpio_direction_output(gpio_led, !gpio_value); //设置GPIO初始电平为低电平
	printk("gpio_led pin level is %d\n", !gpio_value); //这里gpio_value取反!gpio_value，是低电平
	 err = device_create_file(&pdev->dev, &dev_attr_gpio_led);
	if(err)
	{
		printk("#########################################sys file creation failed\n");
	}

	printk("[%d]: ___%s___ sucess!\n", __LINE__, __func__);

	
	return 0;
}

static int led_ctrl_remove(struct platform_device *pdv)
{
	printk("___%s___\n", __func__);

	gpio_set_value(led.pin, 0);
	cdev_del(&led.cdev);
	unregister_chrdev_region(led.devid, 1);
	device_destroy(led.class, led.devid);
	class_destroy(led.class);
	return 0;
}

static struct of_device_id led_ctrl_match_table[] = {
	{ .compatible = "rk3288,led_ctrl",},
	{},
	};

static struct platform_driver led_ctrl_driver = {

	.driver = {
		.name = "led_ctrl",
		.owner = THIS_MODULE,
		.of_match_table = led_ctrl_match_table,
	},
	.probe = led_ctrl_probe,
	.remove = led_ctrl_remove,
	};

static int led_ctrl_init(void)
{
	printk("#led_ctrl#: ___%s___\n", __func__);
	platform_driver_register(&led_ctrl_driver);
	return 0;
}

static void led_ctrl_exit(void)
{
	printk("#led_ctrl#: ___%s___,\n", __func__);
	//cancel_delayed_work_sync(&gpioled_event);  //取消延时工作队列
	platform_driver_unregister(&led_ctrl_driver);

}


/*设备文件操作方法表*/

module_init(led_ctrl_init); //模块加载函数
module_exit(led_ctrl_exit); //模块卸载函数

MODULE_AUTHOR("czd,214241976@qq.com");
MODULE_DESCRIPTION("Driver for control sysled");
MODULE_LICENSE("GPL");  //许可声明, 描述内核模块的许可权限，如果不声明LICENSE，模块被加载时，将收到kernel tainted的警告



