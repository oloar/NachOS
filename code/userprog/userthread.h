#ifndef USERTHREAD_H
#define USERTHREAD_H

int do_UserThreadCreate(int f, int arg);
void do_UserThreadExit();
int do_UserThreadJoin(int tid);
void do_MainThreadExit();
int do_ForkExec(int addr);
int do_ForkWait(int pid);

#endif // USERTHREAD_H
