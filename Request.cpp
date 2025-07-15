#include "Request.hpp"



Request::Request(void): _client(std::cout){}


void		Request::parsFirstLine(std::istream & clientRequest) {
	std::string line;
	std::getline(clientRequest, line);
	if (line.empty()){
		_error =  ERROR_400;
		return ;
	}
	size_t	space1 = line.find(' ');
	_method = line.substr(0, space1);


	if (_methodMap.find(_method) == _methodMap.end()){
		_error = ERROR_405;
		return ;
	}

	if (space1 == std::string::npos){
		_error =  ERROR_400;
		return ;
	}
	++space1;
	size_t space2 = line.find(' ', space1);
	_URI = line.substr(space1, space2 - space1);

	if (space2 == std::string::npos || _URI.empty()){
		_error =  ERROR_400;
		return ;
	}

	++space2;
	if(line.find(' ', space2) != std::string::npos){
		_error =  ERROR_400;
		return ;
	}

	std::string version = line.substr(space2);
	if (!version.empty() && version[version.length() - 1] != '\r') {
		_error =  ERROR_400;
		return ;
	}
	if (version != "HTTP/1.1\r")
	{
		_error = ERROR_505;
		return ;
	}
}


void		Request::parsHeader(std::istream & clientRequest){
	std::string	line;

	std::getline(clientRequest, line);
	std::size_t	lenLine = line.length();
	if (!line.empty() && line[lenLine - 1] == '\r') {
			line.erase(lenLine - 1, 1);
			--lenLine;
			/*---------------------------------------------------------------------------------------ici faire une erreur?*/
		}
	while(line.empty() == false){
		to_lower(line);
		if (clientRequest.eof()){
			_error = ERROR_400;
			return ;
		}
		size_t	posSep = line.find(':');
		if (posSep == std::string::npos || posSep >= lenLine + 2){	//+2 for :	_header[key] = line.substr(posSep + 1);
			_error = ERROR_400;
			return ;
		}
		if (line[posSep + 1] == ' ') {
			line.erase(posSep + 1, 1);
			--lenLine;
		}
		if (line[lenLine - 1] == ' ') {
			line.erase(lenLine - 1, 1);
			--lenLine;
		}
		_header[line.substr(0, posSep)] = line.substr(posSep + 1);
		std::getline(clientRequest, line);
		lenLine = line.length();
		if (!line.empty() && line[lenLine - 1] == '\r') {
			line.erase(line.size() - 1);
			--lenLine;
			/*---------------------------------------------------------------------------------------ici faire une erreur?*/
		}
	}


	//test the validity of the header

	if (_header.find("host") == _header.end()) {
		_error = ERROR_400;
		return ;
	}

	if (_method == "POST"){
		if (_header.find("content-type") == _header.end()){
			_error = ERROR_400;
			return ;
		}
		std::string	TransferEncoding;
		if (_header.find("transfer-encoding") != _header.end()){
			TransferEncoding = _header["transfer-encoding"];
			if (TransferEncoding != "chunked")
				TransferEncoding = "";
		}
		//case no content-lenght and no TransferEncoding "chunked"
		if (_header.find("content-length") == _header.end() && TransferEncoding.empty()){
			_error = ERROR_400;
			return ;
		}
		//case content-lenght + transfer encoding chunked
		if (_header.find("content-length") != _header.end()  && TransferEncoding.empty() == false){
			_error = ERROR_400;
		}
	}
}


void		Request::parsBody(std::istream & clientRequest){
	if (_method != "POST") {
		return ;
	}

	//case of POST method

	//case of content-lenght:
	if (_header.find("content-length") != _header.end()){
		long int nb_char = strtol(_header["content-length"].c_str(), NULL, 10);
		if (errno == ERANGE || nb_char < 0){
			errno = 0;
			_error = ERROR_400;
			return ;
		}
		char buffer[nb_char + 1];
		clientRequest.read(buffer, nb_char);
		buffer[nb_char] = '\0';
		_body = buffer;
		return ;
	}

	//case of transfer encoding "chunked"
	std::string line;
	while (clientRequest.eof() == false) {
		std::getline(clientRequest, line);
		_body += line + "\n";
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
	if (request.getError().empty() == false){
		o << request.getError();
		o << "\n===================" << std::endl;
		return o;
	}
	o << "Method: " << request.getMethod() << "\n";
	o << "URI: " << request.getURI() << "\n";

	o << "\n----- Headers -----" << "\n";
	const std::map<std::string, std::string>& headers = request.getHeader();
	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
		o << it->first << ": " << it->second << "$\n";
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


std::map<std::string, void (Request::*)()> Request::_createMethodMap() {
    std::map<std::string, void (Request::*)()> m;
    m["GET"]    = &Request::handleGET;
    m["POST"]   = &Request::handlePOST;
    m["DELETE"] = &Request::handleDELETE;
    return m;
}

const std::map<std::string, void (Request::*)()> Request::_methodMap = Request::_createMethodMap();
