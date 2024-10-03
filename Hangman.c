#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_WORD_LENGTH 24
#define MAX_INCORRECT_GUESSES 6
#define EXIT_SUCCESS 0

/**
 * Hangman is a word-guessing game where the player has to guess
 * all the letters in a randomly selected word before
 * incorrectly guessing 6 letters
 * @author Jack Smith
 */

void getWord(char *word, FILE *stream);
int guess(char letter, const char *word, char *guessedWord);
void drawHangman(int incorrect);
void printGuessedWord(const char *guessedWord);
void printAvailableLetters(const char *guessedLetters, const char *guessedWord);
int alreadyGuessed(char letter, const char *guessedLetters);

/**
 * Initiates a game, and calls helper functions upon each guess
 * Game ends when player runs out of guesses or correctly identifies all letters
 */
int main() {
    FILE *wordFile = fopen("words.txt", "r");
    if (!wordFile) {
        printf("Could not open words.txt\n");
        return 1;
    }

    /** Contains word to be guessed */
    char word[MAX_WORD_LENGTH];
    /** Tracks use progress on guessing word */
    char guessedWord[MAX_WORD_LENGTH];
    /** Tracks letters guessed */
    char guessedLetters[26] = {0};  
    /** Tracks times user guessed incorrectly */
    int incorrectGuesses = 0;
    /** Tracks if user guessed the entire word successfully */
    int guessedCorrectly = 0;

    srand(time(NULL));  // Seed the random number generator
    getWord(word, wordFile);
    fclose(wordFile);

    // Set guessedWord to contain underscores where there are letters to be guessed
    int wordLength = strlen(word);
    for (int i = 0; i < wordLength; i++) {
        guessedWord[i] = '_';
    }
    guessedWord[wordLength] = '\0';

    //Generate the interface, with updated visuals for guessed letters and hangman on each iteration
    while (incorrectGuesses < MAX_INCORRECT_GUESSES && !guessedCorrectly) {
        drawHangman(incorrectGuesses);
        printGuessedWord(guessedWord);
        printAvailableLetters(guessedLetters, guessedWord);
        printf("Incorrect guesses left: %d\n", MAX_INCORRECT_GUESSES - incorrectGuesses);

        char letter;
        int isValidInput = 0;

        // Get valid input
        while (!isValidInput) {
            printf("Enter a letter: ");
            scanf(" %c", &letter);
            // Convert to uppercase for comparison
            letter = toupper(letter);

            // Break loop if input is a valid letter
            if (isalpha(letter)) {
                isValidInput = 1;
            } else {
                printf("Please enter a valid letter.\n");
            }
        }

        // Check if the letter was guessed previously
        if (alreadyGuessed(letter, guessedLetters)) {
            printf("You have already guessed that letter.\n");
            continue;
        }

        //Mark letter as guessed
        guessedLetters[letter - 'A'] = letter;  

        if (!guess(letter, word, guessedWord)) {
            incorrectGuesses++;
        }
        // Check if the word is fully guessed
        guessedCorrectly = (strcmp(word, guessedWord) == 0);
    }

    if (guessedCorrectly) {
        printf("Congratulations! You guessed the word: %s\n", word);
    } else {
        drawHangman(incorrectGuesses);
        printf("Game over! The word was: %s\n", word);
    }

    return EXIT_SUCCESS;
}

/**
 * Reads in words from file words.txt, and randomly chooses one
 * limited first 100 words of file
 * file should be formatted as one word per line
 */
void getWord(char *word, FILE *stream) {
    char words[100][MAX_WORD_LENGTH];
    int wordCount = 0;

    while (fgets(words[wordCount], MAX_WORD_LENGTH, stream)) {
        for (int i = 0; i < strlen(words[wordCount]); i++) {
            if (!isalpha(words[wordCount][i])) {
                words[wordCount][i] = '\0';
            }
        }
        wordCount++;
    }

    int randomIndex = rand() % wordCount;
    strcpy(word, words[randomIndex]);
    word[strlen(word)] = '\0';
}

/**
 * Checks if the guessed letter is in the word
 * returns 1 if true, 0 otherwise
 */
int guess(char letter, const char *word, char *guessedWord) {
    int correctGuess = 0;
    for (int i = 0; i < strlen(word); i++) {
        if (toupper(word[i]) == letter) {
            guessedWord[i] = word[i];  // Keep the case of the original word
            correctGuess = 1;
        }
    }
    return correctGuess;
}

/**
 * Draws the current state of the hangman
 */
void drawHangman(int incorrect) {
    printf("+---------------------------------------+\n");
    printf("                HANGMAN                  \n");
    printf("                                         \n");

    switch (incorrect) {
        case 0:
            printf("                                         \n");
            printf("                                         \n");
            printf("                                         \n");
            printf("                                         \n");
            break;
        case 1:
            printf("                    O                    \n");
            printf("                                         \n");
            printf("                                         \n");
            printf("                                         \n");
            break;
        case 2:
            printf("                    O                    \n");
            printf("                    |                    \n");
            printf("                                         \n");
            printf("                                         \n");
            break;
        case 3:
            printf("                    O                    \n");
            printf("                   \\|                    \n");
            printf("                                         \n");
            break;
        case 4:
            printf("                    O                    \n");
            printf("                   \\|/                   \n");
            printf("                                         \n");
            break;
        case 5:
            printf("                    O                    \n");
            printf("                   \\|/                   \n");
            printf("                    |                    \n");
            printf("                                         \n");
            break;
        case 6:
            printf("                    O                    \n");
            printf("                   \\|/                   \n");
            printf("                    |                    \n");
            printf("                   / \\                   \n");
            break;
    }
    printf("+---------------------------------------+\n");
}

/**
 * Prints the currently guessed letters in the word (e.g., "_ a _ _ _")
 * Each unguessed letter is represented as an underscore
 */
void printGuessedWord(const char *guessedWord) {
    printf("Word: ");
    for (int i = 0; i < strlen(guessedWord); i++) {
        printf("%c ", guessedWord[i]);
    }
    printf("\n");
}

/**
 * Prints the available letters, and underscores for guessed letters
 */
void printAvailableLetters(const char *guessedLetters, const char *guessedWord) {
    printf("Available letters: ");
    for (char letter = 'A'; letter <= 'Z'; letter++) {
        if (guessedLetters[letter - 'A'] == 0) {
            printf("%c ", letter);  // Show available letters
        } else {
            printf("_ ");  // Hide already guessed letters
        }
    }
    printf("\n");
}

/**
 * Check if a letter has already been guessed
 */
int alreadyGuessed(char letter, const char *guessedLetters) {
    return guessedLetters[letter - 'A'] != 0;
}