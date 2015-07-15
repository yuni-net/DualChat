#ifndef DualChat_class_h_
#define DualChat_class_h_

#include <WinSock2.h>
#pragma comment(lib, "ws2_32")
#include <vector>

class DualChatClass
{
public:
	DualChatClass(const char * user_name);

	/***
	@brief find and join DualChat's guild.
	*/
	void join_guild();

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



	~DualChatClass();
private:
	SOCKET com_sock;
	sockaddr_in broad_addr;
	sockaddr_in recv_addr;
	unsigned long my_addr;
	std::string user_name;
	std::vector<sockaddr_in> targets;


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
	@brief process the received message.
	@param
	    message: set received message
		cliant_addr: set cliant's sockaddr_in variable
	@return
	    You should show the message as user's message ... true
		You should NOT show it. Caz it's system message ... false
	*/
	bool process_message(char * message, const sockaddr_in & cliant_addr);

	/***
	@brief register cliant's sockaddr_in.
	@param set cliant's sockaddr_in variable.
	*/
	void register_cliant(const sockaddr_in & cliant_addr);

	/***
	@brief tell the cliant about me.
	@param set the cliant's sockaddr_in.
	*/
	void tell_cliant_about_me(const sockaddr_in & cliant_addr);

	/***
	@brief set the message which tell the user that another person did join this guild in 'message' buffer.
	@param
	    message: set the buffer to receive the message.
		offset: set the offset for the name's point.
	*/
	void tell_user_another_joined(char * message, const int offset);

	/***
	@brief get my address as unsigned long value.
	*/
	unsigned long get_myaddr();
};

#endif