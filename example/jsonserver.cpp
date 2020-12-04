#include "websocket_server.hpp"






int main(){

	ws::WebSocketJsonServer().run(8080);
	

	std::cerr << "___main_thread_finished__" << std::endl;
	return 0;
}//end_main