#include <cctype>
#include <string>

std::string to_lower(std::string & input) {
	for (std::string::size_type i = 0; i < input.length(); ++i) {
		input[i] = std::tolower(static_cast<unsigned char>(input[i]));
	}
	return input;
}