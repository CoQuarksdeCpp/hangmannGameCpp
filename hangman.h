#pragma once


#include <string>
#include <vector>

// Hangman class handles the core game logic
class Hangman {
private:
    std::string wordToGuess;   // The word that the player needs to guess
    std::string guessedWord;   // The current state of the guessed word (with underscores for unguessed letters)
    std::vector<char> wrongGuesses;  // Stores the player's incorrect guesses
    int remainingAttempts;     // Number of attempts left before the player loses

public:
    // Constructor: Initializes the game with the word to guess and the number of attempts
    Hangman(const std::string& word, int attempts);

    // Displays the current status of the game (word state, wrong guesses, remaining attempts)
    void displayStatus() const;

    // Processes a guessed letter and updates the game state
    bool guessLetter(char letter);

    // Checks if the game is over (no more attempts)
    bool isGameOver() const;

    // Checks if the player has guessed the entire word
    bool isWordGuessed() const;

    // Blinks the correct letters when guessed (for visual feedback)
    void blinkCorrectLetters(char letter) const;
};

// Clears the terminal screen (for Windows and Unix-based systems)
void clearScreen();     // Function declaration

