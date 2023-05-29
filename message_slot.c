// based on chardev.c file shown in recitation 6  
// some error codes were taken from https://mariadb.com/kb/en/operating-system-error-codes/
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
channel find_channel(unsigned long channel_id, channelList *this_channelList){
    channel *channel = this_channelList->head;
    while (channel != NULL){
        if (channel->channel_id == channel_id){
            return channel;
        }
        channel = channel->next_channel;
    }
    return NULL; // if we got to this part, it means channel == NULL
}

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
    int minor_number = iminor(inode);
    printk("Invoking device_open(%p)\n", file);
    if (message_slots_array[minor_number] == NULL){
        message_slots_array[minor_number] = kmalloc(sizeof(channelsList), GFP_KERNEL);
        if (message_slots_array[minor_number] == NULL){
            printk("allocate memory error\n");
            return -ENOMEM;
        }
        message_slots_array[minor_number]->head = NULL;
        message_slots_array[minor_number]->len = 0;
    }
    return SUCCESS;
}

static long device_ioctl( struct   file* file,
                          unsigned int   ioctl_command_id,
                          unsigned long  ioctl_param){ 
    
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

// a process which has already opened                                                      
// the device file attempts to read from it                                                
static ssize_t device_read( struct file* file,
                            char __user* buffer,
                            size_t       length,
                            loff_t*      offset ){

    int minor_num, i;
    unsigned long channel_id;
    channel *this_channel;
    channelsList *this_channelList;
    minor_num = iminor(file->f_inode);
    if (file->private_data == NULL){
        printk("no channel_id");
        return -EINVAL;
    }
    channel_id = (file->private_data);
    this_channelList = message_slots_array[minor_num];
    this_channel = find_channel(channel_id);
    if (this_channel == NULL){
        printk("channel not found\n");
        return -EINVAL;
    }
    if (this_channel->message_len == 0){
        printk("there is not a message in minor %d, channel %lu", minour_num, channel_id);
    }
    if (length < this_channel->message_len){
        printk("user buffer to small\n");
        return -ENOSPC;
    }
    for (i = 0; i < length; i++){
        if ((put_user(this_channel->the_message[i], &buffer[i]) != 0)){
             printk("put_user error, can't read from this address\n");
             return -EFAULT;
        }   
    }
    printk("number of bytes read = %d", i);
    return i;
}

// a processs which has already opened                                                     
// the device file attempts to write to it                                                 
static ssize_t device_write( struct file*       file,
                             const char __user* buffer,
                             size_t             length,
                             loff_t*            offset){
    

}





//==================== DEVICE SETUP =============================  
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



