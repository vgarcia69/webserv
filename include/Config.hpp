#ifndef CONFIG_HPP
	#define CONFIG_HPP

	#include <iostream>
	#include <fstream>
	#include <string>
	#include <vector>
	#include <cstdlib>
	#include <map>
	#include <sstream>
	#include "Server.hpp"

	#define ERROR_SERVERBLOCK "Server block not found"
	#define	ERROR_BRACKET "Bracket to open a block was not found"

	enum	ParsingState
	{
		GLOBAL,
		SERVER_BLOCK,
		LOCATION_BLOCK,
		LOOKING_FOR_SERVER_BLOCK,
		LOOKING_FOR_LOCATION_BLOCK,
		ERROR
	};

	class Config
	{
		private:
			std::vector<Server> m_servers;

		public:
			Config();
			Config(const std::string& configFile, Server& servers);
			~Config();
	};

	void		lookingFor(std::stringstream& content, std::string target, const char* errorMessage);
	void		lookingForBracket(std::stringstream& content);
    void		ParsingServerInfo(std::stringstream& content, ParsingState& state, Server& server);
    void		ParsingLocationInfo(std::stringstream& content, ParsingState& state, Server& server);

#endif
