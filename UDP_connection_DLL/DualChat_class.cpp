#include <string>
#include "DualChat_class.h"

#pragma warning(disable:4996)

void DualChatClass::broadcast_to_find()
{
	sockaddr_in sendaddr;
	memset(&sendaddr, 0, sizeof(sockaddr_in));
	sendaddr.sin_family = AF_INET;
	sendaddr.sin_addr.s_addr = inet_addr("255.255.255.255");
	sendaddr.sin_port = htons(9696);

	char data[] = "   I'm finding the server";
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

int DualChatClass::find_server()
{
	broadcast_to_find();

	char data[256];
	int addr_len = sizeof(sockaddr_in);	// not const for recvfrom

	char servers_response[] = "   I'm the server";
	servers_response[0] = 0;
	servers_response[1] = 0;
	servers_response[2] = 96;

	fd_set fds, readfds;
	FD_ZERO(&readfds);
	FD_SET(com_sock, &readfds);

	timeval timev;
	timev.tv_sec = 2;
	timev.tv_usec = 0;

	while (true)
	{
		memcpy(&fds, &readfds, sizeof(fd_set));

		int result = select(0, &fds, NULL, NULL, &timev);
		if (result == 0)
		{
			// time is out
			Ima_server = true;
			return 0;
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
			reinterpret_cast<sockaddr *>(&server_addr),
			&addr_len);

		if (memcmp(data, servers_response, sizeof(servers_response))==0)
		{
			Ima_server = false;
			return 1;
		}
	}

	return 0;
}

void DualChatClass::connect_server(const char * user_name)
{
	char data[256] = "   plz register me";
	data[0] = 0;
	data[1] = 0;
	data[2] = 96;

	int send_res = sendto(
		com_sock,
		data,
		sizeof(data),
		0,
		(sockaddr *)&server_addr,
		sizeof(server_addr));

	// ëóêMÇÕïKÇ∏ê¨å˜Ç∑ÇÈÇ∆âºíËÇ∑ÇÈ
}

void DualChatClass::build_server()
{
	cliants.clear();

	memset(&server_addr, 0, sizeof(sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(9696);

	cliants.push_back(server_addr);
}

void DualChatClass::convert_req_data(char req_data[1024], const char * message)
{
	char dif_request[] = "   plz diffuse that";
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
	sendto(
		com_sock,
		req_data,
		sizeof(req_data),
		0,
		(sockaddr *)&server_addr,
		sizeof(server_addr));
}

/***
@brief check whether message is the system message for finding server.
@param set received message.
@return
    the message is system message for finding server ... true
    the message is NOT system message for finding server ... false
@detail
    In this function, I did the process of when the message was system message for finding server.
*/
bool DualChatClass::message_is_finding_server(const char * message, const sockaddr_in & cliant_addr)
{
	char finding[] = "   I'm finding the server";
	finding[0] = 0;
	finding[1] = 0;
	finding[2] = 96;

	if (memcmp(message, finding, sizeof(finding)) == 0)
	{
		tell_cliant_Ima_server(cliant_addr);
		return true;
	}

	return false;
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

		if (message_is_finding_server(message, cliant_addr))
		{
			return 0;
		}

		if (Ima_server)
		{
			return process_message(message, cliant_addr);
		}

		return 1;
	}

	return 0;
}

DualChatClass::DualChatClass()
{
	I_have_to_show = false;

	com_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	int perm = 1;
	setsockopt(com_sock, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<const char *>(&perm), sizeof(int));
}
DualChatClass::~DualChatClass()
{
	closesocket(com_sock);
}


bool DualChatClass::process_message(const char * message, const sockaddr_in & cliant_addr)
{
	char reg_request[] = "   plz register me";
	reg_request[0] = 0;
	reg_request[1] = 0;
	reg_request[2] = 96;

	if (memcmp(message, reg_request, sizeof(reg_request)) == 0)
	{
		register_cliant(cliant_addr);
		return false;
	}

	char dif_request[] = "   plz diffuse that";
	dif_request[0] = 0;
	dif_request[1] = 0;
	dif_request[2] = 96;

	if (memcmp(message, dif_request, sizeof(dif_request)) == 0)
	{
		diffuse_message(message+sizeof(dif_request));
		return false;
	}

	return true;
}

void DualChatClass::tell_cliant_Ima_server(const sockaddr_in & cliant_addr)
{
	char servers_response[] = "   I'm the server";
	servers_response[0] = 0;
	servers_response[1] = 0;
	servers_response[2] = 96;

	sendto(
		com_sock,
		servers_response,
		sizeof(servers_response),
		0,
		(sockaddr *)&cliant_addr,
		sizeof(sockaddr_in));
}

void DualChatClass::register_cliant(const sockaddr_in & cliant_addr)
{
	cliants.push_back(cliant_addr);
}

void DualChatClass::diffuse_message(const char * message)
{
	for (unsigned int cliant_No = 0; cliant_No < cliants.size(); ++cliant_No)
	{
		auto & cliant_addr = cliants[cliant_No];

		sendto(
			com_sock,
			message,
			1024,
			0,
			(sockaddr *)&cliant_addr,
			sizeof(sockaddr_in));
	}
}
