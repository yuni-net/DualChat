#include <WinSock2.h>
#pragma comment(lib, "ws2_32")
#include "DualChat.h"
#include "DualChat_class.h"

int * begin_DualChat()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	return reinterpret_cast<int *>(new DualChatClass());
}

int find_server(int * chat_manager_handle)
{
	DualChatClass & chat = *reinterpret_cast<DualChatClass *>(chat_manager_handle);
	return chat.find_server();
}

void connect_server(int * chat_manager_handle, const char * user_name)
{
	DualChatClass & chat = *reinterpret_cast<DualChatClass *>(chat_manager_handle);
	chat.connect_server(user_name);
}

void build_server(int * chat_manager_handle)
{
	DualChatClass & chat = *reinterpret_cast<DualChatClass *>(chat_manager_handle);
	chat.build_server();
}

void send_message(int * chat_manager_handle, const char * message)
{
	DualChatClass & chat = *reinterpret_cast<DualChatClass *>(chat_manager_handle);
	chat.send_message(message);
}

int receive_message(int * chat_manager_handle, char * message)
{
	DualChatClass & chat = *reinterpret_cast<DualChatClass *>(chat_manager_handle);
	return chat.receive_message(message);
}

void finish_DualChat(int * chat_manager_handle)
{
	DualChatClass & chat = *reinterpret_cast<DualChatClass *>(chat_manager_handle);
	delete &chat;
	WSACleanup();
}