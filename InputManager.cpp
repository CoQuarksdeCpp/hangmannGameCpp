#include "InputManager.h"
#include <iostream>
#include <cctype>

// Wartet, bis der Spieler die Eingabetaste drückt
void InputManager::waitForEnter() {
    char input;
    do {
        input = std::cin.get();
    } while (input != '\n');
}

// Überprüft, ob die Eingabe gültig ist (ein einzelner Buchstabe)
bool InputManager::isValidInput(const std::string& input) {
    return input.length() == 1 && std::isalpha(input[0]);
}
