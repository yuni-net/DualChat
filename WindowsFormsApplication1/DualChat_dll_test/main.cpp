#include <iostream>
#include <cstdio>
#include "../../UDP_connection_DLL/DualChat.h"

#pragma warning(disable:4996)

int main()
{
	std::cout << "tell me your name" << std::endl;
	char user_name[256];
	scanf("%s", user_name);
	int * handle = begin_DualChat(user_name);
	join_guild(handle);
	std::cout << "you joined the guild" << std::endl;

	char message[1024];
	while (true)
	{
		if (receive_message(handle, message))
		{
			std::cout << message << std::endl;
		}
	}

	finish_DualChat(handle);
	return 0;
}