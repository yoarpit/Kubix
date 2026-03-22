#pragma once
#include<string>
using namespace std;

void pull_image(string distro);

void run_container(string image,string name,char* cmd,string memory,string cpu);
void stop_container(string name);

void set_container_limits(string name,int pid,string mem_limit, string cpu_limit);



