#include "InputManager.h"
#include <iostream>
#include <cctype>

// Wartet, bis der Spieler die Eingabetaste dr�ckt
void InputManager::waitForEnter() {
    char input;
    do {
        input = std::cin.get();
    } while (input != '\n');
}

// �berpr�ft, ob die Eingabe g�ltig ist (ein einzelner Buchstabe)
bool InputManager::isValidInput(const std::string& input) {
    return input.length() == 1 && std::isalpha(input[0]);
}
