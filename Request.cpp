#include "Request.hpp"



Request::Request(void){}


void		Request::parsFirstLine(std::istream & clientRequest) {
	std::string line;
	std::getline(clientRequest, line);
	if (line.empty()){
		_error =  "400 – Bad Request";
		return ;
	}
	size_t	space1 = line.find(' ');
	_method = line.substr(0, space1);
	

	if (_validMethods.find(_method) == _validMethods.end()){
		_error =  "405 – Method Not Allowed";
		return ;
	}

	if (space1 == std::string::npos){
		_error =  "400 – Bad Request";
		return ;
	}
	++space1;
	size_t space2 = line.find(' ', space1);
	_URI = line.substr(space1, space2 - space1);
	
	if (space2 == std::string::npos || _URI.empty()){
		_error =  "400 – Bad Request";
		return ;
	}

	++space2;
	if(line.find(' ', space2) != std::string::npos){
		_error =  "400 – Bad Request";
		return ;
	}

	std::string version = line.substr(space2);
	if (version != "HTTP/1.1" && version != "HTTP/1.1\r")
	{
		_error="505 - HTTP Version Not Supported";
		return ;
	}
}


std::string	to_lower(std::string & input);

void		Request::parsHeader(std::istream & clientRequest){
	std::string	line;

	std::getline(clientRequest, line);
	while(line.empty() == false){
		to_lower(line);
		if (clientRequest.eof()){
			_error = "400 – Bad Request";
			return ;
		}
		size_t	posSep = line.find(':');
		if (posSep == std::string::npos || posSep >= line.length() + 2){	//+2 for :	_header[key] = line.substr(posSep + 1);
			_error = "400 – Bad Request";
			return ;
		}
		std::string value = line.substr(posSep + 1);
		if (!value.empty() && value[0] == ' ') {
			value.erase(0, 1);
		}
		_header[line.substr(0, posSep)] = value;
		if (!line.empty() && line[value.length() - 1] == '\r') {
			value.erase(value.size() - 1, 1);
		}
		std::getline(clientRequest, line);
	}


	//test the validity of the header

	if (_header.find("host") == _header.end()) {
		_error = "400 – Bad Request";
		return ;
	}

	if (_method == "POST"){
		if (_header.find("content-type") == _header.end()){
			_error = "400 – Bad Request";
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
			_error = "400 – Bad Request";
			return ;
		}
		if (_header.find("content-length") != _header.end()  && TransferEncoding.empty() == false){
			_error = "400 – Bad Request";
			return ;
		}
	}
}


void		Request::parsBody(std::istream & clientRequest){
	if (_method != "POST") {
		if (clientRequest.eof() == false)
			_error = "400 – Bad Request";
		return ;
	}

	//case of POST method

	//case of content-lenght:
	if (_header.find("content-length") != _header.end()){
		size_t nb_char = strtol(_header["content-length"].c_str(), NULL, 10);
		if (errno == ERANGE || nb_char < 0){
			errno = 0;
			_error = "400 – Bad Request";
			return ;
		}
		char buffer[nb_char + 1];
		clientRequest.read(buffer, nb_char + 1);
		if (clientRequest.eof() == false){
			_error = "400 – Bad Request";
			return ;
		}
		_body = buffer;
		return ;
	}
	//case of transfer encoding "chunked"
	std::string line;
	std::getline(clientRequest, line);	
}

void		Request::parsRequest(std::istream & clientRequest){

	parsFirstLine(clientRequest);
	if (_error.empty() == false)
		return ;
	std::cout << "first line pass" <<std::endl;
	parsHeader(clientRequest);
	if (_error.empty() == false)
		return ;
	std::cout << "seconde line pass" <<std::endl;

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

std::set<std::string> creatValidMethode(void){
	std::set<std::string> m;
	m.insert("GET");
	m.insert("POST");
	m.insert("DELETE");
	return m;
};

const std::set<std::string> Request::_validMethods = creatValidMethode();