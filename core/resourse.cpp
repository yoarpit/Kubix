#include <fstream>
#include<iostream>
#include"../includes/engine.hpp"
#include<filesystem>
#include<fstab.h>
#include <system_error>

using namespace std;

namespace fs = std::filesystem;

void set_container_limits(string name, int pid, string mem_limit, string cpu_limit){
  
 string cgroups_base = "/sys/fs/cgroup/";
 string path = cgroups_base + "kubix_" + name;

try {
        // 1. Create the cgroup directory if it doesn't exist
     if (!fs::exists(path)) {
     fs::create_directories(path);

        


ofstream control("/sys/fs/cgroup/cgroup.subtree_control");
        control << "+memory +cpu +pids";
        control.close();


 if(!mem_limit.empty()&& mem_limit !="0M"){
   ofstream mem_file (path+"/memory.max");
   if(mem_file.is_open()){
     mem_file<<mem_limit;
   }}
 
   else if(mem_limit=="0M"){
     ofstream(path+"/memory.max")<<"4M";
   }
  
}

 if(!cpu_limit.empty()){
   ofstream(path+"/cpu.max")<<"50000";
  
}

 ofstream(path+"/pids.max")<<500;

 ofstream procs(path+"/cgroup.procs");
 if(procs.is_open()){
   procs<<pid;
 }

cout<<"\033[31m Kubix Resource:"<<name<<"Resricted TO"<<(mem_limit.empty()?"Unlimited":mem_limit)<<"RAM"<<"\033[0m\n";

}
catch (const exception& e) {
        cerr << "Kubix Isolation Error: " << e.what() << endl;
    
  
}

}
