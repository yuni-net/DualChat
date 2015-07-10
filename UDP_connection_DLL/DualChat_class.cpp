#include <string>
#include "DualChat_class.h"

#pragma warning(disable:4996)

int DualChatClass::join_guild()
{
	broadcast_to_find();

	char data[256];
	sockaddr_in target_addr;
	int addr_len = sizeof(sockaddr_in);	// not const for recvfrom

	char expected_response [] = "   plz register me";
	expected_response[0] = 0;
	expected_response[1] = 0;
	expected_response[2] = 96;

	fd_set fds, readfds;
	FD_ZERO(&readfds);
	FD_SET(com_sock, &readfds);

	timeval timev;
	timev.tv_sec = 2;
	timev.tv_usec = 0;

	targets.clear();

	while (true)
	{
		memcpy(&fds, &readfds, sizeof(fd_set));

		int result = select(0, &fds, NULL, NULL, &timev);
		if (result == 0)
		{
			// time is out
			break;
		}

		if (FD_ISSET(com_sock, &fds) == 0)
		{
			// not set
			continue;
		}

		memset(data, 0, sizeof(data));
		recvfrom(
			com_sock,
			data,
			sizeof(data),
			0,
			reinterpret_cast<sockaddr *>(&target_addr),
			&addr_len);

		if (memcmp(data, expected_response, sizeof(expected_response)) == 0)
		{
			targets.push_back(target_addr);
		}
	}

	return targets.size();
}

void DualChatClass::broadcast_to_find()
{
	sockaddr_in sendaddr;
	memset(&sendaddr, 0, sizeof(sockaddr_in));
	sendaddr.sin_family = AF_INET;
	sendaddr.sin_addr.s_addr = inet_addr("255.255.255.255");
	sendaddr.sin_port = htons(9696);

	char data[] = "   plz let me join DualChat";
	data[0] = 0;
	data[1] = 0;
	data[2] = 96;

	sendto(
		com_sock,
		data,
		sizeof(data),
		0,
		(sockaddr *)&sendaddr,
		sizeof(sockaddr_in));
}


void DualChatClass::convert_req_data(char req_data[1024], const char * message)
{
	char dif_request[] = "   you got a message";
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

	memcpy(&fds, &readfds, sizeof(fd_set));

	int result = select(0, &fds, NULL, NULL, &timev);
	if (result == 0)
	{
		return 0;
	}

	if (FD_ISSET(com_sock, &fds))
	{
		memset(message, 0, 1024);
		recvfrom(
			com_sock,
			message,
			1024,
			0,
			reinterpret_cast<sockaddr *>(&cliant_addr),
			&addr_len);

		return process_message(message, cliant_addr);
	}

	return 0;
}

DualChatClass::DualChatClass()
{
	com_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	int perm = 1;
	setsockopt(com_sock, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<const char *>(&perm), sizeof(int));
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
		tell_cliant_about_me(cliant_addr);
		return false;
	}

	char reg_request [] = "   plz register me";
	reg_request[0] = 0;
	reg_request[1] = 0;
	reg_request[2] = 96;

	if (memcmp(message, reg_request, sizeof(reg_request)) == 0)
	{
		register_cliant(cliant_addr);
		return false;
	}

	char dif_request[] = "   you got a message";
	dif_request[0] = 0;
	dif_request[1] = 0;
	dif_request[2] = 96;

	if (memcmp(message, dif_request, sizeof(dif_request)) == 0)
	{
		const char * main_message = message + sizeof(dif_request);
		strcpy(message, main_message);
		return true;
	}

	return false;
}

void DualChatClass::register_cliant(const sockaddr_in & cliant_addr)
{
	targets.push_back(cliant_addr);
}

void DualChatClass::tell_cliant_about_me(const sockaddr_in & cliant_addr)
{
	char reg_request [] = "   plz register me";
	reg_request[0] = 0;
	reg_request[1] = 0;
	reg_request[2] = 96;

	sendto(
		com_sock,
		reg_request,
		sizeof(reg_request),
		0,
		reinterpret_cast<const sockaddr *>(&cliant_addr),
		sizeof(sockaddr_in));
}