#ifndef DualChat_h_
#define DualChat_h_

extern "C" {
	__declspec(dllexport) int * begin_DualChat();
	__declspec(dllexport) int join_guild(int * chat_manager_handle);
	__declspec(dllexport) void send_message(int * chat_manager_handle, const char * message);
	__declspec(dllexport) int receive_message(int * chat_manager_handle, char * message);
	__declspec(dllexport) void finish_DualChat(int * chat_manager_handle);
}

#endif