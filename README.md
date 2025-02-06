# C++ Port Scanner

## Introduction

Welcome to my **C++ Port Scanner**! This project is a personal exploration aimed at improving my skills in **C++ programming**, **networking**, and **cybersecurity**. The primary focus is on implementing an efficient and scalable **port scanning tool** that leverages both **multithreading** and **asynchronous I/O** to handle multiple ports concurrently without blocking, making it both fast and scalable.

Through this project, I aim to better understand how networking is done in C++ and how to efficiently scan open ports on a target system or network. At the same time, I’m applying and expanding my knowledge of cybersecurity concepts.

## Features

- **Multithreading Support**: The port scanner uses multiple threads to perform scans concurrently, significantly improving performance and reducing the total scan time.
- **Asynchronous I/O**: The program is optimized for asynchronous I/O operations using **epoll**, which allows handling a large number of connections without blocking threads.
- **Scan Types**:
  - **Full Scan** (scan all ports from 1 to 65535)
  - **Single Port Scan** (scan a specific port)
  - **Range Scan** (scan a range of ports)
  - **Common Port Scan** (scan a predefined list of common ports)
- **Real-Time Results**: Choose to see results as the scan progresses or wait until the scan completes for a final report.
- **Supports Both IPv4 and IPv6**: The scanner can handle both IPv4 and IPv6 addresses, making it more versatile for different networks.

## How It Works

The program prompts you to select a scanning mode (full, single, range, or common port scan) and enter the target's IP address or hostname. It then proceeds to scan the chosen ports for open connections.

- The **multithreading** approach utilizes all available CPU cores to parallelize the scanning process, making it more efficient and faster.
- **Asynchronous I/O (aio)** is used to handle socket connections concurrently, with the **epoll** system call managing multiple connections without blocking threads.
- The program reports the results in real-time, showing open ports as they are discovered or waiting until all ports have been scanned.

### Key Features in Code:
- **Multithreading**: The program uses multiple threads to divide the work, allowing the scanning process to be completed much faster, especially on large target systems.
- **Asynchronous I/O**: Using epoll, the program can scan ports asynchronously, handling large batches of ports without blocking any thread. This helps in managing numerous simultaneous socket connections.
- **Port Scanning**: The program can scan for open ports in different modes. You can scan a range of ports or a list of common ports, or perform a full scan.
- **Error Handling**: The program detects errors such as connection timeouts, unreachable hosts, or closed ports and handles them accordingly.

## Technologies Used

- **C++**: The core of the program is written in C++, leveraging modern features and best practices.
- **Multithreading**: The program uses standard multithreading with `std::thread` to handle different scanning tasks concurrently.
- **Asynchronous I/O (aio)**: Uses **epoll** for non-blocking socket operations to manage multiple concurrent connections.
- **Networking**: The program uses `getaddrinfo` and `inet_pton` to support both IPv4 and IPv6 addresses, and the `connect` system call for checking open ports.
- **Error Handling**: Handles network-related errors like `ECONNREFUSED`, `ETIMEDOUT`, and unreachable hosts gracefully.

## Getting Started

To get started with the port scanner, clone the repository and build the project:

```bash
git clone https://github.com/yourusername/port-scanner.git
cd port-scanner
make
