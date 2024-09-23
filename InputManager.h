#pragma once
#include <string>

// Die InputManager-Klasse verwaltet die Eingabe des Spielers
class InputManager {
public:
    // Wartet, bis der Spieler die Eingabetaste drückt
    void waitForEnter();

    // Überprüft, ob die Eingabe gültig ist (ein einzelner Buchstabe)
    bool isValidInput(const std::string& input);
};
