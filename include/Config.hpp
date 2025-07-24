#ifndef CONFIG_HPP
	#define CONFIG_HPP

	#include <iostream>
	#include <fstream>
	#include <string>
	#include <vector>
	#include <cstdlib>
	#include <sys/stat.h>
	#include <map>
	#include <sys/wait.h>
	#include <sstream>
	#include <unistd.h>
	#include "Server.hpp"
	#include "define.hpp"

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
			ParsingState			m_state;
			Location				m_currentLoc;

		public:
			Config(const std::string& configFile, std::vector<Server>& servers);
			~Config();

			std::vector<Server>&	m_servers;

			void	parsingServerInfo(std::stringstream& sequenced_line, ParsingState& state);
			void	parsingIPAddress(std::string address);
			void	parsingPort(std::string& port);
			void	parsingServerName(std::string& info);
			void	parsingMaxBodySize(std::string& info);
			void	parseDefaultFile(std::string& info);
			void	parseErrorPage(std::string& number, std::string& path);

			void	parsingLocationInfo(std::stringstream& sequenced_line, ParsingState& state);
			void	parseLocDefaultFile(std::string& path);
			void	parseReturn(std::string info);
			void	parseAutoIndex(std::string info);
			void	parseMethods(std::stringstream& sequenced_line);

			void	checkRoot(std::string& info);
			void	addLocationInfo(std::string keyword, std::string info);
	};

	bool	lookingFor(std::stringstream& content, std::string target);
	void	setupContent(std::fstream& configIn, std::stringstream& content);

#endif
