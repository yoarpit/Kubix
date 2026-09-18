<div align="center">

🐧 KUBIX

Lightweight Linux Container Runtime in C++

<p>
  <b>A learning-focused container runtime built using Linux primitives.</b>
</p>

<p>
  <img src="https://img.shields.io/badge/C++-17%2B-blue?style=for-the-badge&logo=c%2B%2B" alt="C++">
  <img src="https://img.shields.io/badge/Linux-Ubuntu-black?style=for-the-badge&logo=linux" alt="Linux">
  <img src="https://img.shields.io/badge/License-Apache%202.0-orange?style=for-the-badge" alt="License">
</p>

</div>

📖 Introduction

Kubix is a lightweight container runtime built in C++ that provides isolation using Linux primitives such as namespaces, cgroups, and chroot.

It is designed as a learning-focused alternative to Docker, giving developers more control and a deeper understanding of container internals.

✨ Features

🔒 Process isolation using Linux namespaces

📁 Filesystem isolation using chroot

⚙️ Resource control using cgroups

🧠 CPU and memory limits

▶️ Basic container lifecycle management

🛑 Start and stop containers

📋 CLI-based interaction

⚡ Lightweight and fast execution

🏗️ Architecture

Kubix is divided into four main modules:

                    ┌───────────────────┐
                    │     CLI Layer     │
                    │ run / stop / list │
                    │       / pull      │
                    └─────────┬─────────┘
                              │
                              ▼
                    ┌───────────────────┐
                    │    Core Engine    │
                    │                   │
                    │ Create containers │
                    │ Manage namespaces │
                    │ Execute processes │
                    └─────────┬─────────┘
                              │
              ┌───────────────┴───────────────┐
              │                               │
              ▼                               ▼
    ┌───────────────────┐          ┌───────────────────┐
    │ Resource Manager  │          │      Storage      │
    │                   │          │                   │
    │ CPU limits        │          │ Root filesystem   │
    │ Memory limits     │          │ Mount directories │
    │ cgroups           │          │ rootfs            │
    └───────────────────┘          └───────────────────┘

CLI Layer

Handles user commands such as:

run
stop
list
pull

Core Engine

Responsible for:

Creating containers

Managing namespaces

Handling process execution

Resource Manager

Manages system resources using cgroups:

CPU limits

Memory limits

Storage

Handles filesystem setup:

Root filesystem (rootfs)

Mounting required directories

🛠️ Requirements

Kubix currently requires:

Linux system

Ubuntu recommended

g++ compiler

Root privileges

📥 Installation

Clone the repository:

git clone https://github.com/yoarpit/kubix.git
cd kubix

Build Kubix using:

g++ cli/main.cpp core/engine.cpp core/resource.cpp utils/storage.cpp -o kubix -Wall

After successful compilation, the kubix executable will be created.

🚀 Usage

▶️ Run a Container

To start a new isolated session:

sudo ./kubix run <image_name> <container_name> <mem_limit> <cpu_limit>

Example:

sudo ./kubix run kali myxp --mem 512M --cpu 500

🛑 Stop a Container

To terminate a running container and release its resources:

sudo ./kubix stop <container_name>

Example:

sudo ./kubix stop myxp

📋 List Containers

To list containers:

sudo ./kubix list

📦 Pull an OS Image

To pull an operating-system image:

sudo ./kubix pull <image_name>

Example:

sudo ./kubix pull ubuntu

🔐 Container Isolation

Kubix uses Linux primitives to provide container isolation.

                    KUBIX CONTAINER
                           │
          ┌────────────────┼────────────────┐
          │                │                │
          ▼                ▼                ▼
     Namespaces          cgroups           chroot
          │                │                │
          ▼                ▼                ▼
     Process            CPU/Memory       Filesystem
     Isolation           Control          Isolation

This demonstrates the fundamental mechanisms used by modern container technologies.

🗺️ Future Improvements

The project roadmap includes:

🌐 Networking using veth pairs and bridges

📦 Improved image management system

💾 Volume mounting

🔐 Security enhancements using seccomp and Linux capabilities

🖥️ Electron-based GUI

🎯 Project Goal

Kubix is primarily designed to help developers understand how container technologies work internally.

Instead of hiding container internals behind a high-level interface, Kubix exposes the important concepts involved in building an isolated Linux environment.

Linux Kernel
     │
     ├── Namespaces
     ├── cgroups
     ├── chroot
     └── Filesystem
            │
            ▼
       ┌──────────┐
       │  Kubix   │
       └────┬─────┘
            │
            ▼
        Container

⚠️ Disclaimer

Kubix is a learning-focused project and is intended to demonstrate container concepts and Linux internals.

Some operations require root privileges and can affect the host system. Use Kubix carefully and preferably in a dedicated Linux development environment.

📜 License

This project is licensed under the Apache License 2.0.

See the LICENSE file for details.

<div align="center">

🐧 Kubix

Understanding containers from the Linux primitives up.

</div>
