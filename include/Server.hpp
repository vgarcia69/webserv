#ifndef SERVER_HPP
	#define SERVER_HPP

	#include <vector>
	#include <string>
	#include <map>
	#include <sstream>
	#include <sys/epoll.h>
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <fcntl.h>
	#include <csignal>
	#include "Location.hpp"
	#include <cstdlib>
	#include "Client.hpp"

	typedef std::map <std::string, std::string>	StringMap;
	typedef std::pair<std::string, std::string> StringPair;

	// std::string				serverName;
	// std::string				ipAddress;       a voir ou les stockés ceux la
 	// std::string 				root;

	class Server
	{
		private:
			StringMap					m_info;	
			std::vector<Location>		m_locations;
    		std::map<int, std::string>	m_errorPages;
			std::vector<Client>			m_clients;
			int							m_maxBodySize;

 			int							m_port;
			int							m_epoll_fd;
			int							m_server_fd;
			epoll_event					m_events[SOMAXCONN]; 

		public:
			Server();			
			~Server();

			void	addLocation(std::string& root);
			void	addInfo(std::string keyword, std::string& info);
			void	addConnexion(int& fd, epoll_event& event);
			void	removeConnexion(int& fd, epoll_event& event);
			void	handleClients(int& fd, epoll_event& event);

			class SafeExit: public std::exception
			{
				public:
					const char* what() const throw()
					{
						return "Shutting Down the Server";
					}
			};

			void		start();
			void		run();
			static void	shut(int);
	};

#endif
