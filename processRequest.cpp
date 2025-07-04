#include "Request.hpp"
#include "utils.hpp"
#include <ctime>

void Request::handleRequest(void) {
	if (_error.empty() == false){
		return handleError();
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


void	Request::processHeader(std::ostream & client){
	
	// Date: Wed, 21 Oct 2020 07:28:00 GMT
	std::time_t timeUTC = std::time(0);
	std::tm*	timeGMT = gmtime(&timeUTC);

	char buffer[100];
	std::strftime(buffer, sizeof(buffer), "Date: %a, %d %b %Y %H:%M:%S GMT\r", timeGMT);

	client << buffer << std::endl;

	//name
	client << "LE NOM DU SERVEUR !!!!!!!!!!!!!!!!!!!\r" <<std::endl;

	if (_error == ERROR_405){
		client << "Allow:" ;
		for (std::map<std::string, void(Request::*)()>::const_iterator it = _methodMap.begin(); it != _methodMap.end(); ++it){
			client << " " << it->first;
		}
		client << "\r" << std::endl;
	}

	//client << "Connection: ";
}

// Autres header :
// Content-Type: text/html; charset=utf-8
// Content-Length: 5432 ou Transfer-Encoding: chunked


void	Request::handleError(){
	processHeader(std::cout);
	std::cout << "handle Error" <<std::endl;
}

void	Request::handleGET(){
	std::cout << "handle GET" <<std::endl;
}

void	Request::handlePOST(){
	std::cout << "handle POST" <<std::endl;
}

void	Request::handleDELETE(){
	std::cout << "handle DELETE" <<std::endl;
}