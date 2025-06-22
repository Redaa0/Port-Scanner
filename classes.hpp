#pragma once

#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <vector>
#include <thread>
#include <vector>
#include <mutex>
#include <queue>
#include <poll.h>
#include <sys/epoll.h>
#include <unordered_map>
#include <atomic>
#include <bits/stdc++.h>

class Config
{
protected:
	std::string			m_ip;
	std::string			m_stringPort;
	long 				m_port;
	std::vector<long>	m_ports;
	bool verbose = false;
	enum class ScanType
	{
		FULL,
		SINGLE,
		RANGE,
		COMMON,
	};
	ScanType scan_type;
	int parse_input(int ac, char **av);


public:
	Config(int ac, char **av);
	Config();
};

class Scanner : public Config
{
private:
	std::queue<uint16_t> port_queue;
	std::mutex queue_mutex;
	std::vector<uint16_t> open_ports;
	std::mutex results_mutex;
	std::mutex write_mutex;
    std::unordered_map<int, uint16_t> sock_to_port;
    std::mutex epoll_mutex;
	std::atomic<bool> host_responded_{false};
    std::atomic<bool> host_unreachable_{false};
	void process_epoll_events(int epoll_fd);
	void exec_thread();
	void exec_udp();
	void scan_port(std::vector<uint16_t> ports);
	void write_string(std::string str, ScanType type, int color);
	bool is_ipv6(const std::string& ip);

public:
	Scanner(int ac, char **av);
	void format(long port, int status);
	void scan();
	void display_results();
};
