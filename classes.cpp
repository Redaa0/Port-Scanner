#include "classes.hpp"

Config::Config()
{}

Config::Config(int ac, char **av)
{
	std::string choice;
	int choice1;

	if (ac != 1)
		throw std::string("Invalid number of arguments");
	std::cout << "\nEnter the IP address or the Hostname: ";
	std::getline(std::cin, m_ip);
	while (m_ip.empty())
	{
		std::cout << "IP address or Hostname cannot be empty. Please enter again: ";
		std::getline(std::cin, m_ip);
	}
	while (true)
	{
		std::cout << "\nOPTIONS:\n\n[0] FULL SCAN\n[1] SINGLE PORT SCAN\n"
					"[2] RANGE SCAN\n[3] COMMON PORT SCAN\n[4] EXIT\n" << std::endl;
		std::cout << "YOU CHOOSE ->>>> ";
		std::getline(std::cin, choice);
		while (choice.empty())
		{
			std::cout << "Your choice cannot be empty, Please enter again: ";
			std::getline(std::cin, choice);
		}
		try
		{
			if (choice.length() > 1)
				throw std::invalid_argument("Invalid input. Please"
											"enter a valid number between 0 and 4.");
			choice1 = std::stoi(choice);
			if (choice1 < 0 || choice1 > 4)
				throw std::invalid_argument("Invalid input. Please enter a"
											"valid number between 0 and 4.");
			if (choice1 == 4)
				exit(0);
			break ;
		}
		catch (const std::invalid_argument& e)
		{
			std::cout << "Invalid input. Please enter a valid number between 0 and 4." << std::endl;
			continue;
		}
	}
	if (choice1 == 0)
	{
		for (int i = 0; i <= 65535; i++)
			m_ports.push_back(i);
		scan_type = ScanType::FULL;
	}
	else if (choice1 == 1)
	{
		std::string port;
		std::cout << "\nEnter the port number: ";
		std::getline(std::cin, port);
		if ((all_of(port.begin(), port.end(), ::isdigit) == false))
		{
			std::cout << "The port need to be all digits. Please enter again: ";
			std::getline(std::cin, port);
		}
		while (port.empty())
		{
			std::cout << "Port number cannot be empty. Please enter again: ";
			std::getline(std::cin, port);
		}
		int port2 = std::stoi(port);
		if (port2 < 0 || port2 > 65535)
		{
			std::cout << "Port number must be between 0 and 65535. Please enter again: ";
			std::getline(std::cin, port);
		}
		m_ports.push_back(port2);
		scan_type = ScanType::SINGLE;
	}
	else if (choice1 == 2)
	{
		std::string port;
		std::cout << "\nEnter the range of ports (e.g. 1-1000): ";
		std::getline(std::cin, port);
		while (port.empty())
		{
			std::cout << "Port range cannot be empty. Please enter again: ";
			std::getline(std::cin, port);
		}
		for (int i = std::stoi(port.substr(0, port.find('-')));
				i <= std::stoi(port.substr(port.find('-') + 1)); i++)
		{
			std::string beg, end;
			if ((all_of(beg.begin(), end.end(), ::isdigit) || all_of(end.begin(), end.end(), ::isdigit)) == false)
			{
				std::cout << "The range have to be all digits. Please enter again: ";
				std::getline(std::cin, port);
			}
			m_ports.push_back(i);
		}
		scan_type = ScanType::RANGE;
	}
	else if (choice1 == 3)
	{
		std::vector<int> common_ports = {
										21, 22, 23, 25, 53, 80, 110, 111, 135, 139, 143,
										443, 445, 993, 995, 1723, 3306, 3389,5900, 8080
										};
		for (auto port : common_ports)
			m_ports.push_back(port);
		scan_type = ScanType::COMMON;
	}
	else
		exit(0);
	std::cout << "\nDo you wanna see results in real-time? (y/n): ";
	std::getline(std::cin, choice);
	while (choice.empty())
	{
		std::cout << "Your choice cannot be empty, Please enter again: ";
		std::getline(std::cin, choice);
	}
	while (1)
	{
		if (choice == "y")
		{
			std::cout << "Results will be displayed in real-time" << std::endl;
			verbose = true;
			break;
		}
		else if (choice == "n")
		{
			std::cout << "Results will be displayed at the end" << std::endl;
			break;
		}
		else
		{
			std::cout << "Invalid input. Please enter 'y' or 'n'." << std::endl;
			std::cout << "Do you wanna see results in real-time? (y/n): ";
			std::getline(std::cin, choice);
		}
	}
	struct sockaddr_in tmp4;
	struct sockaddr_in tmp6;
	if (inet_pton(AF_INET, m_ip.c_str(), &tmp4.sin_addr))
	{
		struct addrinfo hints = {}, *res;
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		if (getaddrinfo(m_ip.c_str(), nullptr, &hints, &res) != 0)
			throw std::string("\n-Invalid hostname or IP address");
		char ip_str[INET_ADDRSTRLEN];
		struct sockaddr_in *addr = (struct sockaddr_in *)res->ai_addr;
		inet_ntop(AF_INET, &addr->sin_addr, ip_str, sizeof(ip_str));
		m_ip = ip_str;
		freeaddrinfo(res);
	}
	else if (inet_pton(AF_INET6, m_ip.c_str(), &tmp6))
	{
		struct in6_addr tmp6;
		{
			struct addrinfo hints = {};
			struct addrinfo *res = nullptr;
			hints.ai_family = AF_INET6;
			hints.ai_socktype = SOCK_STREAM;

			if (getaddrinfo(m_ip.c_str(), nullptr, &hints, &res) != 0)
				throw std::string("\n-Invalid hostname or IP address");
			char ip_str[INET6_ADDRSTRLEN];
			struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)res->ai_addr;
			if (!inet_ntop(AF_INET6, &addr6->sin6_addr, ip_str, sizeof(ip_str)))
			{
				freeaddrinfo(res);
				throw std::runtime_error("inet_ntop failed");
			}
			m_ip = ip_str;
			freeaddrinfo(res);
		}
	}
	else
	{
		// Not a valid IP address, try to resolve as hostname
		struct addrinfo hints = {}, *res;
		hints.ai_family = AF_UNSPEC; // Allow both IPv4 and IPv6
		hints.ai_socktype = SOCK_STREAM;
		
		if (getaddrinfo(m_ip.c_str(), nullptr, &hints, &res) != 0)
			throw std::string("\n-Invalid hostname or IP address");
		
		// Prefer IPv4 if available, otherwise use IPv6
		struct addrinfo *current = res;
		bool resolved = false;
		
		while (current != nullptr && !resolved)
		{
			if (current->ai_family == AF_INET)
			{
				char ip_str[INET_ADDRSTRLEN];
				struct sockaddr_in *addr = (struct sockaddr_in *)current->ai_addr;
				if (inet_ntop(AF_INET, &addr->sin_addr, ip_str, sizeof(ip_str)))
				{
					m_ip = ip_str;
					std::cout << "Resolved to IPv4: " << m_ip << std::endl;
					resolved = true;
				}
			}
			current = current->ai_next;
		}
		
		if (!resolved)
		{
			// Try IPv6 if IPv4 resolution failed
			current = res;
			while (current != nullptr && !resolved)
			{
				if (current->ai_family == AF_INET6)
				{
					char ip_str[INET6_ADDRSTRLEN];
					struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)current->ai_addr;
					if (inet_ntop(AF_INET6, &addr6->sin6_addr, ip_str, sizeof(ip_str)))
					{
						m_ip = ip_str;
						std::cout << "Resolved to IPv6: " << m_ip << std::endl;
						resolved = true;
					}
				}
				current = current->ai_next;
			}
		}
		
		freeaddrinfo(res);
		
		if (!resolved)
			throw std::string("\n-Failed to resolve hostname to IP address");
	}
}
void Scanner::display_results()
{
	if (open_ports.empty())
	{
		write_string("No open ports found", scan_type, 31);
		return;
	}
	std::cout << "\nOpen ports: ";
	for (auto port : open_ports)
		std::cout << "\033[32m" << port << "\033[0m ";
	std::cout << std::endl;
}

Scanner::Scanner(int ac, char **av)
: Config(ac, av)
{
	for (auto port : m_ports)
		port_queue.push(port);
	const int num_threads = std::thread::hardware_concurrency();
	std::vector<std::thread> threads;
	for (int i = 0; i < num_threads; ++i)
		threads.emplace_back(&Scanner::exec_thread, this);
	for (auto& t : threads)
	{
		if (t.joinable())
			t.join();
	}
	display_results();
}

void Scanner::write_string(std::string str, ScanType type, int color)
{
	std::lock_guard<std::mutex> lock(write_mutex);
	if (verbose || type == ScanType::SINGLE)
		std::cout << "\033[" << color << "m" << str << "\033[0m" << std::endl;
}

void Scanner::process_epoll_events(int epoll_fd)
{
    const int MAX_EVENTS = 100;
    struct epoll_event events[MAX_EVENTS];
    int timeout_ms = 1500;

    while (true)
	{
        int num_ready = epoll_wait(epoll_fd, events, MAX_EVENTS, timeout_ms);
        if (num_ready > 0)
		{
			for (int i = 0; i < num_ready; ++i)
			{
				int socketd = events[i].data.fd;
				uint16_t port;
				{
					std::lock_guard<std::mutex> lock(epoll_mutex);
					auto it = sock_to_port.find(socketd);
					if (it == sock_to_port.end()) continue;
					port = it->second;
					sock_to_port.erase(it);
				}
				int connect_error;
				socklen_t len = sizeof(connect_error);
				if (getsockopt(socketd, SOL_SOCKET, SO_ERROR, &connect_error, &len) < 0 || connect_error != 0)
				{
					if (connect_error == ECONNREFUSED)
					{
						host_responded_.store(true);
						write_string("Port " + std::to_string(port) + " is closed", scan_type, 31);
					}
					else if (connect_error == ETIMEDOUT)
						write_string("Port " + std::to_string(port) + " is filtered", scan_type, 31);
					else if (connect_error == ENETUNREACH || connect_error == EHOSTUNREACH)
					{
						write_string("Host is unreachable", scan_type, 31);
						host_unreachable_.store(true);
					}
					close(socketd);
					continue;
				}
				host_responded_.store(true);
				char buffer[1];
				if (send(socketd, buffer, 0, 0) < 0)
				{
					close(socketd);
					continue;
				}
				std::lock_guard<std::mutex> lock(results_mutex);
				open_ports.push_back(port);
				write_string("Port " + std::to_string(port) + " is open", scan_type, 92);
				close(socketd);
			}
		}
		else
		{
            std::lock_guard<std::mutex> lock(epoll_mutex);
            for (const auto& pair : sock_to_port)
			{
                if (host_unreachable_.load())
					write_string("Port " + std::to_string(pair.second) + " is unreachable (host down)", scan_type, 31);
				else if (host_responded_.load())
					write_string("Port " + std::to_string(pair.second) + " is filtered (no response)", scan_type, 31);
				else
					write_string("Port " + std::to_string(pair.second) + " is unreachable (host down or no response)", scan_type, 31);
                close(pair.first);
            }
            sock_to_port.clear();
            break;
        }
    }
    host_responded_.store(false);
    host_unreachable_.store(false);
}

void Scanner::exec_thread()
{
    const int BATCH_SIZE = 150;
	const int MAX_CONCURRENT = 1000;
	int epoll_fd = epoll_create1(0);
	int socketd;
	struct sockaddr_in target_addr {};
	struct sockaddr_in6 target_addr2 {};
	int result;

	if (epoll_fd == -1)
	{
		perror("epoll_create1");
		return ;
	}
    while (true)
	{
        std::vector<uint16_t> ports;
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            for (int i = 0; i < BATCH_SIZE && !port_queue.empty(); ++i)
			{
                ports.push_back(port_queue.front());
                port_queue.pop();
        	}
            if (ports.empty()) return;int result = connect(socketd, (struct sockaddr*)&target_addr, sizeof(target_addr));
    	}
        for (uint16_t port : ports)
		{
            if (is_ipv6(m_ip))
			{
                target_addr2.sin6_family = AF_INET6;
                target_addr2.sin6_port = htons(port);
                if (inet_pton(AF_INET6, m_ip.c_str(), &target_addr2.sin6_addr) <= 0)
					continue;
				socketd = socket(AF_INET6, SOCK_STREAM | SOCK_NONBLOCK, 0);
				if (socketd < 0)
					continue;
				result = connect(socketd, (struct sockaddr*)&target_addr2, sizeof(target_addr));
			}
			else
			{
				target_addr.sin_family = AF_INET;
				target_addr.sin_port = htons(port);
				if (inet_pton(AF_INET, m_ip.c_str(), &target_addr.sin_addr) <= 0)
					continue;
				socketd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
 	        	if (socketd < 0)
					continue;
            	result = connect(socketd, (struct sockaddr*)&target_addr, sizeof(target_addr));
			}
            if (result == 0) 
			{
                std::lock_guard<std::mutex> lock(results_mutex);
				write_string("Port " + std::to_string(port) + " is open", scan_type, 92);
                open_ports.push_back(port);
                close(socketd);
                continue;
            }
			else if (errno != EINPROGRESS)
			{
                close(socketd);
                continue;
            }
            struct epoll_event event {};
            event.events = EPOLLOUT | EPOLLET;
            event.data.fd = socketd;
            {
                std::lock_guard<std::mutex> lock(epoll_mutex);
				if (sock_to_port.size() >= MAX_CONCURRENT) break;
                sock_to_port[socketd] = port;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socketd, &event) == -1)
				{
                    perror("epoll_ctl");
                    close(socketd);
                    sock_to_port.erase(socketd);
                }
            }
        }
        process_epoll_events(epoll_fd);
    }
	std::lock_guard<std::mutex> lock(epoll_mutex);
	for (auto& [fd, port] : sock_to_port) close(fd);
	sock_to_port.clear();
	close(epoll_fd);
}

bool Scanner::is_ipv6(const std::string& ip)
{
    struct sockaddr_in6 sa;
    return inet_pton(AF_INET6, ip.c_str(), &(sa.sin6_addr)) != 0;
}