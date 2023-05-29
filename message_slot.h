// based on chardev.h file shown in recitation 6                                                                                                                                                                                              
#ifndef MESSAGE_SLOT_H
#define MESSAGE_SLOT_H

#include <linux/ioctl.h>

// The major device number.                                                                                                                                                                                                                   
// We don't rely on dynamic registration                                                                                                                                                                                                      
// any more. We want ioctls to know this                                                                                                                                                                                                      
// number at compile time.                                                                                                                                                                                                                                                                                                                                                                                                                                   
#define MAJOR_NUM 235

// Set the message of the device driver                                                                                                                                                                                                       
#define MSG_SLOT_CHANNEL _IOW(MAJOR_NUM, 0, unsigned int)

#define DEVICE_NAME "char_dev"
#define BUF_LEN 128
#define DEVICE_FILE_NAME "simple_char_dev"
#define SUCCESS 0

// new structs to represent message slot and message channel
typedef struct channel{
    unsigned int channel_id;
    int message_len;
    char the_message[BUF_LEN];
    struct channel *next_channel;
}channel;

typedef struct channelsList{
    int len;
    struct channel *head;
}channelsList;

#endif


