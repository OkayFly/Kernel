#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/leds.h>
#include <linux/slab.h>


#include <dt-bindings/gpio/gpio.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>
#include <linux/device.h>
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

static int gpio_led; 

static ssize_t gpio_led_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", gpio_get_value(gpio_led));
}


static ssize_t gpio_led_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	int val;

	int ret;

	ret = kstrtoint(buf, 10, &val);
	
	if(ret)
	{
		printk("%s: kstrtoint error return :%d\n", __func__, ret);
		return -1;
	}
	
	if(val == 1)
	{
		printk("%s:%d : gpio led value:%d\n",__func__, __LINE__, val);

		gpio_set_value(gpio_led, val);
	}
	else
	{
		printk("%s:%d: gpio led value:%d\n",__func__, __LINE__, val);
		gpio_set_value(gpio_led, val);


	}	
	
	return size;

}

static DEVICE_ATTR(gpio_led, 0664, gpio_led_show, gpio_led_store);

static int led_ctrl_probe(struct platform_device *pdev)
{
	struct device_node *led_ctrl_node = pdev->dev.of_node;
	enum of_gpio_flags flags;
	int gpio_value;
	printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printk("[%d] enter %s start...\n", __LINE__, __func__);
	
	gpio_led = of_get_named_gpio_flags(led_ctrl_node, "gpio_led", 0, &flags);//解析dts的gpio
	printk("gpio_led is %d \n", gpio_led);

	gpio_value = (flags == GPIO_ACTIVE_HIGH) ? 1:0;
	
	if(!gpio_is_valid(gpio_led))
	{
		printk("gpio led:%d is invalid\n", gpio_led);
		return -ENODEV;
	}

	if(gpio_request(gpio_led, "gpio_led"))
	{
		printk("gpio led:%d request failed!\n", gpio_led);
		gpio_free(gpio_led);
		return -ENODEV;
	}
	
	gpio_direction_output(gpio_led, gpio_value);

	printk("gpio_led pin level is %d\n",gpio_value);
	
	device_create_file(&pdev->dev, &dev_attr_gpio_led);
	printk("[%d]:__%s__sucess!\n",__LINE__, __func__);
	return 0;
}

static int led_ctrl_remove(struct platform_device *pdev)
{
	printk("[%d]:__%d__sucess!\n",__LINE__, __func__);
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



static int  __init led_ctrl_init(void)
{
	printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printk("#led_ctrl#:__%s__\n", __func__);
	platform_driver_register(&led_ctrl_driver);
	return 0;

}


static void __exit led_ctrl_exit(void)
{

	printk("#led_ctrl#:__%s__\n", __func__);
	platform_driver_unregister(&led_ctrl_driver);
}



module_init(led_ctrl_init);
module_exit(led_ctrl_exit);

MODULE_AUTHOR("flyfly@163.com");
MODULE_DESCRIPTION("Driver for control sysled");
MODULE_LICENSE("GPL");
