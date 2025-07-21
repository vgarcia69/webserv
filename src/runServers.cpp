#include "Config.hpp"

void	Config::startServers(void)
{
	for (unsigned i = 0; i < m_servers.size(); i++)
	{
		// on tente de bind chaque server a un listen_fd pour avoir une vec de listen
		// ensuite on epoll de chaaque listen en boucle
	}
}
