#include<iostream>
#include"../includes/engine.hpp"
#include<filesystem>
#include<map>
#include <string>

using namespace std;
namespace fs =std::filesystem;

#define KUBIX_IMG_PATH "/var/lib/kubix/images/"

void pull_image(string distro){
  string target_path = string(KUBIX_IMG_PATH) + distro;

  if(fs::exists(target_path)){
    cout<<"Kubix Image:"<<distro<<"already exist\n";
    return;
  }

  map<string, string> urls = {
        {"ubuntu", "https://partner-images.canonical.com/core/focal/current/ubuntu-focal-core-cloudimg-amd64-root.tar.gz"},
        {"fedora", "https://download.fedoraproject.org/pub/fedora/linux/releases/39/Container/x86_64/images/Fedora-Container-Base-39-1.5.x86_64.tar.xz"},
        {"arch",   "http://os.archlinuxarm.org/os/ArchLinuxARM-x86_64-latest.tar.gz"},
        {"kali",   "https://kali.download/nethunter-images/current/rootfs/kali-nethunter-rootfs-minimal-amd64.tar.xz"},
        {"rocky",  "https://dl.rockylinux.org/pub/rocky/9/images/x86_64/Rocky-9-Container-Base.tar.xz"}
    };

  if(urls.find(distro)==urls.end()){
    cout << "Kubix Error: '" << distro << "' not supported. Use: ubuntu, fedora, arch, kali, rocky.\n";
    return;
  }

  fs::create_directories(target_path);
  string url=urls[distro];

  cout<<"Kubix pulling "<<distro<<"...\n";

  string extract_cmd;
  if (url.find(".tar.gz") != string::npos) {
    extract_cmd = "curl -sL " + url + " | tar -xzf - --strip-components=1 -C " + target_path;
}
else if (url.find(".tar.xz") != string::npos) {
    extract_cmd = "curl -sL " + url + " | tar -xJf - --strip-components=1 -C " + target_path;
}
else {
    extract_cmd = "curl -sL " + url + " | tar -xf - --strip-components=1 -C " + target_path;
}

    int result = system(extract_cmd.c_str());

    if (result == 0) {
        // Fix for Fedora/Rocky which sometimes wrap the rootfs in a sub-tar
        if (fs::exists(target_path + "/layer.tar")) {
             system(("tar -xf " + target_path + "/layer.tar -C " + target_path).c_str());
        }
        cout << "Kubix: " << distro << " is ready for the team.\n";
    } else {
        cout << "Kubix Error: Download failed.\n";
        fs::remove_all(target_path);
    }

    

    

  
}
