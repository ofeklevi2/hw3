#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "message_slot.h"

int main(int argc, char **argv){
    int fd, len, ioctal_ret_val;
    unsigned long channel_id;
    char user_buff[BUF_LEN];

    if (argc != 3){
        perror("wrong number of arguments\n");
        close(fd);
        exit(1);
    }

    fd = open(argv[1], O_RDONLY);
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

    len = read(fd, user_buff, BUF_LEN);
    if (len < 0){
        perror("read error");
        close(fd);
        exit(1);
    }
    close(fd);
    if((write(STDOUT_FILENO, user_buff, len)) != len){
        perror("write to stdout error");
        exit(1);
    }
    exit(0);
}