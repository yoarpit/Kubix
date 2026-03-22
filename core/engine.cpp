#include <csignal>
#include <cstdio>
#include<sched.h>
#include<string>
#include<unistd.h>
#include<sys/mount.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include "../includes/engine.hpp"
#include<iostream>
#include<filesystem>
#include<fstream>
#include<signal.h>
using namespace std;
namespace fs = std::filesystem;

#define STACK_SIZE (1028*1028)
#define KUBIX_BASE "/var/lib/kubix"

struct ContainerConfig{
  string path;
  char* cmd;
};


static int container_main(void* arg){
  ContainerConfig* config = (ContainerConfig*)arg;

  sethostname("kubix",6);

  if (chroot(config->path.c_str()) != 0 || chdir("/") != 0) {
        perror("Kubix Error: Could not chroot into image");
        return 1;
    }
  
  mount("proc","/proc","proc",0,NULL);
  mount("tmpfs","/tmp","tmpfs",0,NULL);
  mount("devtmfps","/dev","tmpfs",0,NULL);

  char* args[] = {config->cmd, NULL};
    if (execvp(config->cmd, args) == -1) {
        perror("Kubix Error: execvp failed");
        return 1;
    }
  return 0;
  }


 void run_container(string image,string name,char *cmd,string mem, string cpu){
    string image_path= string(KUBIX_BASE)+"/images/"+image;
    string run_path = string(KUBIX_BASE)+"/run";

    if(!fs::exists(image_path)){
      cout<<"Kubix image:"<<image<<" Not Found,Pull it first"<<endl;
      exit(1);
    }
    fs::create_directories(image_path + "/etc");
    fs::copy_file("/etc/resolv.conf", image_path + "/etc/resolv.conf", fs::copy_options::overwrite_existing);

    ContainerConfig config = {image_path, cmd};
    char* stack=new char[STACK_SIZE];

    pid_t pid=clone(container_main,
                    stack+STACK_SIZE,
                    CLONE_NEWPID | CLONE_NEWUTS | CLONE_NEWNS | CLONE_NEWNET  | SIGCHLD,
                   &config);

    set_container_limits(name, pid, mem, cpu);
    
    fs::create_directories(run_path);
    ofstream(run_path + "/" + name) << pid;


    cout << "Kubix: Container '" << name << "' started (PID: " << pid << ")\n";

    waitpid(pid, NULL, 0);

    fs::remove(run_path + "/" + name);
    delete[] stack;
    cout << "Kubix: Container '" << name << "' stopped.\n";

}

void container_list(){
  string run_path =string(KUBIX_BASE)+"/run";

  if(!fs::exists(run_path)){
    cout<<"No cantainers found.\n";
    return;
  }

  cout<<"Name\t\tPID\t\tSTATUS\n";
  cout<<"-----------------------------------------\n";
  for(const auto& entry : fs::directory_iterator(run_path)){
      string name=entry.path().filename().string();

      ifstream f(entry.path());
      pid_t pid;

      if(!(f>>pid)){
        cout<<name<<"\t\tInvalid\t\tErorr\n";
        continue;

      }
      string proc_path="/proc/"+to_string(pid);

      if(fs::exists(proc_path)){
        cout<<name<<"\t\t"<<pid<<"\t\tRunning\n";
      }
      else{
        cout<<name<<"\t\t"<<pid<<"\t\tStopped\n";
      }
      
      
  }
}


  
 void stop_container(string name){
    string run_path=string(KUBIX_BASE)+"/run/"+name;
    ifstream f(run_path);

    if (!f) {
        cerr << "\033[31mKubix Error:"<<name<<"is not rinnning.\033[0m\n";
        return;
    }

    pid_t pid=-1;
   if(!(f >> pid)){
    cout<<"Invalid PID file\n";
    return;
   }

   if(!fs::exists("/proc/"+to_string(pid))){
     cout<<"process already stopped\n";
     remove(run_path.c_str());
     return;
     
   }

    if (kill(pid, SIGTERM) == 0) {
          cout << "Container stopped gracefully\n";
          
    }

    for(int i = 0; i < 6; i++) {
        usleep(500000); 

}
    if(filesystem::exists("/proc/" + to_string(pid))){
      kill(pid,SIGKILL);    
      remove(run_path.c_str());
      return;
      
  }
   

     else {
        perror("Failed to stop container");
    }
   }


  
 

