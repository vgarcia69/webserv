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
			ParsingState					m_state;
			std::vector<Server>				m_servers;

		public:
			Config();
			Config(const std::string& configFile, Server& servers);
			~Config();
			void	ParsingServerInfo(std::stringstream& sequenced_line, ParsingState& state, Server& server);
			void	parsingIPAddress(std::string& address);
    		// void	ParsingLocationInfo(std::stringstream& content, ParsingState& state, Server& server);
	};

	void	lookingFor(std::stringstream& content, std::string target, const char* errorMessage);
	void	setupContent(std::fstream& configIn, std::stringstream& content);

#endif
