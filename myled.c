/*
 * myled.c
 * driver for to talk
 *
 * Copyright (C) 2017 MasayaOkawa <masamasa9841@gmail.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 * 
 *      You should have received a copy of the GNU General Public License
 *      along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/io.h>

MODULE_AUTHOR("MasayaOkawa");
MODULE_DESCRIPTION("driver for to talk");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;
static volatile u32*gpio_base = NULL;

static ssize_t led_write(struct file* filp, const char* buf, size_t count, loff_t* pos)
{
    char c;
    if(copy_from_user(&c,buf,sizeof(char)))
    return -EFAULT;

    if(c == '0')
        gpio_base[10] = 1 << 25;
    else if(c=='1')
        gpio_base[7] = 1 << 25;
    //printk(KERN_INFO "receive %c\n",c);
    return 1; 
}

static ssize_t sushi_read(struct file* filp, char* buf, size_t count, loff_t* pos)
{
    int size = 0;
    char sushi[][100] =
    {
        "空前絶後のぉォォ～!! 超絶怒涛のセクシー俳優\n",
        "セクシーを愛し セクシーに愛された男!!\n",
        "浮気 不倫 ダブル不倫\n",
        "全てのセクシーの 生みの親\n",
        "そう 我こそはぁぁぁ!!!\n",
        "身長184cm! 体重74kg! 貯金残高7625万円!!!\n",
        "キャッシュカードの暗証番号2741!!\n",
        "財布は今 楽屋に置いてあります! 松本さん 今がチャンスです!!\n",
        "もう一度言います 『2741(ふなよい)』って覚えてくださぁぁーーい!!!\n",
        "そう 全てをさらけ出した俺は\n",
        "サンシャイーーン 斎～～～～～ (ボフッ) 藤ーーー!!!\n",
        "イエェェェ～イ!!!\n",
        "ジャスティス!!!\n"
    };
    if(copy_to_user(buf+size,(const char *)sushi, sizeof(sushi))){
        printk( KERN_INFO "sushi : copy_to_user faile" );
        return -EFAULT;
    }
    size += sizeof(sushi);
    return size;
}

static struct file_operations led_fops = {
        .owner = THIS_MODULE,
        .write = led_write,
        .read = sushi_read
};

static int __init init_mod(void)
{
    int retval;
    gpio_base = ioremap_nocache(0x3f200000,0xA0);

    const u32 led = 25;
    const u32 index = led/10;
    const u32 shift = (led%10)*3;
    const u32 mask = ~(0x7 << shift);
    gpio_base[index] = (gpio_base[index] & mask) | (0x1 << shift);

    retval =  alloc_chrdev_region(&dev, 0, 1, "myled");
    if(retval < 0){
        printk(KERN_ERR "alloc_chrdev_region failed.\n");
        return retval;
    }
    printk(KERN_INFO "%s is loaded. major:%d\n",__FILE__,MAJOR(dev));
    
    cdev_init(&cdv, &led_fops);
    retval = cdev_add(&cdv, dev, 1);
    if(retval < 0){
        printk(KERN_ERR "cdev_add failed. major:%d, minor:%d",MAJOR(dev),MINOR(dev));
        return retval;
    }
    cls = class_create(THIS_MODULE,"myled");
    if(IS_ERR(cls)){
         printk(KERN_ERR "class_create failed.");
         return PTR_ERR(cls);
    }
    device_create(cls,NULL,dev,NULL,"myled%d",MINOR(dev));
    return 0;
}

static void __exit cleanup_mod(void)
{
    cdev_del(&cdv);
    device_destroy(cls, dev);
    class_destroy(cls);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "%s is unloaded. major:%d\n",__FILE__,MAJOR(dev));
}

module_init(init_mod);
module_exit(cleanup_mod);
