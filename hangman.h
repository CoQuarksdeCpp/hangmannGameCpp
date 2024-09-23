#pragma once

/*
* If the compiler does not support '#pragma once', the following two lines
* can be used to implement an "Include Guard", which prevents multiple 
* inclusions of this header file.
* Include Guards ensure that the contents of the file are included only once during compilation.
* This used to be the standard way to prevent multiple inclusions before '#pragma once' became popular.

#ifndef HANGMAN_H
#define HANGMAN_H
*/

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
void clearScreen();

/*
* The #endif directive marks the end of the Include Guard.
* It pairs with the #ifndef at the top of the file to ensure
* that the header file's contents are only included once during compilation.
* Without this closing #endif, the Include Guard would not function correctly.

#endif // HANGMAN_H
*/





//#include <string>
//#include <vector>
//
//class Hangman {
//private:
//    std::string wordToGuess;
//    std::string guessedWord;
//    std::vector<char> wrongGuesses;
//    int remainingAttempts;
//
//public:
//    Hangman(const std::string& word, int attempts);  // Constructor that accepts word and attempts
//    void displayStatus() const;
//    bool guessLetter(char letter);
//    bool isGameOver() const;
//    bool isWordGuessed() const;
//
//    void blinkCorrectLetters(char letter) const;
//};
//
//void clearScreen();


/*
* The #endif directive marks the end of the Include Guard.
* It pairs with the #ifndef at the top of the file to ensure
* that the header file's contents are only included once during compilation.
* Without this closing #endif, the Include Guard would not function correctly.

#endif // HANGMAN_H
*/
