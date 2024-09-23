#include "Hangman.h"
#include <iostream>
#include <algorithm>  // For std::find
#include <thread>     // For sleep_for (used in blinking effect)
#include <chrono>     // For time durations in milliseconds

// Function to clear the terminal screen based on the operating system
void clearScreen() {
#ifdef _WIN32
    system("cls"); // Clear screen command for Windows
#else
    system("clear"); // Clear screen command for Linux and macOS
#endif
}

// Constructor for the Hangman class
// Initializes the word to be guessed and the number of remaining attempts
Hangman::Hangman(const std::string& word, int attempts)
    : wordToGuess(word), remainingAttempts(attempts) {
    // Initialize the guessedWord with underscores representing the letters of the word
    guessedWord = std::string(word.length(), '_');
}

// Display the current status of the game
// Shows the guessed letters, underscores for unguessed letters, wrong guesses, and remaining attempts
void Hangman::displayStatus() const {
    std::cout << "Word: ";
    for (char c : guessedWord) {
        std::cout << c << ' ';  // Add a space between each character for better readability
    }
    std::cout << std::endl;

    // Display incorrect guesses in red using ANSI escape codes
    std::cout << "Incorrect guesses: \033[31m";  // Start red text
    for (char c : wrongGuesses) {
        std::cout << c << ' ';  // Display each wrong guess separated by a space
    }
    std::cout << "\033[0m";  // Reset the text color to default

    // Display the remaining attempts
    std::cout << "\nRemaining attempts: " << remainingAttempts << std::endl;
}

// Function to create a blinking effect for the correctly guessed letters
// Alternates between showing an underscore and the correct letter in green
void Hangman::blinkCorrectLetters(char letter) const {
    std::vector<size_t> correctPositions;

    // Find all positions of the guessed letter in the word
    for (size_t i = 0; i < wordToGuess.length(); ++i) {
        if (tolower(wordToGuess[i]) == tolower(letter)) {
            correctPositions.push_back(i);  // Store the index of the correct letter
        }
    }

    // Blink the correct letters twice
    for (int i = 0; i < 2; ++i) {
        clearScreen();  // Clear the screen before each blink

        // First blink: Show underscores in green at the correct positions
        std::cout << "Word: ";
        for (size_t j = 0; j < guessedWord.length(); ++j) {
            if (std::find(correctPositions.begin(), correctPositions.end(), j) != correctPositions.end()) {
                std::cout << "\033[32m_ \033[0m";  // Show underscore in green where the correct letter is located
            }
            else {
                std::cout << guessedWord[j] << ' ';  // Show the already guessed letters or underscores for unguessed ones
            }
        }
        std::cout << std::endl;

        // Wait for 500 milliseconds before the second blink
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        clearScreen();  // Clear the screen again before showing the letters

        // Second blink: Show the correct guessed letters in green
        std::cout << "Word: ";
        for (size_t j = 0; j < guessedWord.length(); ++j) {
            if (std::find(correctPositions.begin(), correctPositions.end(), j) != correctPositions.end()) {
                std::cout << "\033[32m" << wordToGuess[j] << "\033[0m ";
            }
            else {
                std::cout << guessedWord[j] << ' ';
            }
        }
        std::cout << std::endl;

        // Wait for 500 milliseconds before continuing
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

// Process the guessed letter and update the game state
// Returns true if the letter is correct, false otherwise
bool Hangman::guessLetter(char letter) {
    bool found = false;

    // Check if the letter has already been guessed (either correctly or incorrectly)
    if (std::find(wrongGuesses.begin(), wrongGuesses.end(), letter) != wrongGuesses.end() ||
        std::find(guessedWord.begin(), guessedWord.end(), letter) != guessedWord.end()) {
        std::cout << "You've already guessed that letter!" << std::endl;
        return false;
    }

    // Iterate through the word to find any occurrences of the guessed letter
    for (size_t i = 0; i < wordToGuess.length(); ++i) {
        if (tolower(wordToGuess[i]) == tolower(letter)) {
            guessedWord[i] = wordToGuess[i];  // Correct letter found, reveal it in the guessedWord
            found = true;
        }
    }

    if (found) {
        blinkCorrectLetters(letter);  // Blink the correct letters
    }
    else {
        wrongGuesses.push_back(letter);  // Add incorrect guess to the list and reduce attempts
        remainingAttempts--;
    }

    return found;  // Return whether the guess was correct or not
}

// Check if the game is over (i.e., no remaining attempts)
bool Hangman::isGameOver() const {
    return remainingAttempts <= 0;
}

// Check if the word has been fully guessed
bool Hangman::isWordGuessed() const {
    return guessedWord == wordToGuess;  // Return true if all letters are revealed
}























//#include "Hangman.h"
//#include <iostream>
//#include <algorithm>  // For std::find
//#include <thread>     // For sleep_for (used in blinking effect)
//#include <chrono>     // For time durations in milliseconds
//
//// Function to clear the terminal screen based on the operating system
//void clearScreen() {
//#ifdef _WIN32
//    system("cls"); // Clear screen command for Windows
//#else
//    system("clear"); // Clear screen command for Linux and macOS
//#endif
//}
//
//// Constructor for the Hangman class
//// Initializes the word to be guessed and the number of remaining attempts
//Hangman::Hangman(const std::string& word, int attempts)
//    : wordToGuess(word), remainingAttempts(attempts) {
//    // Initialize the guessedWord with underscores representing the letters of the word
//    guessedWord = std::string(word.length(), '_');
//}
//
//// Display the current status of the game
//// Shows the guessed letters, underscores for unguessed letters, wrong guesses, and remaining attempts
//void Hangman::displayStatus() const {
//    // Display the current guessed word with underscores for unguessed letters
//    std::cout << "Word: ";
//    for (char c : guessedWord) {
//        std::cout << c << ' ';  // Add a space between each character for better readability
//    }
//    std::cout << std::endl;
//
//    // Display incorrect guesses in red using ANSI escape codes
//    std::cout << "Incorrect guesses: \033[31m";  // Start red text
//    for (char c : wrongGuesses) {
//        std::cout << c << ' ';  // Display each wrong guess separated by a space
//    }
//    std::cout << "\033[0m";  // Reset the text color to default
//
//    // Display the remaining attempts
//    std::cout << "\nRemaining attempts: " << remainingAttempts << std::endl;
//}
//
//// Function to create a blinking effect for the correctly guessed letters
//// Alternates between showing an underscore and the correct letter in green
//void Hangman::blinkCorrectLetters(char letter) const {
//    std::vector<size_t> correctPositions;
//
//    // Find all positions of the guessed letter in the word
//    for (size_t i = 0; i < wordToGuess.length(); ++i) {
//        if (tolower(wordToGuess[i]) == tolower(letter)) {
//            correctPositions.push_back(i);  // Store the index of the correct letter
//        }
//    }
//
//    // Blink the correct letters twice
//    for (int i = 0; i < 2; ++i) {
//        clearScreen();  // Clear the screen before each blink
//
//        // First blink: Show underscores in green at the correct positions
//        std::cout << "Word: ";
//        for (size_t j = 0; j < guessedWord.length(); ++j) {
//            if (std::find(correctPositions.begin(), correctPositions.end(), j) != correctPositions.end()) {
//                // Show underscore in green where the correct letter is located
//                std::cout << "\033[32m_ \033[0m";
//            }
//            else {
//                // Show the already guessed letters or underscores for unguessed ones
//                std::cout << guessedWord[j] << ' ';
//            }
//        }
//        std::cout << std::endl;
//
//        // Wait for 500 milliseconds before the second blink
//        std::this_thread::sleep_for(std::chrono::milliseconds(500));
//
//        clearScreen();  // Clear the screen again before showing the letters
//
//        // Second blink: Show the correct guessed letters in green
//        std::cout << "Word: ";
//        for (size_t j = 0; j < guessedWord.length(); ++j) {
//            if (std::find(correctPositions.begin(), correctPositions.end(), j) != correctPositions.end()) {
//                // Show the correct letter in green
//                std::cout << "\033[32m" << wordToGuess[j] << "\033[0m ";
//            }
//            else {
//                // Show the already guessed letters or underscores for unguessed ones
//                std::cout << guessedWord[j] << ' ';
//            }
//        }
//        std::cout << std::endl;
//
//        // Wait for 500 milliseconds before continuing
//        std::this_thread::sleep_for(std::chrono::milliseconds(500));
//    }
//}
//
//// Process the guessed letter and update the game state
//// Returns true if the letter is correct, false otherwise
//bool Hangman::guessLetter(char letter) {
//    bool found = false;  // Flag to track if the guessed letter is correct
//
//    // Check if the letter has already been guessed (either correctly or incorrectly)
//    if (std::find(wrongGuesses.begin(), wrongGuesses.end(), letter) != wrongGuesses.end() ||
//        std::find(guessedWord.begin(), guessedWord.end(), letter) != guessedWord.end()) {
//        // If the letter has already been guessed, show a message and return false
//        std::cout << "You've already guessed that letter!" << std::endl;
//        return false;
//    }
//
//    // Iterate through the word to find any occurrences of the guessed letter
//    for (size_t i = 0; i < wordToGuess.length(); ++i) {
//        if (tolower(wordToGuess[i]) == tolower(letter)) {
//            // If the guessed letter is correct, reveal it in the guessedWord
//            guessedWord[i] = wordToGuess[i];
//            found = true;  // Set the flag to true, indicating a correct guess
//        }
//    }
//
//    if (found) {
//        // If the letter is correct, blink the correct positions in the word
//        blinkCorrectLetters(letter);
//    }
//    else {
//        // If the letter is incorrect, add it to the wrong guesses and reduce attempts
//        wrongGuesses.push_back(letter);
//        remainingAttempts--;
//    }
//
//    return found;  // Return whether the guess was correct or not
//}
//
//// Check if the game is over (i.e., no remaining attempts)
//bool Hangman::isGameOver() const {
//    return remainingAttempts <= 0;
//}
//
//// Check if the word has been fully guessed
//bool Hangman::isWordGuessed() const {
//    return guessedWord == wordToGuess;  // Return true if all letters are revealed
//}
//
