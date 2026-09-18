# <div align="center">KUBIX</div>

<div align="center">

### 🐧 Lightweight Linux Container Runtime written in C++

<p>
  <b>Build containers from Linux kernel primitives.</b>
</p>

<p>
  <a href="#features">Features</a> •
  <a href="#architecture">Architecture</a> •
  <a href="#installation">Installation</a> •
  <a href="#usage">Usage</a> •
  <a href="#roadmap">Roadmap</a>
</p>

<br>

<img src="https://img.shields.io/badge/C++-17%2B-blue?style=for-the-badge&logo=c%2B%2B" alt="C++">
<img src="https://img.shields.io/badge/Linux-Kernel-black?style=for-the-badge&logo=linux" alt="Linux">
<img src="https://img.shields.io/badge/CMake-Build-green?style=for-the-badge&logo=cmake" alt="CMake">
<img src="https://img.shields.io/badge/License-Apache%202.0-orange?style=for-the-badge" alt="License">

</div>

---

## 📖 About

**Kubix** is a lightweight container runtime written in **C++** for Linux.

Unlike high-level container tools, Kubix focuses on the **low-level Linux mechanisms** that make containers possible.

It uses Linux kernel features such as:

* Linux Namespaces
* cgroups v2
* `clone()`
* `chroot()`
* Mount namespaces
* Filesystem isolation
* Process isolation

The main goal of Kubix is to understand and implement the fundamental components of a container runtime from the ground up.

---

## ✨ Features

<table>
<tr>
<td width="50%">

### 🔒 Isolation

* PID Namespace
* UTS Namespace
* Mount Namespace
* Filesystem Isolation
* Process Isolation

</td>

<td width="50%">

### ⚙️ Resource Management

* cgroups v2
* CPU limits
* Memory limits
* PID limits
* Process management

</td>
</tr>

<tr>
<td>

### 📁 Filesystem

* `chroot`
* `/proc`
* `/tmp`
* `/dev`
* Container root filesystem
* `/var/lib/kubix`

</td>

<td>

### 🌐 Networking

* Network Namespace
* veth pairs
* Linux bridge
* Container networking
* NAT

</td>
</tr>
</table>

---

# 🏗️ Architecture

```text
                         ┌─────────────────────┐
                         │      KUBIX CLI      │
                         │                     │
                         │  kubix run          │
                         │  kubix pull         │
                         │  kubix ps           │
                         └──────────┬──────────┘
                                    │
                                    ▼
                         ┌─────────────────────┐
                         │    Kubix Engine     │
                         └──────────┬──────────┘
                                    │
              ┌─────────────────────┼─────────────────────┐
              │                     │                     │
              ▼                     ▼                     ▼
       ┌──────────────┐      ┌──────────────┐      ┌──────────────┐
       │  Namespaces  │      │   Cgroups    │      │  Filesystem  │
       └──────┬───────┘      └──────┬───────┘      └──────┬───────┘
              │                     │                     │
              ▼                     ▼                     ▼
        PID / UTS / NS         CPU / Memory / PID       chroot
              │                     │                     │
              └─────────────────────┼─────────────────────┘
                                    │
                                    ▼
                         ┌─────────────────────┐
                         │   Isolated Process  │
                         │     Container       │
                         └─────────────────────┘
```

---

# 🧠 How Kubix Works

## 1️⃣ Create a Container Process

Kubix uses Linux's `clone()` system call to create an isolated process.

```cpp
clone(
    child_function,
    stack,
    CLONE_NEWPID |
    CLONE_NEWUTS |
    CLONE_NEWNS |
    SIGCHLD,
    args
);
```

This allows Kubix to create new Linux namespaces for the container.

---

## 2️⃣ PID Namespace

The PID namespace isolates the container's process tree.

### Host

```text
PID 1
PID 1024
PID 2034
PID 4050
```

### Container

```text
PID 1
PID 2
PID 3
```

The container can have its own PID `1`.

---

## 3️⃣ UTS Namespace

UTS namespaces allow Kubix to provide an isolated hostname.

```bash
hostname kubix
```

Inside the container:

```bash
$ hostname
kubix
```

The host's hostname remains unchanged.

---

## 4️⃣ Mount Namespace

Kubix creates an isolated mount environment.

```text
Host Mount Namespace
          │
          │
          ▼
    Kubix Container
          │
          ├── /proc
          ├── /tmp
          ├── /dev
          └── root filesystem
```

---

## 5️⃣ Root Filesystem

Kubix uses a dedicated root filesystem.

```text
/var/lib/kubix/
│
├── images/
│
├── containers/
│
└── runtime/
```

A container may have:

```text
container-rootfs/
│
├── bin/
├── etc/
├── lib/
├── usr/
├── proc/
├── tmp/
└── dev/
```

The container process is then moved into the new filesystem environment.

---

# ⚙️ Resource Management

Kubix uses **cgroups v2** for resource management.

```text
                 KUBIX CONTAINER
                        │
              ┌─────────┼─────────┐
              │         │         │
              ▼         ▼         ▼
             CPU      Memory     PIDs
              │         │         │
              ▼         ▼         ▼
            Limit     Limit     Limit
```

Example:

```text
CPU      → restricted
Memory   → restricted
Processes → restricted
```

This prevents a container from consuming unlimited resources.

---

# 📁 Project Structure

```text
kubix/
│
├── cli/
│   └── main.cpp
│
├── core/
│   ├── engine.cpp
│   ├── namespace.cpp
│   ├── filesystem.cpp
│   ├── cgroup.cpp
│   ├── network.cpp
│   └── image.cpp
│
├── include/
│   ├── engine.hpp
│   ├── namespace.hpp
│   ├── filesystem.hpp
│   ├── cgroup.hpp
│   ├── network.hpp
│   └── image.hpp
│
├── utils/
│   └── ...
│
├── docs/
│   └── ...
│
├── CMakeLists.txt
├── LICENSE
└── README.md
```

---

# 🛠️ Technology Stack

<div align="center">

<table>
<tr>
<th>Technology</th>
<th>Purpose</th>
</tr>

<tr>
<td><b>C++</b></td>
<td>Container runtime</td>
</tr>

<tr>
<td><b>Linux</b></td>
<td>Operating system</td>
</tr>

<tr>
<td><b>Namespaces</b></td>
<td>Isolation</td>
</tr>

<tr>
<td><b>cgroups v2</b></td>
<td>Resource management</td>
</tr>

<tr>
<td><b>chroot</b></td>
<td>Filesystem isolation</td>
</tr>

<tr>
<td><b>OverlayFS</b></td>
<td>Layered filesystem</td>
</tr>

<tr>
<td><b>veth</b></td>
<td>Container networking</td>
</tr>

<tr>
<td><b>CMake</b></td>
<td>Build system</td>
</tr>

</table>

</div>

---

# 💻 Requirements

Kubix currently targets **Linux**.

### Requirements

* Linux kernel
* C++17 or newer
* GCC / Clang
* CMake
* Linux namespaces
* cgroups v2
* Root privileges for some operations

### Windows

If you are using Windows, you can run Kubix using:

```text
Windows
   │
   ▼
WSL2
   │
   ▼
Ubuntu / Linux
   │
   ▼
Kubix
```

For advanced networking and kernel-level testing, a native Linux system or VM may be preferable.

---

# 📥 Installation

## Clone Repository

```bash
git clone https://github.com/yoarpit/kubix.git
cd kubix
```

## Build

```bash
mkdir build
cd build
```

```bash
cmake ..
```

```bash
make -j$(nproc)
```

---

# ▶️ Usage

After building Kubix:

```bash
kubix run <rootfs>
```

Example:

```bash
kubix run ./rootfs
```

Pull an image:

```bash
kubix pull kali
```

Run it:

```bash
kubix run kali
```

Inside the container:

```bash
$ hostname
kubix
```

```bash
$ ps
```

> ⚠️ CLI commands may change during development.

---

# 📦 Container Storage

Kubix uses:

```text
/var/lib/kubix
```

Example:

```text
/var/lib/kubix/
│
├── images/
│   ├── kali/
│   └── ubuntu/
│
├── containers/
│   ├── container-1/
│   └── container-2/
│
└── runtime/
```

---

# 🌐 Networking

Kubix is designed to support Linux network namespaces and virtual Ethernet interfaces.

```text
                         HOST
                           │
                           │
                      ┌────▼────┐
                      │ Bridge  │
                      └────┬────┘
                           │
                    ┌──────┴──────┐
                    │     veth    │
                    └──────┬──────┘
                           │
                    ┌──────▼──────┐
                    │  Container  │
                    │  Namespace  │
                    └─────────────┘
```

Planned networking functionality:

* Network namespaces
* veth pairs
* Linux bridges
* Container IP allocation
* NAT
* Container-to-container communication

---

# 📂 OverlayFS

Kubix plans to use **OverlayFS** for layered container filesystems.

```text
             Container
                 │
                 ▼
        ┌─────────────────┐
        │    OverlayFS    │
        ├─────────────────┤
        │   Upper Layer   │
        │  Container Data │
        ├─────────────────┤
        │   Lower Layer   │
        │      Image      │
        └─────────────────┘
```

This allows multiple containers to share a common image while keeping their changes isolated.

---

# 🔐 Security

Container isolation is built using multiple Linux primitives.

```text
              KUBIX
                │
     ┌──────────┼──────────┐
     │          │          │
     ▼          ▼          ▼
Namespaces   Cgroups   Filesystem
     │          │          │
     ▼          ▼          ▼
 Isolation   Limits     Isolation
```

Future security features:

* User namespaces
* Linux capabilities
* Seccomp
* Read-only root filesystem
* Rootless containers
* Security profiles

---

# 🗺️ Roadmap

## Runtime

* [x] C++ runtime foundation
* [x] PID namespace
* [x] UTS namespace
* [x] Mount namespace
* [x] Basic filesystem isolation
* [x] cgroups foundation
* [ ] Container lifecycle management
* [ ] Signal forwarding
* [ ] Improved error handling

## Filesystem

* [ ] OverlayFS
* [ ] Image layers
* [ ] Copy-on-write
* [ ] Container snapshots

## Networking

* [ ] Network namespace
* [ ] veth pairs
* [ ] Linux bridge
* [ ] Container IP allocation
* [ ] NAT
* [ ] Container-to-container networking

## Security

* [ ] User namespaces
* [ ] Linux capabilities
* [ ] Seccomp
* [ ] Rootless containers
* [ ] Read-only containers

## CLI

* [ ] `kubix run`
* [ ] `kubix ps`
* [ ] `kubix exec`
* [ ] `kubix stop`
* [ ] `kubix rm`
* [ ] `kubix images`
* [ ] `kubix pull`
* [ ] `kubix inspect`

## Future

* [ ] Container daemon
* [ ] REST / gRPC API
* [ ] Remote image registry
* [ ] Multi-user support
* [ ] Electron GUI

---

# 🔍 Kubix vs Traditional Container Tools

Kubix focuses on **learning and implementing container internals**.

```text
        High Level
            │
            ▼
      ┌─────────────┐
      │    Docker   │
      └──────┬──────┘
             │
             ▼
      ┌─────────────┐
      │  containerd │
      └──────┬──────┘
             │
             ▼
      ┌─────────────┐
      │   Runtime   │
      └──────┬──────┘
             │
             ▼
      Linux Kernel
```

Kubix explores the lower layers directly:

```text
             Kubix
               │
               ▼
       ┌───────────────┐
       │   clone()     │
       │   namespaces  │
       │   cgroups     │
       │   mount()     │
       │   chroot()    │
       │   networking  │
       └───────┬───────┘
               │
               ▼
          Linux Kernel
```

---

# 🎯 Goals

Kubix aims to provide a practical implementation of the concepts behind Linux containers.

### Main goals

* Understand Linux container internals
* Implement process isolation
* Implement resource management
* Build isolated filesystems
* Implement container networking
* Learn Linux kernel interfaces
* Build a lightweight runtime in C++

---

# ⚠️ Project Status

> **Kubix is currently an experimental and educational project.**

It is **not intended to replace production container runtimes** such as Docker, containerd, or CRI-O.

Container-runtime development involves privileged kernel operations. Incorrect configuration can affect the host system.

For development and testing, use a dedicated Linux environment or VM.
