#include <WinSock2.h>
#pragma comment(lib, "ws2_32")
#include "DualChat.h"
#include "DualChat_class.h"

int * begin_DualChat(const char * user_name)
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	return reinterpret_cast<int *>(new DualChatClass(user_name));
}

int join_guild(int * chat_manager_handle)
{
	DualChatClass & chat = *reinterpret_cast<DualChatClass *>(chat_manager_handle);
	chat.join_guild();
	return 0;
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