#ifndef CONFIG_HPP
	#define CONFIG_HPP

	#include <iostream>
	#include <fstream>
	#include <string>
	#include <vector>
	#include <cstdlib>
	#include <map>

	enum	ParsingState
	{
		GLOBAL,
		SERVER_BLOCK,
		LOCATION_BLOCK,
		LOOKING_FOR_SERVER_BLOCK,
		LOOKING_FOR_LOCATION_BLOCK,
		ERROR
	};

	struct t_Location
	{
		std::string 	root;
		std::string		methods;
		std::string		redirect;
		std::string		searchedReference;
		std::string		tryFilesOrder;
		std::string		uploadPath;
		bool			autoIndex;
	};

	struct t_ServerInfo
	{
		std::string					serverName;
		std::string					ipAddress;
		std::string					port;
		std::vector<std::string>	errorPages;
		std::string 				root;
		unsigned int				bodySize;

		std::vector<t_Location>		locations;
	};

	class Config
	{
		private:
			std::vector<t_ServerInfo> m_servers;
			std::vector<std::string> m_port;
			std::string m_serverName;

		public:
			Config();
			Config(const std::string& configFile);
			~Config();
	};

	ParsingState	lookingForServerCall(std::fstream& infile, std::string& currentString);
	void			lookingForBracket(std::fstream& infile, std::string& currentLine);

#endif
