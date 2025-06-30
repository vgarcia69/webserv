#ifndef LOCATION_HPP
	#define LOCATION_HPP

	#include <vector>
	#include <string>
	#include <iostream>
	#include <map>

	typedef std::map<std::string, std::string>	StringMap;
	typedef std::pair<std::string, std::string> StringPair;

	class Location
	{
		private:
			StringMap					m_info;	
			std::vector<std::string>	m_indexes;
			StringPair					m_cgi;

		public:
			Location();			
			~Location();
			void	addInfo(std::string keyword, std::string& info);
			void	addIndex(std::string& info);
			void	addCGI(std::string& suffix, std::string& info);
	};


#endif
