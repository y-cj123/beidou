#ifndef BD_COMBINE_H
#define BD_COMBINE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
void BD_send(unsigned char data[],int lenth,unsigned char my_addr[],unsigned char send_addr[],unsigned char send_data[]);

#endif
