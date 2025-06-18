#include <iostream>
#include <map>
#include <string>

int main() {
    // Déclaration d'une map qui associe un string à un int
    std::map<std::string, std::string> age;

    // Ajout d'éléments
    age["Alice"] = "25";
    age["Bob"] = "30";
    age["Charlie"] = "22";
	age["Test"] = "21";

	const std::string test= "Bob";
    // Accès à un élément
    std::cout << "L'âge de Bob est " << age[test] << " ans." << std::endl;
    std::cout << "L'âge de Bob est " << age[test] << " ans." << std::endl;
    std::cout << "L'âge de Bob est " << age[test] << " ans." << std::endl;
    std::cout << "L'âge de Bob est " << age[test] << " ans." << std::endl;

    // Boucle sur tous les éléments
    for (const auto& pair : age) {
        std::cout << pair.first << " a " << pair.second << " ans." << std::endl;
    }

    return 0;
}
