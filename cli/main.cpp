#include <cstdlib>
#include <iostream>
#include "../includes/engine.hpp"

using namespace std;

int main(int argc, char const *argv[]){
  if(argc < 2){
    cout<<"\033[31m";
    cout << "Kubix CLI\nUsage: kubix run <img_name> <cont_name> --mem 512M --cpu 50000"<<"\033[0m\n";
    exit(1);
   }
   string cmd = argv[1];

   if(cmd == "run"){
     string image=argv[2];
     string name = argv[3];
     string mem = "";
     string cpu ="";

     for(int i=0;i<=4;i++){
       string arg = argv[i];
       if (arg == "--mem" && i + 1 < argc) mem = argv[++i];
       if (arg == "--cpu" && i + 1 < argc) cpu = argv[++i];
     }

     char sh[]="/bin/bash";

     run_container(image,name,sh,mem,cpu);
     
     } 

   if(cmd=="pull"){
     if(argc<3){
       cout<<"Usage: kubix pull <distro_name>";
       return 1;
       
     }
     string distro =argv[2];

     pull_image(distro);
     
   }

   if(cmd == "list"){
     if(argc<2){
       cout<<"Usage: kubix list";
     }

     container_list();
   }


  
    if(cmd=="stop"){
      if(argc<3){
        cout<<"\033[31m Usage: kubix stop <container name>\033[31m\n ";
      }

      string name=argv[2];
     stop_container(name);
   }
   
   return 0;    
}
