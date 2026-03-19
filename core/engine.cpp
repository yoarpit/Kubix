#include <cstdio>
#include<sched.h>
#include<unistd.h>
#include<sys/mount.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include "../includes/engine.hpp"
#include<iostream>
#include<fstream>
#include<signal.h>
using namespace std;

#define STACK_SIZE (1028*1028)

static int container_main(void* arg){
  char* cmd =(char*)arg;

  sethostname("kubix",6);

  if(mount(NULL,"/",NULL,MS_REC|MS_PRIVATE,NULL)==-1){
    perror("Mount Private Failed");
  }

  chroot("./rootfs");
  chdir("/");


  mkdir("/proc",0555);
  mount("proc","/proc","proc",0,NULL);

  execlp(cmd, cmd, NULL);
  return 0;
  }

 void run_container(const char *cmd){
    char* stack=new char[STACK_SIZE];

    pid_t pid=clone(container_main,
                    stack+STACK_SIZE,
                    CLONE_NEWPID | CLONE_NEWUTS | CLONE_NEWNS |  SIGCHLD,
                   (void*)cmd);
    ofstream("kubix.pid")<<pid;
    cout<<"\033[32mContainer started PID:"<<pid<<endl;
    waitpid(pid,NULL,0);
}
  
 void stop_container(){
    ifstream f("kubix.pid");

    if (!f) {
        cerr << "PID file not found\n";
        return;
    }

    int pid;
    f >> pid;

    if (kill(pid, SIGTERM) == 0) {
          cout << "Container stopped gracefully\n";
    } else {
        perror("Failed to stop container");
    } }


  
 
