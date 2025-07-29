#include "Request.hpp"

//----------------------------------------------------------------------------------probablement les 3/4 qui ne sont pas utilisé
const std::string& Request::getMethod() const {
    return _method;
}

const std::string& Request::getURI() const {
    return _URI;
}

const std::map<std::string, std::string>& Request::getHeader() const {
    return _header;
}

const std::string Request::getHeader(const std::string& key) {
    return _header[key];
}

const std::string& Request::getBody() const {
    return _body;
}

const std::string& Request::getError() const {
    return _error;
}

std::string Request::getHTTPresponse() {
    return _HTTPresponse.str();
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
