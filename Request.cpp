#include "Request.hpp"
#include <map>
#include <iostream>

Request::Request(void) : _httpVersion(0){}

std::string Request::getHeader(const std::string& key) const {
	return _header[key];
	

}
