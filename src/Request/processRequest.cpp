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


void	Request::processHeader(){

	// Date: Wed, 21 Oct 2020 07:28:00 GMT
	std::time_t timeUTC = std::time(0);
	std::tm*	timeGMT = gmtime(&timeUTC);

	char buffer[100];
	std::strftime(buffer, sizeof(buffer), "Date: %a, %d %b %Y %H:%M:%S GMT\r\n", timeGMT);

	_HTTPreponse << buffer;

	//name					----------------------------------------------------------------------------------------------------------mettre le vrai nom du server
	_HTTPreponse << "Server: " << "NameOfTheServer !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << "\r\n";
	_HTTPreponse << "Connexion: close\r\n";
	//-------------------------------------------------------------------------------------------------qu'est ce qu'on fait ?
	// _HTTPreponse << "Connection: keep-alive\r\n";
	// _HTTPreponse << "Keep-Alive: timeout=5, max=100\r\n";
	
}

// Autres header :
// Content-Type: text/html; charset=utf-8
// Content-Length: 5432 ou Transfer-Encoding: chunked


void	Request::handleError(){
	_HTTPreponse << "HTTP/1.1 " << _error << "\r\n";
	processHeader();
	if (_error == ERROR_405){
		_HTTPreponse << "Allow:" ;
		for (std::map<std::string, void(Request::*)()>::const_iterator it = _methodMap.begin(); it != _methodMap.end(); ++it){
			_HTTPreponse << " " << it->first;
		}
		_HTTPreponse << "\r\n\r\n";
	}
	//-------------------------------------------------------------------------------------------si erreur 401 doit faire qqc de special.
	std::cout << "handle Error/n";
}

void	Request::handleGET(){
	std::ifstream	file(_URI.c_str());
	if (!file){
		_HTTPreponse << "HTTP/1.1 " << ERROR_404 <<  "\r\n";
		processHeader();
		_HTTPreponse <<  "Content-Type: text/html; charset=UTF-8\r\n";
		std::string page;
		page += "<html>\r\n";
		page += "<head><title>404 Not Found</title></head>\r\n";
		page += "	<body>\r\n";
		page += "		<h1>404 Not Found</h1>\r\n";
		page += "		<p>La page demandée est introuvable.</p>\r\n";
		page += "	</body>\r\n";
		page += "</html>\r\n";
		page += "\r\n";
		_HTTPreponse <<  "Content-Length: " <<  page.length() <<  "\r\n\r\n";
		_HTTPreponse <<  page;
		return ;
	}
	
	std::string page;
	std::string line;
	
	while (std::getline(file, line)) {
		page += line;
		page += '\n';
	}

	_HTTPreponse <<  "HTTP/1.1 200 OK\r\n";
	processHeader();
	_HTTPreponse <<  "Content-Length: " <<  page.length() <<  "\r\n\r\n";
	_HTTPreponse <<  page;
}

void	Request::handlePOST(){
	_HTTPreponse <<  "handle POST\n";
}

void	Request::handleDELETE(){
	_HTTPreponse <<  "handle DELETE\n";
}
