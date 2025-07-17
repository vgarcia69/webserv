#include "Request.hpp"
#include <iostream>
#include <fstream>



int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <request_file1> [request_file2 ...]" << std::endl;
		return 1;
	}

	// Parcourir tous les fichiers passés en argument
	for (int i = 1; i < argc; i++) {
		std::cout << "\nParsing file: " << argv[i] << std::endl;
		std::cout << "----------------------------------------" << std::endl;

		// Ouvrir le fichier ----------------------------------------------ici la socket
		std::ifstream file(argv[i]);
		if (!file.is_open()) {
			std::cerr << "Error: Cannot open file " << argv[i] << std::endl;
			continue;
		}

		// Créer et parser la requête
		Request request;
		request.parsRequest(file);

		std::cout << request;

		file.close();
		std::cout << "before handle----------------------------------------\n" << std::endl;
		request.handleRequest();
		//--------------------------------------------------------ici envoyer dans la socket
		std::cerr << request.getHTTPreponse();
		std::cout << "end handle----------------------------------------\n" << std::endl;
	}

	return 0;
}
