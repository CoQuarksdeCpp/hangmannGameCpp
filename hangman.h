#pragma once
#include <string>
#include <vector>

// Die Hangman-Klasse verwaltet die Hauptlogik des Spiels
class Hangman {

private:
    std::string wordToGuess;   // Das Wort, das der Spieler erraten muss
    std::string guessedWord;   // Der aktuelle Stand des erratenen Wortes (mit Unterstrichen für nicht erratene Buchstaben)
    std::vector<char> wrongGuesses;  // Speichert die falschen Vermutungen des Spielers
    int remainingAttempts;     // Anzahl der verbleibenden Versuche, bevor der Spieler verliert

public:
    // Konstruktor: Initialisiert das Spiel mit dem zu erratenden Wort und der Anzahl der Versuche
    Hangman(const std::string& word, int attempts);

    // Verarbeitet einen erratenen Buchstaben und aktualisiert den Spielzustand
    bool guessLetter(char letter);

    // Überprüft, ob das Spiel vorbei ist (keine Versuche mehr)
    bool isGameOver() const;

    // Überprüft, ob der Spieler das gesamte Wort erraten hat
    bool isWordGuessed() const;

    // Berechnet die Anzahl der Versuche basierend auf dem Wort und den eindeutigen Buchstaben
    size_t calculateAttempts(const std::string& word);

    // Gibt das erratene Wort zur Anzeige zurück
    std::string getGuessedWord() const { return guessedWord; }

    // Gibt die verbleibenden Versuche zurück
    int getRemainingAttempts() const { return remainingAttempts; }

    // Gibt die falschen Vermutungen zur Anzeige zurück
    const std::vector<char>& getWrongGuesses() const { return wrongGuesses; }

    // Lässt die richtigen Buchstaben blinken, wenn sie erraten wurden (für visuelles Feedback)
    void blinkCorrectLetters(char letter) const;
};
