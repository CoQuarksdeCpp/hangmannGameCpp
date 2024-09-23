#pragma once
#include <string>

// Die InputManager-Klasse verwaltet die Eingabe des Spielers
class InputManager {
public:
    // Wartet, bis der Spieler die Eingabetaste dr�ckt
    void waitForEnter();

    // �berpr�ft, ob die Eingabe g�ltig ist (ein einzelner Buchstabe)
    bool isValidInput(const std::string& input);
};
