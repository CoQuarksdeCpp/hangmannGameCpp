#pragma once
#include <string>
#include <vector>

class GameManager {
public:
    GameManager(); // Konstruktor-Deklaration
    std::string getRandomWordFromFile(const std::string& filename);
    int calculateAttempts(const std::string& word);
    bool playAgain();
};
