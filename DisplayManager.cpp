#include "DisplayManager.h"
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

// Verbesserte Funktion clearScreen für Windows und Unix-basierte Systeme
void DisplayManager::clearScreen() {
#ifdef _WIN32
    // Setzt den Cursor auf die oberste linke Ecke zurück, um den Bildschirm zu "leeren"
    COORD topLeft = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    // Holen der aktuellen Puffergröße
    GetConsoleScreenBufferInfo(console, &screen);

    // Füllt den gesamten Bildschirm mit Leerzeichen
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );

    // Füllt die Attributwerte (Farben etc.) zurück
    FillConsoleOutputAttribute(
        console, screen.wAttributes, screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );

    // Setzt den Cursor auf die ursprüngliche Position zurück
    SetConsoleCursorPosition(console, topLeft);
#else
    // Auf Unix-basierten Systemen wird der Bildschirm durch den Befehl "clear" gelöscht
    system("clear");
#endif
}

// Bestehende Funktion getTerminalWidth
int DisplayManager::getTerminalWidth() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    else {
        columns = 80;
    }
    return columns;
#else
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        return w.ws_col;
    }
    else {
        return 80;
    }
#endif
}

// Bestehende Funktion centerText
void DisplayManager::centerText(const std::string& text) {
    int terminalWidth = getTerminalWidth();
    size_t textLength = text.length();
    int padding = (terminalWidth - static_cast<int>(textLength)) / 2;
    if (padding > 0) {
        std::cout << std::setw(padding) << "" << text << std::endl;
    }
    else {
        std::cout << text << std::endl;
    }
}

// Bestehende Funktion showInstructions
void DisplayManager::showInstructions() {
    clearScreen();
    centerText("Willkommen zu Hangman!\n\n");
    std::cout << "Anweisungen:\n";
    std::cout << "1. Errate die Buchstaben im Wort.\n";
    std::cout << "2. Für jede richtige Vermutung wird der entsprechende Buchstabe enthüllt.\n";
    std::cout << "3. Wenn du falsch rätst, verringert sich die Anzahl der Versuche.\n";
    std::cout << "4. Du gewinnst, indem du das ganze Wort errätst, oder verlierst, indem du keine Versuche mehr hast.\n";
    std::cout << "Viel Glück!\n";
}

// Bestehende Funktion blinkMessage
void DisplayManager::blinkMessage(const std::string& message) {
    for (int i = 0; i < 2; ++i) {
        std::cout << message << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "\r" << std::string(message.length(), ' ') << "\r" << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

// Neue Funktion displayStatus (Spielestatus anzeigen)
void DisplayManager::displayStatus(const std::string& guessedWord, const std::vector<char>& wrongGuesses, int remainingAttempts) {
    std::cout << "Wort: ";
    for (char c : guessedWord) {
        std::cout << c << ' ';
    }
    std::cout << std::endl;

    std::cout << "Falsche Vermutungen: \033[31m";
    for (char c : wrongGuesses) {
        std::cout << c << ' ';
    }
    std::cout << "\033[0m";
    std::cout << "\nVerbleibende Versuche: " << remainingAttempts << std::endl;
}

// Neue Funktion blinkCorrectLetters (Buchstaben blinken)
void DisplayManager::blinkCorrectLetters(const std::string& guessedWord, const std::vector<size_t>& correctPositions) {
    for (int i = 0; i < 2; ++i) {
        clearScreen();

        std::cout << "Wort: ";
        for (size_t j = 0; j < guessedWord.length(); ++j) {
            if (std::find(correctPositions.begin(), correctPositions.end(), j) != correctPositions.end()) {
                std::cout << "\033[32m_ \033[0m";
            }
            else {
                std::cout << guessedWord[j] << ' ';
            }
        }
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        clearScreen();

        std::cout << "Wort: ";
        for (size_t j = 0; j < guessedWord.length(); ++j) {
            if (std::find(correctPositions.begin(), correctPositions.end(), j) != correctPositions.end()) {
                std::cout << "\033[32m" << guessedWord[j] << "\033[0m ";
            }
            else {
                std::cout << guessedWord[j] << ' ';
            }
        }
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}
