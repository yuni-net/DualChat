#include <iostream>
#include <cstdio>
#include "../../UDP_connection_DLL/DualChat.h"

#pragma warning(disable:4996)

int main()
{
	std::cout << "tell me your name" << std::endl;
	char user_name[256];
	scanf("%s", user_name);
	int * handle = begin_DualChat();
	join_guild(handle, user_name);
	std::cout << "you joined the guild" << std::endl;

	char message[1024];
	while (true)
	{
		while (true)
		{
			if (receive_message(handle, message))
			{
				std::cout << message << std::endl;
				break;
			}
		}

		scanf("%s", message);
		send_message(handle, message);
	}

	finish_DualChat(handle);
	return 0;
}