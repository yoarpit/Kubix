#include <fstream>
#include<iostream>
#include"../includes/engine.hpp"
#include<filesystem>
#include<fstab.h>

using namespace std;

namespace fs = std::filesystem;

void set_container_limits(string name, int pid, string mem_limit, string cpu_limit){
  
string cgroups_base="/sys/fs/cgroup/kubix";
string path = cgroups_base+name;

if(!mem_limit.empty()){
  ofstream(path+"/memory.max")<<mem_limit;
  
}

if(!cpu_limit.empty()){
  ofstream(path+"/cpu.max")<<cpu_limit;
  
}

ofstream(path+"/pids.max")<<500;

ofstream(path+"/cgroup.procs")<<pid;

cout<<"\033[31m Kubix Resource:"<<name<<"Resricted TO"<<(mem_limit.empty()?"Unlimited":mem_limit)<<"RAM"<<"\033[0m\n";

  
}

