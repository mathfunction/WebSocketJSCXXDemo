#ifndef __WEBSOCKET_SERVER_CXX__
#define __WEBSOCKET_SERVER_CXX__


#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <mutex>
#include <unordered_map>

#define _WEBSOCKETPP_CPP11_THREAD_  // <boost_thread> bug !!
#define ASIO_STANDALONE
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <asio/io_service.hpp>
#include <json.hpp>


namespace ws{
	typedef websocketpp::server<websocketpp::config::asio> WebSocketEndpoint;
	typedef websocketpp::connection_hdl ClientConnection;


	class WebSocketServerEngine{
		//===================================================================================================================
		private:
			asio::io_service engineLoop;
			asio::io_service &serverLoop;
			WebSocketEndpoint endpoint;
			std::vector<ClientConnection> openConnections;
			std::mutex globalLock;

			
			static nlohmann::json parseJson(const std::string& jsonstr){
				return nlohmann::json::parse(jsonstr);
			}
			static std::string stringifyJson(const nlohmann::json& d){
				return d.dump();
			}

			
			void print(std::string _str){
				serverLoop.post([_str](){
					std::cout << _str << std::endl;
				});
			}

			void onOpen(ClientConnection conn){
				{
					std::lock_guard<std::mutex> lock(this->globalLock);
					this->openConnections.push_back(conn);
					
				}
				print("onOpen #="+std::to_string(this->numConnections()));
			}//onOpen

			void onClose(ClientConnection conn){
				{
					std::lock_guard<std::mutex> lock(this->globalLock);
					auto connVal = conn.lock(); 
					auto newEnd = std::remove_if(this->openConnections.begin(), this->openConnections.end(), [&connVal](ClientConnection elem){
						if(elem.expired() == true){
							return true;
						}//endif
					
						auto elemVal = elem.lock();
						if(elemVal.get() == connVal.get()){
							return true;
						}//endif
						return false;
					});
					this->openConnections.resize(std::distance(this->openConnections.begin(), newEnd));
				}
				print("onClose #="+std::to_string(this->numConnections()));
			}//onClose


			void onMessage(ClientConnection conn, WebSocketEndpoint::message_ptr msg){
				//print("onMessage !!");
				try{
					nlohmann::json d = WebSocketServerEngine::parseJson(msg->get_payload());
					// ===========================================================================
					// include some hpp !!
					// ============================================================================
					this->sendMessage(conn,d);
					print("sendMessage ");
				}catch(...){
					nlohmann::json error;
					error["error"] = "parseJson";
					this->sendMessage(conn,error);
					print("parseJson Failed ");
				}//end_catch
				
			}//end_onMessage
			

		//=======================================================================================================================
		public:
			WebSocketServerEngine(asio::io_service &outer) : serverLoop(outer){
				// receive messages from client (javascript)
				this->endpoint.set_open_handler(std::bind(&WebSocketServerEngine::onOpen, this, std::placeholders::_1));
				this->endpoint.set_close_handler(std::bind(&WebSocketServerEngine::onClose, this, std::placeholders::_1));
				this->endpoint.set_message_handler(std::bind(&WebSocketServerEngine::onMessage, this, std::placeholders::_1, std::placeholders::_2));
				this->endpoint.init_asio(&(this->engineLoop));
			}//end_WebsocketServer()

			void serve(int port){
				print("listening ...");
				this->endpoint.listen(port);
				this->endpoint.start_accept();
				this->endpoint.run();

			}//end_serve
			
			
			size_t numConnections(){
				std::lock_guard<std::mutex> lock(this->globalLock);
				return this->openConnections.size();
			}
			

			void sendMessage(ClientConnection conn, const nlohmann::json& d){
				this->endpoint.send(conn, WebSocketServerEngine::stringifyJson(d), websocketpp::frame::opcode::text);
			}//end_sendMessage

			void broadcastMessage(const nlohmann::json& d){
				std::lock_guard<std::mutex> lock(this->globalLock);
				for(auto conn:this->openConnections) {
					this->sendMessage(conn,d);
				}//endfor
			}//end_broadcastMessage
			
	};


	class WebSocketJsonServer{
		public:
			void run(int PORT_NUMBER){
				std::cerr << "PORT = " << PORT_NUMBER << std::endl;					
				asio::io_service serverLoop;
				asio::io_service::work IdleWork(serverLoop);
				WebSocketServerEngine engine(serverLoop);
				// new thread to run engine !!
				std::thread engineThread(
					[PORT_NUMBER,&engine](){
						engine.serve(PORT_NUMBER);
					}
				);
				serverLoop.run();
			}//end_run
	};


};































#endif 