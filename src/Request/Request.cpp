#include "Request.hpp"



Request::Request(void): _HTTPreponse(""){}


void		Request::parsFirstLine(std::string &clientRequest) {
	std::string line;
	size_t nextNewLine = clientRequest.find('\n');
	if (nextNewLine == std::string::npos){
		line = clientRequest;
		clientRequest = "";
	}
	else {
		line = clientRequest.substr(0, nextNewLine);
		clientRequest = clientRequest.substr(nextNewLine);
		if (clientRequest[0] == '\n')
			line.erase(0,1);
	}
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


void		Request::parsHeader(std::string & clientRequest){
	std::string line;
	size_t nextNewLine = clientRequest.find('\n');
	if (nextNewLine == std::string::npos){
		line = clientRequest;
		clientRequest = "";
	}
	else {
		line = clientRequest.substr(0, nextNewLine);
		clientRequest = clientRequest.substr(nextNewLine);
		if (clientRequest[0] == '\n')
			line.erase(0,1);
	}
	std::size_t	lenLine = line.length();
	if (!line.empty() && line[lenLine - 1] == '\r') {
		line.erase(lenLine - 1, 1);
		--lenLine;
		/*---------------------------------------------------------------------------------------ici faire une erreur?*/
	}
	
	std::cout<< "line = [" << line<< "]"<<std::endl;
	while(line.empty() == false){
		to_lower(line);
		//old version
		// if (clientRequest.eof()){
		// 	_error = ERROR_400;
		// 	return ;
		// }
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

		//getline
		nextNewLine = clientRequest.find('\n');
		if (nextNewLine == std::string::npos){
			line = clientRequest;
			clientRequest = "";
		}
		else {
			line = clientRequest.substr(0, nextNewLine);
			clientRequest = clientRequest.substr(nextNewLine);
			if (clientRequest[0] == '\n')
				line.erase(0,1);
		}
		
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


void		Request::parsBody(std::string &clientRequest){
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
		//old version
		// char buffer[nb_char + 1];
		// clientRequest.read(buffer, nb_char);
		// buffer[nb_char] = '\0';
		
		_body = clientRequest.substr(0, nb_char);
		return ;
	}

	//case of transfer encoding "chunked"
	// std::string line;
	// while (clientRequest.eof() == false) {
	// 	std::getline(clientRequest, line);
	// 	_body += line + "\n";
	// }
}

std::string readSocket(int socket_fd, size_t max_size = 0) {
	std::string resultat;
	const size_t BUFFER_SIZE = 4096;
	std::vector<char> buffer(BUFFER_SIZE);
	
	while (true) {
		ssize_t bytes_lus = read(socket_fd, buffer.data(), BUFFER_SIZE);
		
		if (bytes_lus < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				// Socket non-bloquante, pas plus de données disponibles
				break;
			} else {
				throw std::runtime_error("Erreur lors de la lecture de la socket: " + 
									   std::string(strerror(errno)));
			}
		} else if (bytes_lus == 0) {
			// Connexion fermée par le peer
			break;
		} else {
			// Vérifier la limite de taille si spécifiée
			if (max_size > 0 && resultat.size() + bytes_lus > max_size) {
				size_t bytes_a_ajouter = max_size - resultat.size();
				resultat.append(buffer.data(), bytes_a_ajouter);
				break;
			}
			
			resultat.append(buffer.data(), bytes_lus);
		}
	}
	
	return resultat;
}

void		Request::parsRequest(int &socket_fd){

	std::string	clientRequest = readSocket(socket_fd);

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
		//return o;
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
