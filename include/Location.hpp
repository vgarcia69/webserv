#ifndef LOCATION_HPP
	#define LOCATION_HPP

	#include <vector>
	#include <string>
	#include <iostream>
	#include <map>
	#include "define.hpp"

	typedef std::map<std::string, std::string>	StringMap;
	typedef std::pair<std::string, std::string> StringPair;

	class Location
	{
		private:
			StringMap					m_info;
			std::vector<std::string>	m_cgi_path;
			std::vector<std::string>	m_cgi_ext;

		public:
			Location();			
			~Location();
			
			std::string					m_root;

			std::vector<std::string>	getCGIPath();
			std::vector<std::string>	getCGIExt();
			std::string					getInfo(std::string keyword);

			void						addInfo(std::string keyword, std::string info);
			void						addCGIPath(std::string cgi_path);
			void						addCGIExt(std::string cgi_ext);
			void		clear();
	};

#endif
