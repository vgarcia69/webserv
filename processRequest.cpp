#include "Request.hpp"
#include "utils.hpp"

void Request::handleRequest(void) {
	if (_error.empty() == false){
		return handleError();
	}
	if (isCGI(*this)){
		//faire un fork avec un pipe;
		;
	}
	std::map<std::string, void (Request::*)()>::const_iterator it = _methodMap.find(_method);
	(this->*it->second)();
}

