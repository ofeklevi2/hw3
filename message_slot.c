// based on chardev.c file shown in recitation 6  
#undef __KERNEL__
#define __KERNEL__
#undef MODULE
#define MODULE


#include <linux/kernel.h>   /* We're doing kernel work */
#include <linux/module.h>   /* Specifically, a module */
#include <linux/fs.h>       /* for register_chrdev */
#include <linux/uaccess.h>  /* for get_user and put_user */
#include <linux/string.h>   /* for memset. NOTE - not string.h!*/
#include <errno.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");

// header with basic settings
#include "message_slot.h"

static channelsList message_slots_array[257]; // each minor number <= 256

//================= HELP FUNCTIONS ================
void get_or_create_id_channel(int minor, int channel_id){

}

//================= DEVICE FUNCTIONS ================
// 1. In device_open(), the module will check if it has already created a channelList
//    data structure for the file being opened, and create one if not
// 2. device_write() is responsible for creating a new channel_id if hasnt been created yet 
// 3. device_read() only need to check whether or not a channel_id exists, and if not return error
// 4. we can assume that there will not be concurrent system call invocations, so we can save the desired channel_id 
//    in file->private_data field when calling device_ioctl()

static int device_open(struct inode *inode, struct file *file){
    int minor_number;
    printk("Invoking device_open(%p)\n", file);
    if (message_slots_array[minor_number + 1] == NULL){
        message_slots_array[minor_number + 1] = kmalloc;
    }
}

static long device_ioctl( struct   file* file,
                          unsigned int   ioctl_command_id,
                          unsigned long  ioctl_param ){ 
    
    // setting a channel_id (in ioctal_param number) in the opened message slot device file (file)

    //check if ioctal_command_id == MSG_SLOT_CHANNEL
    if (MSG_SLOT_CHANNEL != ioctl_command_id){
        printk("ioctal_command_id != MSG_SLOT_CHANNEL\n");
        return -EINVAL;
    }

    if (ioctl_param == 0){
        printk("ioctal_param (channel id) is 0\n")
        return -EINVAL;
    }
    minor = iminor(file->f_inode);
    printk( "Invoking ioctl: setting message_slot_file with minor = %d channel_id = %ld\n" minor, ioctl_param);
    file->private_data = (void *)ioctl_param; //we will use the file->private_data field to save the channel_id 
    return SUCCESS;
}



// This structure will hold the functions to be called                                                                                  
// when a process does something to the device we created                                                                               
struct file_operations Fops = {
  .owner          = THIS_MODULE,
  .read           = device_read,
  .write          = device_write,
  .open           = device_open,
  .unlocked_ioctl = device_ioctl,
  .release        = device_release,
};



