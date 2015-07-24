#include <string>
#include <iostream>
#include "DualChat_class.h"

#pragma warning(disable:4996)

void DualChatClass::join_guild(const char * user_name)
{
	this->user_name = user_name;
	broadcast_to_find();
	targets.clear();
}

void DualChatClass::broadcast_to_find()
{
	memset(&broad_addr, 0, sizeof(sockaddr_in));
	broad_addr.sin_family = AF_INET;
	broad_addr.sin_addr.s_addr = inet_addr("255.255.255.255");
	broad_addr.sin_port = htons(9696);

	const int data_size = 256;
	char data[data_size] = "   plz let me join DualChat";
	data[0] = 0;
	data[1] = 0;
	data[2] = 96;
	strcpy(data + 3 + strlen(data + 3) + 1, user_name.c_str());

	int result = sendto(
		com_sock,
		data,
		data_size,
		0,
		(sockaddr *) &broad_addr,
		sizeof(sockaddr_in));
}


void DualChatClass::convert_req_data(char req_data[1024], const char * message)
{
	char dif_request [] = "   you got a message";
	dif_request[0] = 0;
	dif_request[1] = 0;
	dif_request[2] = 96;

	strcpy(req_data, dif_request);
	strcpy(req_data + sizeof(dif_request), message);
}

void DualChatClass::send_message(const char * message)
{
	char req_data[1024];
	convert_req_data(req_data, message);

	// debug
	printf("send message:\n");
	for (int i = 0; i < 32; ++i)
	{
		std::cout << message[i] << '\0';
	}
	std::cout << std::endl;
	

	for (unsigned int target_No = 0; target_No < targets.size(); ++target_No)
	{
		auto & target_addr = targets[target_No];
		sendto(
			com_sock,
			req_data,
			sizeof(req_data),
			0,
			reinterpret_cast<sockaddr *>(&target_addr),
			sizeof(sockaddr_in));

		std::cout << target_No << ": sended to " << target_addr.sin_addr.S_un.S_addr << std::endl;
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

		if (cliant_addr.sin_addr.S_un.S_addr == my_addr)
		{
			// 自分自身からのメッセージは無視する
			continue;
		}

		// debug
		printf("you got a message");

		if (process_message(message, cliant_addr))
		{
			return 1;
		}
	}

	return 0;
}

DualChatClass::DualChatClass()
{
	com_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	int perm = 1;
	setsockopt(com_sock, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<const char *>(&perm), sizeof(int));

	recv_addr.sin_family = AF_INET;
	recv_addr.sin_port = htons(9696);
	recv_addr.sin_addr.S_un.S_addr = INADDR_ANY;
	int result = bind(com_sock, reinterpret_cast<sockaddr *>(&recv_addr), sizeof(recv_addr));
	if (result == -1)
	{
		std::cout << "failed to bind port" << std::endl;
	}

	my_addr = get_myaddr();
}

unsigned long DualChatClass::get_myaddr()
{
	const int buffersize = 256;
	char buffer[buffersize];
	gethostname(buffer, buffersize);

	HOSTENT * host = gethostbyname(buffer);

	char * address = host->h_addr_list[0];

	if (address == nullptr)
	{
		std::string("自身のアドレスの取得に失敗しました");
	}

	const in_addr * inaddr = reinterpret_cast<const in_addr *>(address);
	const char * name = inet_ntoa(*inaddr);
	return inet_addr(name);
}
DualChatClass::~DualChatClass()
{
	closesocket(com_sock);
}


bool DualChatClass::process_message(char * message, const sockaddr_in & cliant_addr)
{
	char join_request [] = "   plz let me join DualChat";
	join_request[0] = 0;
	join_request[1] = 0;
	join_request[2] = 96;

	if (memcmp(message, join_request, sizeof(join_request)) == 0)
	{
		register_cliant(cliant_addr);
		tell_user_another_joined(message, sizeof(join_request));
		tell_cliant_about_me(cliant_addr);
		return true;
	}

	char reg_request [] = "   plz register me";
	reg_request[0] = 0;
	reg_request[1] = 0;
	reg_request[2] = 96;

	if (memcmp(message, reg_request, sizeof(reg_request)) == 0)
	{
		register_cliant(cliant_addr);
		tell_user_another_joined(message, sizeof(reg_request));
		return true;
	}

	char dif_request[] = "   you got a message";
	dif_request[0] = 0;
	dif_request[1] = 0;
	dif_request[2] = 96;

	if (memcmp(message, dif_request, sizeof(dif_request)) == 0)
	{
		const char * main_message = message + sizeof(dif_request);
		strcpy(message, main_message);
		std::cout << "you got a message:\n" << message << std::endl;
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
	std::cout << "a cliant was registered" << std::endl;
	std::cout << "targets' size: " << targets.size() << std::endl;
	for (unsigned int target_No = 0; target_No < targets.size(); ++target_No)
	{
		const auto & target = targets[target_No];
		std::cout << target_No << ": " << target.sin_addr.S_un.S_addr << std::endl;
	}
}

void DualChatClass::tell_cliant_about_me(const sockaddr_in & cliant_addr)
{
	const int data_size = 256;
	char reg_request [data_size] = "   plz register me";
	reg_request[0] = 0;
	reg_request[1] = 0;
	reg_request[2] = 96;

	strcpy(reg_request + 3 + strlen(reg_request + 3) + 1, user_name.c_str());

	sendto(
		com_sock,
		reg_request,
		data_size,
		0,
		reinterpret_cast<const sockaddr *>(&cliant_addr),
		sizeof(sockaddr_in));
}