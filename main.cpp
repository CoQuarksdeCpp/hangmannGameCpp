#define NOMINMAX  // Prevent windows.h from defining min/max macros

#include "Hangman.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cctype>
#include <thread>
#include <chrono>
#include <iomanip>
#include <limits>   // Include limits for numeric_limits

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

// Function to get the width of the terminal window
int getTerminalWidth() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    else {
        columns = 80;  // Default width if unable to get terminal size
    }
    return columns;
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;  // Return terminal width in characters
#endif
}

// Function to center text in the terminal
void centerText(const std::string& text) {
    int terminalWidth = getTerminalWidth();  // Get the width of the terminal
    size_t textLength = text.length();  // Use size_t for textLength to match the return type of std::string::length()
    int padding = (terminalWidth - static_cast<int>(textLength)) / 2;  // Calculate padding for centering
    if (padding > 0) {
        std::cout << std::setw(padding) << "" << text << std::endl;  // Add padding and display the text
    }
    else {
        std::cout << text << std::endl;  // If terminal is smaller than text, just print the text
    }
}

// Function to wait for the player to press the Enter key before proceeding
void waitForEnter() {
    char input;
    do {
        input = std::cin.get();  // Read input from the user
    } while (input != '\n');  // Keep waiting until Enter is pressed
}

// Function to display game instructions
void showInstructions() {
    clearScreen();
    centerText("Welcome to Hangman!\n\n");
    std::cout << "Instructions:\n";
    std::cout << "1. You will be given a word represented by underscores (_), where each underscore represents a letter of the word.\n";
    std::cout << "2. Your goal is to guess the letters in the word.\n";
    std::cout << "3. For each correct letter guessed, the corresponding underscore will be replaced with the correct letter.\n";
    std::cout << "4. If you guess incorrectly, your number of remaining attempts decreases.\n";
    std::cout << "5. The game ends if you guess all the letters (you win) or if you run out of attempts (you lose).\n";
    std::cout << "6. After 3 incorrect guesses, the instructions will be shown again to help you remember the rules.\n"; // New instruction added
    std::cout << "7. You can play again after a game is over or exit.\n";
    std::cout << "Good luck!\n";

    std::cout << "\nPress Enter to continue..." << std::endl;
    waitForEnter();  // Wait for the user to press Enter before proceeding
}

// Function to blink a message twice without clearing the screen
void blinkMessage(const std::string& message) {
    for (int i = 0; i < 2; ++i) {
        std::cout << message << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));  // Wait 500ms
        std::cout << "\r" << std::string(message.length(), ' ') << "\r" << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));  // Wait another 500ms
    }
}

// Function to load a random word from a file
std::string getRandomWordFromFile(const std::string& filename) {
    std::vector<std::string> words;
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Error: Could not open the file: " << filename << std::endl;
        return "";
    }

    std::string word;
    while (file >> word) {
        words.push_back(word);
    }

    if (words.empty()) {
        std::cerr << "Error: No words found in the file." << std::endl;
        return "";
    }

    srand(static_cast<unsigned int>(time(0)));
    int randomIndex = rand() % words.size();

    return words[randomIndex];
}

// Function to count the number of unique letters in a word
size_t countUniqueLetters(const std::string& word) {
    std::string uniqueLetters;
    for (char c : word) {
        char lowerChar = std::tolower(c);
        if (uniqueLetters.find(lowerChar) == std::string::npos) {
            uniqueLetters += lowerChar;
        }
    }
    return uniqueLetters.length();
}

// Function to dynamically calculate the number of attempts based on word length and unique letters
int calculateAttempts(const std::string& word) {
    int wordLength = word.length();               // Get the length of the word
    size_t uniqueLetterCount = countUniqueLetters(word);  // Get the number of unique letters (use size_t)

    // Base attempts: 5 + 1 for every 2 letters in the word + 1 for every 2 unique letters
    int attempts = 5 + (wordLength / 2) + (static_cast<int>(uniqueLetterCount) / 2);

    return attempts;
}

// Function to prompt the player to play again
bool playAgain() {
    char choice;
    while (true) {
        std::cout << "Do you want to play again? (Y/N): ";
        std::cin >> choice;
        choice = std::tolower(choice);

        if (choice == 'y') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return true;
        }
        else if (choice == 'n') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return false;
        }
        else {
            std::cout << "Invalid input! Please enter Y or N." << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

int main() {
    bool continuePlaying = true;
    std::string input;

    std::cout << "Welcome to Hangman! Type '1' to start or '2' to view instructions: ";
    std::getline(std::cin, input);
    if (input == "2") {
        showInstructions();
    }

    while (continuePlaying) {
        std::string word = getRandomWordFromFile("resources/words.txt");
        if (word.empty()) {
            return 1;
        }

        int attempts = calculateAttempts(word);
        Hangman game(word, attempts);

        char guess;
        int correctGuesses = 0;
        int totalGuesses = 0;
        int wrongGuesses = 0;

        while (!game.isGameOver() && !game.isWordGuessed()) {
            clearScreen();
            game.displayStatus();

            if (wrongGuesses == 3) {
                std::cout << "\nIt seems you're having trouble. Here are the instructions again:\n";
                showInstructions();
            }

            std::cout << "Guess a letter: ";
            std::getline(std::cin, input);

            if (input.length() == 1 && std::isalpha(input[0])) {
                guess = std::tolower(input[0]);
                totalGuesses++;

                if (game.guessLetter(guess)) {
                    correctGuesses++;
                    std::cout << "Correct!" << std::endl;
                }
                else {
                    std::cout << "Incorrect!" << std::endl;
                    wrongGuesses++;
                }
            }
            else {
                blinkMessage("Invalid input! Please enter exactly one letter.");
            }

            if (!game.isGameOver() && !game.isWordGuessed()) {
                std::cout << "Press Enter to continue..." << std::endl;
                waitForEnter();
            }
        }

        clearScreen();

        if (game.isWordGuessed()) {
            std::cout << "Congratulations! You have guessed the word \"" << word
                << "\" in " << totalGuesses << " attempts with " << correctGuesses
                << " correct guesses." << std::endl;
        }
        else {
            std::cout << "Game Over! The correct word was: " << word << std::endl;
            std::cout << "In " << totalGuesses << " attempts, you guessed " << correctGuesses
                << " letters correctly." << std::endl;
        }

        continuePlaying = playAgain();
    }

    std::cout << "Thank you for playing! Goodbye!" << std::endl;
    return 0;
}










//#define NOMINMAX  // Prevent windows.h from defining min/max macros
//
//#include "Hangman.h"
//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <string>
//#include <ctime>
//#include <cstdlib>
//#include <cctype>
//#include <thread>
//#include <chrono>
//#include <iomanip>
//#include <limits>   // Include limits for numeric_limits
//
//
//
//// Function to get the width of the terminal window
//int getTerminalWidth() {
//#ifdef _WIN32
//    CONSOLE_SCREEN_BUFFER_INFO csbi;
//    int columns;
//    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
//        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
//    }
//    else {
//        columns = 80;  // Default width if unable to get terminal size
//    }
//    return columns;
//#else
//    struct winsize w;
//    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
//    return w.ws_col;  // Return terminal width in characters
//#endif
//}
//
//// Function to center text in the terminal
//void centerText(const std::string& text) {
//    int terminalWidth = getTerminalWidth();  // Get the width of the terminal
//    size_t textLength = text.length();  // Use size_t for textLength to match the return type of std::string::length()
//    int padding = (terminalWidth - static_cast<int>(textLength)) / 2;  // Calculate padding for centering
//    if (padding > 0) {
//        std::cout << std::setw(padding) << "" << text << std::endl;  // Add padding and display the text
//    }
//    else {
//        std::cout << text << std::endl;  // If terminal is smaller than text, just print the text
//    }
//}
//
//
//// Function to wait for the player to press the Enter key before proceeding
//void waitForEnter() {
//    char input;
//    do {
//        input = std::cin.get();  // Read input from the user
//    } while (input != '\n');  // Keep waiting until Enter is pressed
//}
//
//// Function to display game instructions
//void showInstructions() {
//    clearScreen();
//    centerText("Welcome to Hangman!\n\n");
//    std::cout << "Instructions:\n";
//    std::cout << "1. You will be given a word represented by underscores (_), where each underscore represents a letter of the word.\n";
//    std::cout << "2. Your goal is to guess the letters in the word.\n";
//    std::cout << "3. For each correct letter guessed, the corresponding underscore will be replaced with the correct letter.\n";
//    std::cout << "4. If you guess incorrectly, your number of remaining attempts decreases.\n";
//    std::cout << "5. The game ends if you guess all the letters (you win) or if you run out of attempts (you lose).\n";
//    std::cout << "6. After 3 incorrect guesses, the instructions will be shown again to help you remember the rules.\n"; // New instruction added
//    std::cout << "7. You can play again after a game is over or exit.\n";
//    std::cout << "Good luck!\n";
//
//    std::cout << "\nPress Enter to continue..." << std::endl;
//    waitForEnter();  // Wait for the user to press Enter before proceeding
//}
//
//// Function to blink a message twice without clearing the screen
//void blinkMessage(const std::string& message) {
//    for (int i = 0; i < 2; ++i) {
//        std::cout << message << std::flush;
//        std::this_thread::sleep_for(std::chrono::milliseconds(500));  // Wait 500ms
//        std::cout << "\r" << std::string(message.length(), ' ') << "\r" << std::flush;
//        std::this_thread::sleep_for(std::chrono::milliseconds(500));  // Wait another 500ms
//    }
//}
//
//// Function to load a random word from a file
//std::string getRandomWordFromFile(const std::string& filename) {
//    std::vector<std::string> words;
//    std::ifstream file(filename);
//
//    if (!file) {
//        std::cerr << "Error: Could not open the file: " << filename << std::endl;
//        return "";
//    }
//
//    std::string word;
//    while (file >> word) {
//        words.push_back(word);
//    }
//
//    if (words.empty()) {
//        std::cerr << "Error: No words found in the file." << std::endl;
//        return "";
//    }
//
//    srand(static_cast<unsigned int>(time(0)));
//    int randomIndex = rand() % words.size();
//
//    return words[randomIndex];
//}
//
//// Function to count the number of unique letters in a word
//size_t countUniqueLetters(const std::string& word) {
//    std::string uniqueLetters;
//    for (char c : word) {
//        char lowerChar = std::tolower(c);
//        if (uniqueLetters.find(lowerChar) == std::string::npos) {
//            uniqueLetters += lowerChar;
//        }
//    }
//    return uniqueLetters.length();
//}
//
//// Function to dynamically calculate the number of attempts based on word length and unique letters
//int calculateAttempts(const std::string& word) {
//    int wordLength = word.length();               // Get the length of the word
//    size_t uniqueLetterCount = countUniqueLetters(word);  // Get the number of unique letters (use size_t)
//
//    // Base attempts: 5 + 1 for every 2 letters in the word + 1 for every 2 unique letters
//    int attempts = 5 + (wordLength / 2) + (static_cast<int>(uniqueLetterCount) / 2);  // Cast uniqueLetterCount to int
//
//    return attempts;  // Return the calculated number of attempts
//}
//
//
//// Function to prompt the player to play again
//bool playAgain() {
//    char choice;
//    while (true) {
//        std::cout << "Do you want to play again? (Y/N): ";
//        std::cin >> choice;
//        choice = std::tolower(choice);
//
//        if (choice == 'y') {
//            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//            return true;
//        }
//        else if (choice == 'n') {
//            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//            return false;
//        }
//        else {
//            std::cout << "Invalid input! Please enter Y or N." << std::endl;
//            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//        }
//    }
//}
//
//int main() {
//    bool continuePlaying = true;
//    std::string input;
//
//    std::cout << "Welcome to Hangman! Type '1' to start or '2' to view instructions: ";
//    std::getline(std::cin, input);
//    if (input == "2") {
//        showInstructions();
//    }
//
//    while (continuePlaying) {
//        std::string word = getRandomWordFromFile("resources/words.txt");
//        if (word.empty()) {
//            return 1;
//        }
//
//        int attempts = calculateAttempts(word);
//        Hangman game(word, attempts);
//
//        char guess;
//        int correctGuesses = 0;
//        int totalGuesses = 0;
//        int wrongGuesses = 0;
//
//        while (!game.isGameOver() && !game.isWordGuessed()) {
//            clearScreen();
//            game.displayStatus();
//
//            if (wrongGuesses == 3) {
//                std::cout << "\nIt seems you're having trouble. Here are the instructions again:\n";
//                showInstructions();
//            }
//
//            std::cout << "Guess a letter: ";
//            std::getline(std::cin, input);
//
//            if (input.length() == 1 && std::isalpha(input[0])) {
//                guess = std::tolower(input[0]);
//                totalGuesses++;
//
//                if (game.guessLetter(guess)) {
//                    correctGuesses++;
//                    std::cout << "Correct!" << std::endl;
//                }
//                else {
//                    std::cout << "Incorrect!" << std::endl;
//                    wrongGuesses++;
//                }
//            }
//            else {
//                blinkMessage("Invalid input! Please enter exactly one letter.");
//            }
//
//            if (!game.isGameOver() && !game.isWordGuessed()) {
//                std::cout << "Press Enter to continue..." << std::endl;
//                waitForEnter();
//            }
//        }
//
//        clearScreen();
//
//        if (game.isWordGuessed()) {
//            std::cout << "Congratulations! You have guessed the word \"" << word
//                << "\" in " << totalGuesses << " attempts with " << correctGuesses
//                << " correct guesses." << std::endl;
//        }
//        else {
//            std::cout << "Game Over! The correct word was: " << word << std::endl;
//            std::cout << "In " << totalGuesses << " attempts, you guessed " << correctGuesses
//                << " letters correctly." << std::endl;
//        }
//
//        continuePlaying = playAgain();
//    }
//
//    std::cout << "Thank you for playing! Goodbye!" << std::endl;
//    return 0;
//}
