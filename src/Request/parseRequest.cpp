#include "Request.hpp"

Request::Request(void): _HTTPresponse(""){}

void		Request::parsFirstLine(std::string &clientRequest) {
	
	std::string line;
	size_t nextNewLine = clientRequest.find('\n');
	if (nextNewLine == std::string::npos){
		line = clientRequest;
		clientRequest = "";
	}
	else {
		line = clientRequest.substr(0, nextNewLine);
		if (clientRequest.length() > nextNewLine + 1)
			clientRequest = clientRequest.substr(nextNewLine + 1);
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
	if (_URI.empty() == true){
		_error =  ERROR_400;
		_error += " No URI";
		return ;
	}
	if (_URI == "/"){
		//------------------------------------------------------------------------------------ici mettre la page par default
		_URI = "pageParDefault.html";
	}
	else if (_URI[0] == '/'){
		_URI.erase(0,1);
	}
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

	//getline
	size_t nextNewLine = clientRequest.find('\n');
	if (nextNewLine == std::string::npos){
		line = clientRequest;
		clientRequest = "";
	}
	else {
		line = clientRequest.substr(0, nextNewLine);
		if (clientRequest.length() > nextNewLine + 1){
			clientRequest = clientRequest.substr(nextNewLine + 1);
		}
	}
	std::size_t	lenLine = line.length();
	if (!line.empty() && line[lenLine - 1] == '\r') {
		line.erase(lenLine - 1, 1);
		--lenLine;
		/*---------------------------------------------------------------------------------------ici faire une erreur?*/
	}

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
			if (clientRequest.length() > nextNewLine + 1)
				clientRequest = clientRequest.substr(nextNewLine + 1);
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
	else {
		//----------------------------------------------------------------------faitparsing
		std::cout << _header["host"] <<std::endl;
	}
}


void		Request::parsBody(std::string &clientRequest){
	if (_method != "POST") {
		return ;
	}

	//case of POST method

	//check body method
	std::string	TransferEncoding;
	if (_header.find("transfer-encoding") != _header.end()){
		TransferEncoding = _header["transfer-encoding"];
	}

	//case no content-lenght and no TransferEncoding "chunked"
	if (_header.find("content-length") == _header.end() && TransferEncoding.empty()){
		_error = ERROR_411;
		return ;
	}

	//case no content-lenght and TransferEncoding != "chunked" (not implement)
	if (/*_header.find("content-length") == _header.end() && */TransferEncoding != "chunked"){
		_error = ERROR_501;
		// _error = "415 Unsupported Media Type";
		return ;
	}

	//case content-lenght + transfer encoding chunked
	if (_header.find("content-length") != _header.end()  && TransferEncoding.empty() == false){
		_error = ERROR_400;
		return ;
	}


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

	else if (TransferEncoding != "chunked"){
		_error = ERROR_501;
	}

	//case of no information of body, consider is empty
	else
		return ;

}

bool Request::readSocketBoundary(int socket_fd) {
	(void)socket_fd;
	return true;
	// //peut-etre gerer le cas d'une config avec un fichier plus petit que 4096 bit?
	// std::vector<char> buffer(BUFFER_SIZE);
	// int bytes_lus = 4096;
	
	// std::cout << GREEN << "Going in" RESET << std::endl;
	// while (bytes_lus == 4096) {
	// 	bytes_lus = read(socket_fd, buffer.data(), BUFFER_SIZE);
		
	// 	if (bytes_lus < 0) {
	// 			throw std::runtime_error("Erreur lors de la lecture de la socket: " + 
	// 								   std::string(strerror(errno)));
	// 	} else if (bytes_lus == 0) {
	// 		std::cout << YELLOW "essaie de deco ?" RESET << std::endl;
	// 		return false;
	// 	} else {
	// 		if (m_processing_request.size() + bytes_lus > max_size) {
	// 			//est-ce necessaire d'ajouter?
	// 			size_t bytes_a_ajouter = max_size - m_processing_request.size();
	// 			m_processing_request.append(buffer.data(), bytes_a_ajouter);
	// 			//----------------------------------------------------------------------------------------gérer l'erreur 413 : fichier trop grand
	// 			//renvoyer l'erreur + fermé la connexion
	// 			std::cout << ERROR_413;
	// 			break;
	// 		}
	// 		m_processing_request.append(buffer.data(), bytes_lus);
	// 	}
	// }
	// if (m_processing_request.find("\r\n\r\n") != std::string::npos)
	// 	return true;
	// return false;
}

void		Request::parsRequest(std::string clientRequest){

	parsFirstLine(clientRequest);
	if (_error.empty() == false)
		return ;

	// std::cout <<"first line ok"<<std::endl;
	parsHeader(clientRequest);
	if (_error.empty() == false)
		return ;
	// std::cout <<"header ok"<<std::endl;
	parsBody(clientRequest);
	// std::cout <<"body end"<<std::endl;
	// if (_error.empty() == false)
	// 	std::cout << "error = " << _error << std::endl;

}