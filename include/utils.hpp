#ifndef UTILS_HPP
# define UTILS_HPP


#include <cctype>
#include <string>


class Request;

void	to_lower(std::string & input);

bool	isCGI(Request & request);

#endif