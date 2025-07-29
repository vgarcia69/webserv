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

	class Client;
	class Server
	{
		private:
			StringMap							m_info;	
			std::map<std::string, Location>		m_locations;
    		std::map<int, std::string>			m_errorPages;
			std::vector<Client>					m_clients;
			int									m_server_fd;

		public:
			Server();
			~Server();

			void		addLocation(Location loc, std::string& root);
			void		addLocationInfo(std::string keyword, std::string info);
			void		addInfo(std::string keyword, std::string info);
			void		addErrorPage(int nbr, std::string path);

			std::string	getLocationInfoOf(std::string keyword, Location& loc);
			std::string	getInfo(std::string keyword);
			std::string	getErrorPage(int nbr);
			int			getServerFD();

			bool		init();
			void		clear();

			static int	s_nb_servers_running;
			static int	s_epoll_fd;
	};

    void	addConnexion(int& fd, epoll_event& event, std::map<int, Client>& clients);
	void	removeConnexion(epoll_event& event, std::map<int, Client>& clients);
    void	handleRequest(int& client_fd, epoll_event& event, std::map<int, Client>& clients, std::vector<Server> &servers);
	void	handleResponse(int& client_fd, epoll_event& event, std::map<int, Client>& clients);

#endif
