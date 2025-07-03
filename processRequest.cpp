#include "Request.hpp"
#include "utils.hpp"

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

void	Request::handleError(){
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