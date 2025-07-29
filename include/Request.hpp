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

	//parse function
	void	parsFirstLine(std::string & clientRequest);
	void	parsHeader(std::string & clientRequest);
	void	parsBody(std::string & clientRequest);
	bool 	readSocketBoundary(int socket_fd);
	
	//process response
	void	processHeader();
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
	const std::string& getMethod() const;
	const std::string& getURI() const;
	const std::map<std::string, std::string>& getHeader() const;
	const std::string getHeader(const std::string& key);
	const std::string& getBody() const;
	const std::string& getError() const;
	std::string getHTTPresponse();
};

std::ostream & operator<<(std::ostream &o, Request & request);

#endif
