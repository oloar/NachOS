#ifndef USERTHREAD_H
#define USERTHREAD_H

int do_UserThreadCreate(int f, int arg);
void do_UserThreadExit();
int do_UserThreadJoin(int tid);
int do_ForkExec(int addr);

#endif // USERTHREAD_H
