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
			std::string		m_processing_request;

		public:
		 	Client() {}
			Client(std::string ip_address, int port, int socket_fd);
			~Client();

			std::string		m_response;

			//Request			request;

			int				getPort();
			std::string&	getIPAdress();
			int				getSocketFD();

			void			setPort(int);
			void			setIPAdress(std::string);
			void			setSocketFD(int);
			bool 			readSocket(int socket_fd, size_t sizeBody);
			std::string		getProcessRequest() { return m_processing_request; }

			void			clear();
	};

#endif
