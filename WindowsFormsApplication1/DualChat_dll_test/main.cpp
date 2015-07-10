#include "../../UDP_connection_DLL/DualChat.h"

int main()
{
	int * handle = begin_DualChat();
	find_server(handle);
	finish_DualChat(handle);
	return 0;
}