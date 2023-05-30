#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "message_slot.h"

int main(int argc, char **argv){
    int fd, ioctal_ret_val, len, write_ret_val;
    unsigned long channel_id;
    if (argc != 4){
        perror("wrong number of arguments\n");
        close(fd);
        exit(1);
    }
    fd = open(argv[1], O_WRONLY);
    if (fd < 0){
        perror("open file error\n");
        close(fd);
        exit(1);
    }
    channel_id = atoi(argv[2]);
    ioctal_ret_val = ioctl(fd, MSG_SLOT_CHANNEL, channel_id);
    if (ioctal_ret_val < 0){
        perror("ioctal error\n");
        close(fd);
        exit(1);
    }
    len = strlen(argv[3]);
    write_ret_val = write(fd, argv[3], len);
    if (write_ret_val != len){
        perror("write error\n");
        close(fd);
        exit(1);
    }

    close(fd);
    exit(0);

}