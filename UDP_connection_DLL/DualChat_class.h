#ifndef DualChat_class_h_
#define DualChat_class_h_

#include <WinSock2.h>
#pragma comment(lib, "ws2_32")
#include <vector>

class DualChatClass
{
public:
	/***
	@brief search the server
	@return found...1, not found...0
	*/
	int find_server();

	/***
	@brief connect to the server
	@param plz set your user name(0-127 words)
	*/
	void connect_server(const char * user_name);

	/***
	@brief build the server
	*/
	void build_server();

	/***
	@brief send the message to the server
	@detail And the server send it to all cliant
	@param plz set your message(0-256 words) included your name
	*/
	void send_message(const char * message);

	/***
	@brief try to receive a message
	@return received...1, not received...0
	*/
	int receive_message(char * message);



	DualChatClass();
	~DualChatClass();
private:
	bool Ima_server;

	sockaddr_in server_addr;
	SOCKET com_sock;
//	char received_data[1024];

	bool I_have_to_show;
	char todo_show_data[1024];

	std::vector<sockaddr_in> cliants;


	/***
	@brief send broadcast message to find the server
	*/
	void broadcast_to_find();

	/***
	@brief convert message to req_data
	@param
	    req_data: set the buffer to receive req_data
		message: set your message
	*/
	void convert_req_data(char req_data[1024], const char * message);

	/***
	@brief If received message is system message, process the message.
	@param
	    message: set received message
		cliant_addr: set cliant's sockaddr_in variable
	@return
	    You should show the message as user's message ... true
		You should NOT show it. Caz it's system message ... false
	*/
	bool process_message(const char * message, const sockaddr_in & cliant_addr);

	/***
	@brief tell the cliant that I'm the server.
	@param set cliant's sockaddr_in variable.
	*/
	void tell_cliant_Ima_server(const sockaddr_in & cliant_addr);

	/***
	@brief register cliant's sockaddr_in.
	@param set cliant's sockaddr_in variable.
	*/
	void register_cliant(const sockaddr_in & cliant_addr);

	/***
	@brief send the message to all cliant.
	@param set your message.
	*/
	void diffuse_message(const char * message);

	/***
	@brief check whether message is the system message for finding server.
	@param set received message.
	@return
	    the message is system message for finding server ... true
		the message is NOT system message for finding server ... false
	*/
	bool message_is_finding_server(const char * message, const sockaddr_in & cliant_addr);
};

#endif