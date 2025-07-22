#ifndef CLIENT_HPP
	#define CLIENT_HPP

	#include <string>

	class Client
	{
		private:
			int			m_port;
			int			m_socket_fd;
			std::string m_ip_address;
			std::string	m_requestContent;

		public:
		 	Client() {}
			Client(std::string ip_address, int port, int socket_fd);
			~Client();

			int			getPort();
			std::string	&getIPAdress();
			int			getSocketFD();

			void		setPort(int);
			void		setIPAdress(std::string);
			void		setSocketFD(int);
	};

#endif
