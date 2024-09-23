#include "GameManager.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <limits>
#include <algorithm>
#include <unordered_set>

GameManager::GameManager() {
    // Zufallsgenerator initialisieren
    srand(static_cast<unsigned int>(time(0)));
}

std::string GameManager::getRandomWordFromFile(const std::string& filename) {
    std::vector<std::string> words;
    std::ifstream file(filename);

    // Prüfen, ob die Datei geöffnet werden kann
    if (!file) {
        std::cerr << "Fehler: Die Datei konnte nicht geöffnet werden: " << filename << std::endl;
        return "";
    }

    // Wörter aus der Datei lesen
    std::string word;
    while (file >> word) {
        words.push_back(word);
    }

    // Prüfen, ob Wörter geladen wurden
    if (words.empty()) {
        std::cerr << "Fehler: Keine Wörter in der Datei gefunden." << std::endl;
        return "";
    }

    // Zufälliges Wort aus der Liste auswählen
    size_t randomIndex = rand() % words.size();
    return words[randomIndex];
}

int GameManager::calculateAttempts(const std::string& word) {
    size_t wordLength = word.length();
    std::unordered_set<char> uniqueLetters;

    // Zähle die Anzahl der eindeutigen Buchstaben im Wort
    for (char c : word) {
        char lowerChar = std::tolower(static_cast<unsigned char>(c));
        uniqueLetters.insert(lowerChar);
    }

    // Berechne die Anzahl der Versuche: Basiswert von 5 + 1 für alle 2 Buchstaben + 1 für alle 2 eindeutigen Buchstaben
    int attempts = 5 + (wordLength / 2) + (static_cast<int>(uniqueLetters.size()) / 2);
    return attempts;
}

bool GameManager::playAgain() {
    char choice;
    while (true) {
        std::cout << "Möchten Sie noch einmal spielen? (J/N): ";
        std::cin >> choice;
        choice = std::tolower(static_cast<unsigned char>(choice));

        if (choice == 'j') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return true;
        }
        else if (choice == 'n') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return false;
        }
        else {
            std::cout << "Ungültige Eingabe! Bitte geben Sie J oder N ein." << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}
