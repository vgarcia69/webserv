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
			StringPair					m_cgi;

		public:
			Location();			
			~Location();
			
			std::string					m_root;

			void		addInfo(std::string keyword, std::string info);
			std::string	getInfo(std::string keyword);
			void		addCGI(std::string& suffix, std::string& info);
			void		clear();
	};


#endif
