#ifndef __IPC__H
#define __IPC__H

void ipc_sendmessage(uint32 pid, message_t * msg);
void ipc_getmessage(message_t * buf);

#endif
