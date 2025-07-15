#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "utils.hpp"
#include "defineError.hpp"


#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <functional>

#include <unistd.h>

class Request {
private :
	std::string		_method;
	std::string		_URI;									// URI : Uniform Resource Identifier
	std::map<std::string, std::string>	_header;
	std::string		_body;
	std::string		_error;
	std::ostream &	_client;								//must be a iostram after

	void	processHeader();
	void	parsFirstLine(std::istream & clientRequest);
	void	parsHeader(std::istream & clientRequest);
	void	parsBody(std::istream & clientRequest);

	void	handleError();
	void	handleGET();
	void	handlePOST();
	void	handleDELETE();

	static const std::map<std::string, void (Request::*)()> _methodMap;
	static std::map<std::string, void (Request::*)()> _createMethodMap();

	public :
	Request();
	~Request(){};


	void		parsRequest(std::istream & clientRequest);
	void		handleRequest();


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
