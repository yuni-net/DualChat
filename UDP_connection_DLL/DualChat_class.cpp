#include <string>
#include <iostream>
#include "DualChat_class.h"
#include "BinaryData.h"
#include "zeromem.h"

#pragma warning(disable:4996)

class system_sign
{
public:
	static const char * join;
	static const char * gota_message;
	static const char * register_me;
};
const char * system_sign::join = "plz let me join DualChat";
const char * system_sign::gota_message = "you got a message";
const char * system_sign::register_me = "plz register me";

const unsigned short port_DualChat = 9696;

static const char system_head [] = {0, 0, 96};

void DualChatClass::join_guild(const char * user_name)
{
	this->user_name = user_name;
	broadcast_to_find();
	targets.clear();
}

void DualChatClass::broadcast_to_find()
{
	zeromem(broad_addr);
	broad_addr.sin_family = AF_INET;
	broad_addr.sin_addr.s_addr = inet_addr("255.255.255.255");
	broad_addr.sin_port = htons(port_DualChat);

	const int data_size = 256;
	BinaryData data(data_size);
	data.add(system_head, sizeof(system_head));
	data.add(system_sign::join);
	data.add(user_name.c_str());

	int result = sendto(
		com_sock,
		data.get_buffer(),
		data_size,
		0,
		(sockaddr *) &broad_addr,
		sizeof(sockaddr_in));

	// debug
	std::cout << "broadcasted" << std::endl;
	for (int i = 0; i < 48; ++i)
	{
		char code_str[256];
		itoa(data.get_buffer()[i], code_str, 10);
		std::cout << data.get_buffer()[i] << " (" << code_str << ")" << std::endl;
	}
}


void DualChatClass::convert_req_data(BinaryData & dif_request, const char * message)
{
	dif_request.add(system_head, sizeof(system_head));
	dif_request.add(system_sign::gota_message);
	dif_request.add(message);
}

void DualChatClass::send_message(const char * message)
{
	const int data_size = 1024;
	BinaryData req_data(data_size);
	convert_req_data(req_data, message);

	// debug
	std::cout << "sended message" << std::endl;
	for (int i = 0; i < 48; ++i)
	{
		char code_str[256];
		itoa(req_data.get_buffer()[i], code_str, 10);
		std::cout << req_data.get_buffer()[i] << " (" << code_str << ")" << std::endl;
	}

	for (unsigned int target_No = 0; target_No < targets.size(); ++target_No)
	{
		auto & target_addr = targets[target_No];
		sendto(
			com_sock,
			req_data.get_buffer(),
			data_size,
			0,
			reinterpret_cast<sockaddr *>(&target_addr),
			sizeof(sockaddr_in));
	}
}

int DualChatClass::receive_message(char * message)
{
	sockaddr_in cliant_addr;
	int addr_len = sizeof(sockaddr_in);

	fd_set fds, readfds;
	FD_ZERO(&readfds);
	FD_SET(com_sock, &readfds);

	timeval timev;
	timev.tv_sec = 1;
	timev.tv_usec = 0;

	while (true)
	{
		memcpy(&fds, &readfds, sizeof(fd_set));

		int result = select(0, &fds, NULL, NULL, &timev);
		if (result == 0)
		{
			break;
		}

		if (FD_ISSET(com_sock, &fds) == 0)
		{
			continue;
		}

		memset(message, 0, 1024);
		recvfrom(
			com_sock,
			message,
			1024,
			0,
			reinterpret_cast<sockaddr *>(&cliant_addr),
			&addr_len);

		std::cout << "You received a message:" << std::endl;
		for (int i = 0; i < 48; ++i)
		{
			char code_str[256];
			itoa(message[i], code_str, 10);
			std::cout << message[i] << " (" << code_str << ")" << std::endl;
		}

		if (is_the_address_mine(cliant_addr.sin_addr.S_un.S_addr))
		{
			// 自分自身からのメッセージは無視する
			std::cout << "It's from my self." << std::endl;
			continue;
		}

		if (process_message(message, cliant_addr))
		{
			return 1;
		}
	}

	return 0;
}

bool DualChatClass::is_the_address_mine(unsigned long address)
{
	for (unsigned int addr_No = 0; addr_No < myaddr_list.size(); ++addr_No)
	{
		const auto & myaddr = myaddr_list[addr_No];
		if (myaddr == address)
		{
			return true;
		}
	}

	return false;
}


DualChatClass::DualChatClass()
{
	com_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	int perm = 1;
	setsockopt(com_sock, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<const char *>(&perm), sizeof(int));

	recv_addr.sin_family = AF_INET;
	recv_addr.sin_port = htons(port_DualChat);
	recv_addr.sin_addr.S_un.S_addr = INADDR_ANY;
	int result = bind(com_sock, reinterpret_cast<sockaddr *>(&recv_addr), sizeof(recv_addr));
	if (result == -1)
	{
		std::cout << "failed to bind port" << std::endl;
	}

	get_myaddr(myaddr_list);
}

void DualChatClass::get_myaddr(std::vector<unsigned long> & myaddr_list)
{
	const int buffersize = 256;
	char buffer[buffersize];
	gethostname(buffer, buffersize);

	HOSTENT * host = gethostbyname(buffer);

	int addr_No = 0;
	while (true)
	{
		char * base_addr = host->h_addr_list[addr_No];
		if (base_addr == nullptr)
		{
			return;
		}

		const in_addr * inaddr = reinterpret_cast<const in_addr *>(base_addr);
		unsigned long myaddr = inaddr->S_un.S_addr;
		myaddr_list.push_back(myaddr);
		++addr_No;
	}
}
DualChatClass::~DualChatClass()
{
	closesocket(com_sock);
}


bool DualChatClass::process_message(char * message, const sockaddr_in & cliant_addr)
{
	BinaryData join_request(256);
	join_request.add(system_head, sizeof(system_head));
	join_request.add(system_sign::join);
	if (join_request.is_same_beginning(message))
	{
		register_cliant(cliant_addr);
		const int offset = sizeof(system_head) + strlen(system_sign::join) + 1;
		tell_user_another_joined(message, offset);
		tell_cliant_about_me(cliant_addr);
		return true;
	}

	BinaryData reg_request(256);
	reg_request.add(system_head, sizeof(system_head));
	reg_request.add(system_sign::register_me);
	if (reg_request.is_same_beginning(message))
	{
		register_cliant(cliant_addr);
		const int offset = sizeof(system_head) + strlen(system_sign::register_me) + 1;
		tell_user_another_joined(message, offset);
		return true;
	}

	BinaryData dif_request(256);
	dif_request.add(system_head, sizeof(system_head));
	dif_request.add(system_sign::gota_message);
	if (dif_request.is_same_beginning(message))
	{
		const int offset = sizeof(system_head)+strlen(system_sign::gota_message)+1;
		const char * main_message = message + offset;
		strcpy(message, main_message);
		return true;
	}

	return false;
}

void DualChatClass::tell_user_another_joined(char * message, const int offset)
{
	char name[256];
	strcpy(name, message + offset);
	strcpy(message, "@");
	strcpy(message + 1, name);
	strcpy(message + strlen(message), "さんが入室しました。");
}

void DualChatClass::register_cliant(const sockaddr_in & cliant_addr)
{
	targets.push_back(cliant_addr);
}

void DualChatClass::tell_cliant_about_me(const sockaddr_in & cliant_addr)
{
	const int data_size = 256;
	BinaryData reg_request(data_size);
	reg_request.add(system_head, sizeof(system_head));
	reg_request.add(system_sign::register_me);
	reg_request.add(user_name.c_str());

	sendto(
		com_sock,
		reg_request.get_buffer(),
		data_size,
		0,
		reinterpret_cast<const sockaddr *>(&cliant_addr),
		sizeof(sockaddr_in));
}