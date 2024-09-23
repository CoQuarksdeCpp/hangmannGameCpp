# Hangman Game in C++

Welcome to the Hangman game! This is a classic word-guessing game implemented in C++.

## How to Play

1. **Start the Game**: The game begins by selecting a random word from a word pool.
2. **Guessing Letters**: You will see underscores (`_`) representing the letters of the word.
   - Type a letter to guess.
   - If the letter is correct, it will be revealed in the word.
   - If the letter is incorrect, you lose one attempt.
3. **Win Condition**: Guess all the letters correctly before running out of attempts.
4. **Lose Condition**: If you run out of attempts, the game is over, and the correct word will be revealed.
5. **Play Again**: After the game ends, you can choose to play again or exit.

## Game Instructions

1. When prompted, type a letter to guess.
2. The number of attempts is dynamically calculated based on the length and complexity of the word.
3. If you need help in the game, type `H` or `?` for instructions.

## Installation and Building

To build the game, follow these steps:

1. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/hangman-game.git
