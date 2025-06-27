#include "Config.hpp"

static ParsingState isServerFound(std::string& currentWord)
{
	std::string serverKeyword = "server";

	for (unsigned i = 0; i < serverKeyword.size(); i++)
	{
		if (!currentWord[i] || serverKeyword[i] != currentWord[i])
			return ERROR;
	}
	currentWord = currentWord.substr(6);
	return LOOKING_FOR_SERVER_BLOCK;
}

ParsingState	lookingForServerCall(std::fstream& infile, std::string& currentLine)
{
	unsigned	i = 0;

	while (std::getline(infile, currentLine))
	{
		while (iswblank(currentLine[i]) && i < currentLine.size())
		{
			i++;
		}
		if (currentLine[i])
		{
			currentLine = currentLine.substr(i);
			return isServerFound(currentLine);
		}
	}
	return ERROR;
}

void	lookingForBracket(std::fstream& infile, std::string& currentLine)
{
	unsigned	i = 0;

	while (!infile.eof())
	{
		while (iswblank(currentLine[i]) && i < currentLine.size())
		{
			i++;
		}
		if (currentLine[i])
		{
			if (currentLine[i] == '{')
			{
				currentLine = currentLine.substr(i + 1);
				return ;
			}
			else
			{
				throw std::runtime_error("Was expecting bracket");
			}
		}
		std::getline(infile, currentLine);
	}
	throw std::runtime_error("File Format Error");
}
