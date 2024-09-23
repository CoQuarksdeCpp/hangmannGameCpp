#pragma once
#include <string>
#include <vector>

class DisplayManager {
public:
    void clearScreen();
    int getTerminalWidth();
    void centerText(const std::string& text);
    void showInstructions();
    void blinkMessage(const std::string& message);

    // Neu hinzugefügte Funktionen
    void displayStatus(const std::string& guessedWord, const std::vector<char>& wrongGuesses, int remainingAttempts);
    void blinkCorrectLetters(const std::string& guessedWord, const std::vector<size_t>& correctPositions);
};
