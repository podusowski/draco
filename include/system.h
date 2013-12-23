#ifndef __SYSTEM__H
#define __SYSTEM__H

#include "drax.h"

void uname(char * s);
void * createsharedframe(int pid);
bool freesharedframe();
int findprocess(char * name);
bool getmessage(message_t * msg);
int getpid();
int getprocesscount();
bool getprocessname(int pid, char * buf);
void setprocessstate(int pid, int state);
int getprocessstate(int pid);
unsigned long gettickcount();
bool isprocess(int pid);
bool sendmessage(int pid, message_t * msg);
void setmyname(char * name);
void puts(char * s);
void putc(char c);
void gets(char * s);
void sleep(unsigned long flags);
void enableirq(int irq);
void disableirq(int irq);
unsigned int getirqstatus(int irq);
void clearirqstatus(int irq);
int getbootdevice();

#endif /* __SYSTEM__H */
