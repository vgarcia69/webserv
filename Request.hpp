#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>
#include <map>
#include <vector>

class Request {
protected :
	std::string		_method;
	std::string		_URI;									// URI : Uniform Resource Identifier
	float			_httpVersion;
	std::map<std::string, std::string>	_header;
	std::string		_body;

public :
	Request();
	~Request();

	//getter
	const std::string& getMethod() const { return _method; }
	const std::string& getURI() const { return _URI; }
	float getHttpVersion() const { return _httpVersion; }
	const std::map<std::string, std::string>& getHeader() const { return _header; }
	const std::string& getBody() const { return _body; }


	//std::string getHeader(const std::string& key) const /*{return _header[key] ;}*/;
	std::string getHeader(const std::string& key) const ;
	bool		parsRequest(int client_fd);

};

#endif