#include "Request.hpp"



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
	}
	else {
		_error = ERROR_400;
		return ;
	}

	while(line.empty() == false){
		to_lower(line);
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
		}
		else {
			_error = ERROR_400;
			return ;
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

	//extract Boundary
	{
		if (_header.find("content-type") == _header.end() ){
			_error = ERROR_501;
			return;
		}
		size_t startBoundary = _header["content-type"].find("multipart/form-data; boundary=");
		if (startBoundary == std::string::npos){
			_error = ERROR_501;
			return ;
		}
		startBoundary += 30;
		size_t endBoundary = _header["content-type"].find(";", startBoundary);
		if (endBoundary == std::string::npos)
		{
			_boundary = _header["content-type"].substr(startBoundary);
		}
		else {
			_boundary = _header["content-type"].substr(startBoundary, endBoundary);
		}
	}
	
	//extract len of body
	{
		if (_header.find("content-length") == _header.end() ){
			_error = ERROR_411;
			return;
		}
		errno = 0;
		long int nb_char = strtol(_header["content-length"].c_str(), NULL, 10);
		if (errno == ERANGE || nb_char < 0){
			errno = 0;
			_error = ERROR_400;
			return ;
		}
		_lenBody = nb_char;
		//---------------------------------------------------------------------------------------mettre la limite de charactere
		if (_lenBody > 10000000000000){
			_error = ERROR_413;
			//-----------------------------------------------------------------fermer la connection ici ou dans le handle error
			return ;
		}
	}
	_body = clientRequest;
	if (_body.size() < _lenBody){
		_notEnd = true;
	}
}

void		Request::addBody(std::string clientRequest){
	//check if _boundary is in the good place
	if (clientRequest.find(_boundary) == std::string::npos){
		_error = ERROR_400;
		///-------------------------------------------------------remove connection ici ?
	}

	//check if the end delimiter is present
	if (clientRequest.find((_boundary + "--")))
	{
		_notEnd = false;
	}

	_body += clientRequest;
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