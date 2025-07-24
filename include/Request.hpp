#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "utils.hpp"
#include "define.hpp"

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

class Request{
private :
	std::string							_method;
	std::string							_URI;					// URI : Uniform Resource Identifier
	std::map<std::string, std::string>	_header;
	std::string							_body;
	std::string							_error;
	std::ostringstream					_HTTPresponse;

	void	processHeader();
	void	parsFirstLine(std::string & clientRequest);
	void	parsHeader(std::string & clientRequest);
	void	parsBody(std::string & clientRequest);

	void	handleError();
	void	handleGET();
	void	handlePOST();
	void	handleDELETE();

	static const std::map<std::string, void (Request::*)()> _methodMap;
	static std::map<std::string, void (Request::*)()> _createMethodMap();

public :
	Request();
	~Request(){};

	void		parsRequest(std::string clientRequest);
	void		handleRequest();

	//getter
	const std::string& getMethod() const { return _method; }
	const std::string& getURI() const { return _URI; }
	const std::map<std::string, std::string>& getHeader() const { return _header; }
	const std::string getHeader(const std::string& key) {return _header[key];} ;
	const std::string& getBody() const { return _body; }
	const std::string& getError() const { return _error; }
	std::string getHTTPresponse() {return _HTTPresponse.str();}
};

std::ostream & operator<<(std::ostream &o, Request & request);

#endif
