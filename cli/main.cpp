#include <iostream>
#include "../includes/engine.hpp"

using namespace std;

int main(int argc, char const *argv[]){
  if(argc>2){
    cout<<"Usage:"<<endl;

   }
   string cmd = argv[1];

   if(cmd == "run"){
     if(argc < 3){
       run_container("/bin/bash");
     }
     else{
       run_container(argv[2]);
     }
 }
   else if(cmd=="stop"){
     stop_container();
   }
   else{
     cout<<"\033[31mUnkown command\033[0m\n";
   } 
   return 0;    
}
