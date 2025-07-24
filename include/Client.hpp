#ifndef CLIENT_HPP
	#define CLIENT_HPP

	#include <string>
	#include "Request.hpp"

	class Client
	{
		private:
			int				m_port;
			int				m_socket_fd;
			std::string 	m_ip_address;
			Request			*m_request;


		public:
		 	Client() {}
			Client(std::string ip_address, int port, int socket_fd);
			~Client();

			int				getPort();
			std::string&	getIPAdress();
			int				getSocketFD();
			Request&		getRequest();

			void			setPort(int);
			void			setIPAdress(std::string);
			void			setSocketFD(int);
			void			setRequest(Request& request);
	};

#endif
