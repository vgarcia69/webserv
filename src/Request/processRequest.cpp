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
	else {
		std::map<std::string, void (Request::*)()>::const_iterator it = _methodMap.find(_method);
		(this->*it->second)();
	}
}

void	Request::processHeader(){

	// Date: Wed, 21 Oct 2020 07:28:00 GMT
	std::time_t timeUTC = std::time(0);
	std::tm*	timeGMT = gmtime(&timeUTC);

	char buffer[100];
	std::strftime(buffer, sizeof(buffer), "Date: %a, %d %b %Y %H:%M:%S GMT\r\n", timeGMT);

	_HTTPresponse << buffer;

	//name					---------------------------------------------------------------------------------------------------------- mettre le vrai nom du server
	_HTTPresponse << "Server: " << "NameOfTheServer !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << "\r\n";


	if (_error == ERROR_413){
		//------------------------------------------------------------------------------------------------- fermer la connexion
		_HTTPresponse << "Connection: close\r\n";
	}
	else{
		//------------------------------------------------------------------------------------------------- chercher les info du serveur;
		;
		// _HTTPreponse << "Connection: keep-alive\r\n";
		// _HTTPreponse << "Keep-Alive: timeout=5, max=100\r\n";
	}
	
}

// Autres header :
// Content-Type: text/html; charset=utf-8
// Content-Length: 5432 ou Transfer-Encoding: chunked


void	Request::handleError(){
	_HTTPresponse << "HTTP/1.1 " << _error << "\r\n";
	processHeader();
	if (_error == ERROR_405){
		_HTTPresponse << "Allow:" ;
		for (std::map<std::string, void(Request::*)()>::const_iterator it = _methodMap.begin(); it != _methodMap.end(); ++it){
			_HTTPresponse << " " << it->first;
		}
		_HTTPresponse << "\r\n\r\n";
	}
	//-------------------------------------------------------------------------------------------si erreur 401 doit faire qqc de special.
}

void	Request::handleGET(){
	std::ifstream	file(_URI.c_str());
	if (!file){
		_HTTPresponse << "HTTP/1.1 " << ERROR_404 <<  "\r\n";
		processHeader();
		_HTTPresponse <<  "Content-Type: text/html; charset=UTF-8\r\n";
		std::string page;
		page += "<html>\r\n";
		page += "<head><title>404 Not Found</title></head>\r\n";
		page += "	<body>\r\n";
		page += "		<h1>404 Not Found</h1>\r\n";
		page += "		<p>La page demandée est introuvable.</p>\r\n";
		page += "	</body>\r\n";
		page += "</html>\r\n";
		page += "\r\n";
		_HTTPresponse <<  "Content-Length: " <<  page.length() <<  "\r\n\r\n";
		_HTTPresponse <<  page;
		return ;
	}
	
	std::string page;
	std::string line;
	
	while (std::getline(file, line)) {
		page += line;
		page += '\n';
	}

	_HTTPresponse << "HTTP/1.1 200 OK\r\n";
	processHeader();
	_HTTPresponse << "Content-Length: " << page.length() << "\r\n\r\n";
	_HTTPresponse << page;
}


// 204 Aucune Maudification
//200 ou 201 : doit renvoyer un corp (soit la ressource chargé, soit la une page)

void	Request::handlePOST(){
	
	//find filename
	size_t	startFilename = _body.find("filename=\"");
	size_t endFilename = _body.find("\"\r\n", startFilename + 10);
	if (startFilename == std::string::npos || endFilename == std::string::npos){	
		_error = ERROR_400;
		return (handleError());
	}
	startFilename += 10; //lenght of filname="
	std::string NameFile = _body.substr(startFilename, endFilename - startFilename);
	if (NameFile.empty()){	
		_error = ERROR_400;
		return (handleError());
	}

	std::cout << "\n\n\n\n\nici : [" << NameFile  << "]\n\n\n\n\n\n\n\n\n\n\n" << startFilename << " - " << endFilename << std::endl;


	size_t endHeader = _body.find("\r\n\r\n");
	if (endHeader == std::string::npos){	
		_error = ERROR_400;
		return (handleError());
	}
	std::ofstream file(NameFile.c_str());
	file << _body.substr(endHeader);
	
	
	//si 201 Created, inclure le Location: URI

	_HTTPresponse << "HTTP/1.1 204 No Content\r\n";
	processHeader();
	//----------------------------------------------------------------- avec 201 DOIT indiquer l'URI du fichier créer
	// _HTTPresponse << "Location: " << "127.0.0.1:8003/" << _URI <<"\r\n";
	_HTTPresponse << "\r\n";
}

void	Request::handleDELETE(){
	if (std::remove(_URI.c_str()) == 0) {
		_HTTPresponse << "HTTP/1.1 200 OK\r\n";
		processHeader();
		_HTTPresponse << "\r\n";
		return ;
	}
	
	switch (errno) {
		case ENOENT:
			_HTTPresponse << "HTTP/1.1 404 Not Found\r\n";
			break ;
		case EACCES:
			_HTTPresponse << "HTTP/1.1 403 Forbidden\r\n";
			break ;
		case EBUSY:
			_HTTPresponse << "HTTP/1.1 409 Conflict\r\n";
			break ;
		case EROFS:
			_HTTPresponse << "HTTP/1.1 403 Forbidden\r\n";
			break ;
		default:
			_HTTPresponse << "HTTP/1.1 500 Internal Server Error\r\n";
			break ;
	}
	processHeader();
}