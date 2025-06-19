#include "Request.hpp"
#include <sstream>
#include <cctype>
#include <set>

Request::Request(void) : _httpVersion(0){}


void	Request::parsFirstLine(std::istream & clientRequest) {
	std::string line;
	std::getline(clientRequest, line);
	if (line.empty()){
		_error =  "Bad input in the first line.";
		return ;
	}
	size_t	space1 = line.find(' ');
	_method = line.substr(0, space1);
	
	static const std::set<std::string> validMethods = {"GET", "POST", "DELETE"};
	if (validMethods.find(_method) == validMethods.end()){
		_error =  "Bad input in the first line.";
		return ;
	}

	if (space1 == std::string::npos){
		_error =  "Bad input in the first line.";
		return ;
	}
	++space1;
	size_t space2 = line.find(' ', space1);
	_URI = line.substr(space1, space2 - space1);
	
	if (space2 == std::string::npos || _URI.empty()){
		_error =  "Bad input in the first line.";
		return ;
	}
	
	++space2;
	if(line.find(' ', space2) != std::string::npos){
		_error =  "Bad input in the first line.";
		return ;
	}

	std::string version = line.substr(space2);
	if (version != "HTTP/1.1")
	{
		_error="Bad input in the first line.";
		return ;
	}
	_httpVersion = 1.1;
}


void		Request::parsHeader(std::istream & clientRequest){
	std::string	line;

	std::getline(clientRequest, line);
	while(line.empty() == false){
		if (clientRequest.eof()){
			_error = "No empty line after Header";
			return ;
		}
		size_t	posSep = line.find(':');
		if (posSep == std::string::npos || posSep >= line.length() + 3){
			_error = "Bad input in the Header.";
			return ;
		}
		_header[line.substr(0, posSep)] = line.substr(posSep + 2);
		std::getline(clientRequest, line);
	}

	//test the validity of the header

	if (_header.find("Host") == _header.end()){
		_error = "No Host.";
		return ;
	}

	if (_header.find("Truc") == _header.end()) ;

	if (_method == "POST"){
		if (_header.find("Content-Type") == _header.end()){
			_error = "No Content-Type on a POST Request.";
			return ;
		}
		/*----------------------------------------- CHECKER TOUTE LES POSSIBILITER D'UNE REQUETES POST---------------------------*/
		// if (_header.find("Content-Length") == _header.end() && _header.find("Transfer-Encoding") != "chunked"){
		// 	_error = "No Content-Length on a POST Request.";
		// 	return ;
		// }
		// if (_header.find("Content-Length") != _header.end()  && _header.find("Transfer-Encoding") == "chunked"){
		// 	_error = "error 400";
		// 	return ;
		// }
	}
}


void		Request::parsBody(std::istream & clientRequest){
	if (_method != "POST"){
		if (clientRequest.eof() == false)
			_error = "No POST methode have a body.";
		return ;
	}

	//case of POST method
	if (_header.find("Content-Length") != _header.end()){
		 
		size_t nb_char = strtol(_header["Content-Length"].c_str(), NULL, 10);
		if (errno == ERANGE || nb_char < 0){
			errno = 0;
			_error = "Nomber of Content-Length is too big or negative.";
			return ;
		}

		std::cout << "Il faut finir le travail " << nb_char <<std::endl;

	}
}

void		Request::parsRequest(std::istream & clientRequest){

	parsFirstLine(clientRequest);
	if (_error.empty() == false)
		return ; 
	parsHeader(clientRequest);
	if (_error.empty() == false)
		return ;
	parsBody(clientRequest);
}

std::ostream & operator<<(std::ostream &o, Request & request) {
	o << "===== REQUEST INFO =====" << "\n";
	o << "Method: " << request.getMethod() << "\n";
	o << "URI: " << request.getURI() << "\n";
	o << "HTTP Version: " << request.getHttpVersion() << "\n";

	o << "\n----- Headers -----" << "\n";
	const std::map<std::string, std::string>& headers = request.getHeader();
	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); 
		 it != headers.end(); ++it) {
		o << it->first << ": " << it->second << "\n";
	}
	if(request.getMethod() == "POST"){
		o << "\n----- Body -----" << "\n";
		o << request.getBody() << "\n";	
	}
	else
		o << "\n----- no body ------\n";
		
	o << "===================" << std::endl;
	return o;
}