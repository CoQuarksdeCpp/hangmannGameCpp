#include "GameManager.h"
#include "DisplayManager.h"
#include "InputManager.h"
#include "Hangman.h"
#include "LanguageManager.h"
#include <iostream>
#include <string>

int main() {
    LanguageManager languageManager("languages.xml");

    std::string input;
    std::cout << languageManager.getText("choose_language");
    std::getline(std::cin, input);

    if (input == "en" || input == "de") {
        languageManager.setLanguage(input);
    }
    else {
        std::cout << languageManager.getText("invalid_language") << std::endl;
        return 1;
    }

    GameManager gameManager;
    DisplayManager displayManager;
    InputManager inputManager;
    bool continuePlaying = true;

    // Hauptmenü: Der Spieler wählt, ob er das Spiel starten oder die Anweisungen sehen möchte
    while (true) {
        std::cout << languageManager.getText("welcome");
        std::getline(std::cin, input);

        if (input == "2") {
            displayManager.showInstructions();
            std::cout << languageManager.getText("instructions");
            inputManager.waitForEnter();  // Warte, bis der Spieler die Eingabetaste drückt
        }
        else if (input == "1") {
            break;  // Startet das Spiel, wenn der Spieler "1" auswählt
        }
        else {
            std::cout << languageManager.getText("invalid_option") << std::endl;
        }
    }

    // Spielschleife
    while (continuePlaying) {
        std::string word = gameManager.getRandomWordFromFile("resources/words.txt");
        if (word.empty()) {
            return 1;
        }

        int attempts = gameManager.calculateAttempts(word);
        Hangman game(word, attempts);

        char guess;
        int correctGuesses = 0;
        int totalGuesses = 0;
        int wrongGuesses = 0;

        while (!game.isGameOver() && !game.isWordGuessed()) {
            displayManager.clearScreen();
            displayManager.displayStatus(game.getGuessedWord(), game.getWrongGuesses(), game.getRemainingAttempts());

            if (wrongGuesses == 3) {
                std::cout << "\n" << languageManager.getText("instructions") << std::endl;
                displayManager.showInstructions();
            }

            std::cout << languageManager.getText("guess_letter");
            std::getline(std::cin, input);

            if (inputManager.isValidInput(input)) {
                guess = std::tolower(input[0]);
                totalGuesses++;

                if (game.guessLetter(guess)) {
                    correctGuesses++;
                    std::cout << languageManager.getText("correct") << std::endl;
                }
                else {
                    std::cout << languageManager.getText("incorrect") << std::endl;
                    wrongGuesses++;
                }
            }
            else {
                displayManager.blinkMessage(languageManager.getText("invalid_input"));
            }

            if (!game.isGameOver() && !game.isWordGuessed()) {
                std::cout << languageManager.getText("continue") << std::endl;
                inputManager.waitForEnter();
            }
        }

        displayManager.clearScreen();

        if (game.isWordGuessed()) {
            std::cout << languageManager.getText("congratulations") << " \"" << word
                << "\" in " << totalGuesses << " attempts with " << correctGuesses
                << " correct guesses." << std::endl;
        }
        else {
            std::cout << languageManager.getText("game_over") << " " << word << std::endl;
            std::cout << "In " << totalGuesses << " attempts, you guessed " << correctGuesses
                << " letters correctly." << std::endl;
        }

        continuePlaying = gameManager.playAgain();
    }

    std::cout << languageManager.getText("thank_you") << std::endl;
    return 0;
}
