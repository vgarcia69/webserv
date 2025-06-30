#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <cstring>


class Request {
private :
	std::string		_method;
	std::string		_URI;									// URI : Uniform Resource Identifier
	std::map<std::string, std::string>	_header;
	std::string		_body;
	std::string		_error;
	static const std::set<std::string> _validMethods;

	void	parsFirstLine(std::istream & clientRequest);
	void	parsHeader(std::istream & clientRequest);
	void	parsBody(std::istream & clientRequest);

public :
	Request();
	~Request(){};


	void		parsRequest(std::istream & clientRequest);
	

	//getter
	const std::string& getMethod() const { return _method; }
	const std::string& getURI() const { return _URI; }
	const std::map<std::string, std::string>& getHeader() const { return _header; }
	std::string getHeader(const std::string& key) {return _header[key];} ;
	const std::string& getBody() const { return _body; }
	const std::string& getError() const { return _error; }
};


std::ostream & operator<<(std::ostream &o, Request & request);

#endif