#include "Request.hpp"
#include "utils.hpp"
#include <ctime>

void Request::handleRequest(void) {
	if (_error.empty() == false){
		return handleError(std::cout);
	}
	if (isCGI(*this)){
		int fd[2];

		if (pipe(fd) == -1){
			//-------------------------renvoyer un message d'erreur au client ou redemarer le serveur ?
			;
			return ;
		}
		pid_t pid = fork();

		if (pid < 0){
			//-------------------------renvoyer un message d'erreur au client ou redemarer le serveur ?
			;
			return ;
		}
		if (pid == 0){
			close(fd[1]);
			//-----------------------------securité si URI infiny avec un time of day?
			std::string resultCGI;
			while (1){
				//-------------------------faire un read en non blocant
				if (true)		// quelle condition d'arret?
					break ;
			}
			close(fd[0]);
		}
		else {
			close(fd[0]);

			//call the good methode
			std::map<std::string, void (Request::*)()>::const_iterator it = _methodMap.find(_method);
			(this->*it->second)();


			//free des choses ???????????????????????????
			close(fd[1]);
		}


	}
	std::map<std::string, void (Request::*)()>::const_iterator it = _methodMap.find(_method);
	(this->*it->second)();
}


void	Request::processHeader(){

	// Date: Wed, 21 Oct 2020 07:28:00 GMT
	std::time_t timeUTC = std::time(0);
	std::tm*	timeGMT = gmtime(&timeUTC);

	char buffer[100];
	std::strftime(buffer, sizeof(buffer), "Date: %a, %d %b %Y %H:%M:%S GMT\r", timeGMT);

	_client << buffer << std::endl;

	//name					----------------------------------------------------------------------------------------------------------mettre le vrai nom du server
	_client << "Server: " << "NameOfTheServer !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << "\r\n";

	//-------------------------------------------------------------------------------------------------qu'est ce qu'on fait ?
	// _client << "Connection: keep-alive\r\n";
	// _client << "Keep-Alive: timeout=5, max=100\r" << std::endl;
}

// Autres header :
// Content-Type: text/html; charset=utf-8
// Content-Length: 5432 ou Transfer-Encoding: chunked


void	Request::handleError(){
	_client << "HTTP/1.1 " << _error << "\r" << std::endl;
	processHeader();
	if (_error == ERROR_405){
		_client << "Allow:" ;
		for (std::map<std::string, void(Request::*)()>::const_iterator it = _methodMap.begin(); it != _methodMap.end(); ++it){
			_client << " " << it->first;
		}
		_client << "\r" << std::endl;
	}
	//-------------------------------------------------------------------------------------------si erreur 401 doit faire qqc de special.
	std::cout << "handle Error" <<std::endl;
}

void	Request::handleGET(){
	_client << "handle GET" <<std::endl;
}

void	Request::handlePOST(){
	_client << "handle POST" <<std::endl;
}

void	Request::handleDELETE(){
	_client << "handle DELETE" <<std::endl;
}
