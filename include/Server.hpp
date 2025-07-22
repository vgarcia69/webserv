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
	#include "define.hpp"

	typedef std::map <std::string, std::string>	StringMap;
	typedef std::pair<std::string, std::string> StringPair;

	class Server
	{
		private:
			StringMap					m_info;	
			std::vector<Location>		m_locations;
    		std::map<int, std::string>	m_errorPages;
			std::vector<Client>			m_clients;
			int							m_server_fd;

		public:
			Server();
			~Server();

			void		addLocation(std::string& root);
			void		addLocationInfo(std::string keyword, std::string info);
			void		addInfo(std::string keyword, std::string info);
			void		addErrorPage(int nbr, std::string path);
			std::string	getLocationInfo(std::string keyword);
			std::string	getInfo(std::string keyword);
			std::string	getErrorPage(int nbr);
			int			getServerFD();

			class SafeExit: public std::exception
			{
				public:
					const char* what() const throw()
					{
						return "Shutting Down the Server";
					}
			};

			static int	s_nb_servers_running;
			static int	s_epoll_fd;

			bool		init();
	};

    void	addConnexion(int& fd, epoll_event& event, std::map<int, Client>& clients);
	void	removeConnexion(epoll_event& event, std::map<int, Client>& clients);
    void	handleClients(int& client_fd, epoll_event& event, std::map<int, Client>& clients);

#endif
