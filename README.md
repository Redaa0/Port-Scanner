# C++ Port Scanner

## Introduction  
A hybrid port scanner combining **multithreading** and **asynchronous I/O** (via `epoll`) for efficient network scanning. Built to explore C++ networking, cybersecurity concepts, and high-performance system design.  

---

## Features  
- 🚀 **Hybrid Architecture**: Threads + asynchronous I/O for scalability.  
- 🔍 **Scan Modes**:  
  - `Full Scan` (1-65535 ports)  
  - `Single Port`  
  - `Range Scan` (e.g., 20-100)  
  - `Common Ports` (predefined list of 20+ ports)  
- 🌐 **IPv4/IPv6 Support**: Resolves hostnames and handles both address types.  
- 📊 **Real-Time Results**: Optional verbose mode for live updates.  
- 🛠️ **Error Classification**: Detects closed, filtered, and unreachable ports/hosts.  

---
## Prerequisites

Before you begin, ensure that you have the following installed on your system:

- **C++ Compiler**: Make sure you have a C++17 compatible compiler (e.g., `g++` or `clang++`).
- **Make**: The build system used to compile the project.
- **Linux**: The program uses `epoll` for asynchronous I/O, which is Linux-specific.

---

## Installation  

```bash
git clone https://github.com/yourusername/port-scanner.git
cd port-scanner
make
```
---

# Usage

Interactive Mode

Run the executable and follow prompts:

```
./port_scanner
```
1. Enter Target: IP/hostname (e.g., 192.168.1.1 or example.com).
2. Choose Scan Type:
       [0] Full Scan
       [1] Single Port
       [2]: Range Scan
       [3]: Common Ports

3. Real-Time Output: y for live results, n for summary.

Example

```
Enter the IP address or the Hostname: 127.0.0.1

OPTIONS:

[0] FULL SCAN
[1] SINGLE PORT SCAN
[2] RANGE SCAN
[3] COMMON PORT SCAN
[4] EXIT

YOU CHOOSE ->>>> 1

Enter the port number: 22

Do you wanna see results in real-time? (y/n): y
Results will be displayed in real-time
Port 22 is open

Open ports: 22
```
---

## Technical Design

Multithreading
  Dynamic Thread Pool: Threads = CPU cores (**std::thread::hardware_concurrency()**).
  Batch Processing: Each thread processes 300 ports per batch to minimize lock contention.

Asynchronous I/O Workflow
  1. Non-Blocking Sockets: Created with SOCK_NONBLOCK.
  2. epoll Monitoring: Tracks connection states (success/timeout/errors).
  3. Error Handling: Classifies errors like ECONNREFUSED (closed) or ETIMEDOUT (filtered).

---

## Contributing
1. Fork the repository.
2. Add features (e.g., UDP scanning, rate limiting).
3. Submit a Pull Request with clear descriptions.

---

## Troubleshooting
No Results:
      Verify target reachability with ping.
      Check firewall rules on target/host.

Build Errors: Ensure g++ and make are installed.
Permission Issues: Run with sudo if required.

Happy Scanning! 🔍
