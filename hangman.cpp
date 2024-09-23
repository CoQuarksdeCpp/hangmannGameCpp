#include "Hangman.h"
#include <iostream>
#include "DisplayManager.h"
#include <algorithm>
#include <thread>
#include <chrono>
#include <unordered_set>

Hangman::Hangman(const std::string& word, int attempts)
    : wordToGuess(word), remainingAttempts(attempts) {
    if (word.empty()) {
        throw std::invalid_argument("Das zu erratende Wort darf nicht leer sein.");
    }
    if (attempts < 0) {
        throw std::invalid_argument("Die Anzahl der Versuche darf nicht negativ sein.");
    }
    guessedWord = std::string(word.length(), '_');
}

bool Hangman::guessLetter(char letter) {
    bool found = false;

    if (std::find(wrongGuesses.begin(), wrongGuesses.end(), letter) != wrongGuesses.end() ||
        std::find(guessedWord.begin(), guessedWord.end(), letter) != guessedWord.end()) {
        std::cout << "Sie haben diesen Buchstaben bereits erraten!" << std::endl;
        return false;
    }

    for (size_t i = 0; i < wordToGuess.length(); ++i) {
        if (tolower(static_cast<unsigned char>(wordToGuess[i])) == tolower(static_cast<unsigned char>(letter))) {
            guessedWord[i] = wordToGuess[i];
            found = true;
        }
    }

    if (found) {
        blinkCorrectLetters(letter);
    }
    else {
        wrongGuesses.push_back(letter);
        remainingAttempts--;
    }

    return found;
}

bool Hangman::isGameOver() const {
    return remainingAttempts <= 0;
}

bool Hangman::isWordGuessed() const {
    return guessedWord == wordToGuess;
}

void Hangman::blinkCorrectLetters(char letter) const {
    std::vector<size_t> correctPositions;

    for (size_t i = 0; i < wordToGuess.length(); ++i) {
        if (tolower(static_cast<unsigned char>(wordToGuess[i])) == tolower(static_cast<unsigned char>(letter))) {
            correctPositions.push_back(i);
        }
    }

    DisplayManager displayManager; // Neue Instanz für visuelle Effekte
    displayManager.blinkCorrectLetters(guessedWord, correctPositions);
}

size_t Hangman::calculateAttempts(const std::string& word) {
    size_t wordLength = word.length();
    std::unordered_set<char> uniqueLetters;

    // Zähle die Anzahl der eindeutigen Buchstaben im Wort
    for (char c : word) {
        char lowerChar = std::tolower(static_cast<unsigned char>(c));
        uniqueLetters.insert(lowerChar);
    }

    // Berechne die Anzahl der Versuche: Basiswert von 5 + 1 für alle 2 Buchstaben + 1 für alle 2 eindeutigen Buchstaben
    size_t attempts = 5 + (wordLength / 2) + (uniqueLetters.size() / 2);
    return attempts;
}
