/*****************************************
 * Boggleplay.cpp
 *
 * Names: Claire Shu & Daniel Chan
 * Section Leader: Marco Ximenes Rego Monteiro
 *
 * Handles playing one game of Boggle. 
 *
 *****************************************/

#include "lexicon.h"
#include "Boggle.h"
#include "simpio.h"
#include "console.h"
#include "bogglegui.h"

void humanMove(string guess, Boggle &boggle);
void printHumanWordsAndScore(Boggle &boggle);
void computerMove(Boggle &boggle);
void printComputerWordsAndScore(Boggle &boggle, Set<string> &computerWords);

/* 
 * Play One Game 
 * 
 * Handles gameplay for one round of Boggle 
 */
void playOneGame(Lexicon& dictionary) {
    int DIMENSIONS = 4;
    BoggleGUI::initialize(DIMENSIONS, DIMENSIONS);
    Boggle boggle = initializeBoggle()
    
    BoggleGUI::labelAllCubes(newBoggle.getBoardText());

    string guess = getLine("Type a word (or Enter to stop):");
    humanMove(guess, newBoggle);
    computerMove(newBoggle);
    
}
Boggle initializeBoggle(){
    if (getYesOrNo("Do you want to generate a random board?")){
        return Boggle newBoggle(dictionary, "");
    } else {
        string input;
        bool isBad = true;
        while (isBad) {
            input = getLine("Type the 16 letters to appear on the board:");
            input = toUpperCase(input);
            if (input.size() == 16) {
                for (int i = 0; i < input.length(); i++) {
                    if (!(input[i] <= 'Z' && input[i] >= 'A')) {
                        isBad = true;
                        break;
                    }
                    isBad = false;
                }
            }
            if(isBad) {
                cout << "That is not a valid 16-letter board string. Try again.";
            }
        }
        return Boggle newBoggle(dictionary, input);

}

/* 
 * Human Move
 *
 * Manages the human player's move, including
 * GUI display, checking if the word input is valid, 
 * and searching to see if the word input can be found
 * on the Boggle board. 
 */
void humanMove(string guess, Boggle& boggle){
    while(guess != ""){
        BoggleGUI::clearHighlighting();
        clearConsole();
        guess = toUpperCase(guess);
        if(boggle.checkWord(guess)) {
            if(boggle.humanWordSearch(guess)){ 
                BoggleGUI::recordWord(guess, BoggleGUI::HUMAN);
            }
        }
        printHumanWordsAndScore(boggle);
        guess = getLine("Type a word (or Enter to stop):");
    }
}

/* 
 * Print Human Words and Scores
 *
 * Prints to console the current words and 
 * score of the human player 
 */
void printHumanWordsAndScore(Boggle& boggle) {
    cout << "It's your turn!" << endl;
    cout << boggle;
    cout << endl;
    cout << "Your words (" << boggle.getNumUsedWords() << "): " << boggle.getUsedWords() << endl;
    cout << "Your score: " << boggle.getScoreHuman() << endl;
    BoggleGUI::setScore(boggle.getScoreHuman(), BoggleGUI::HUMAN);
}

/* 
 * Computer Move 
 *
 * Runs the computer word search to find all words
 * on the Boggle board that were not found by the
 * human player. Prints to console the words found
 * by the computer and the score 
 */
void computerMove(Boggle& boggle) {
    Set<string> computerWords = boggle.computerWordSearch();
    printComputerWordsAndScore(boggle, computerWords);
}

/* 
 * Print Computer Words and Score 
 *
 * Prints to console the current words and 
 * score of the computer player 
 */
void printComputerWordsAndScore(Boggle& boggle, Set<string>& computerWords) {
    int compScore = boggle.getScoreComputer();
    int humanScore = boggle.getScoreHuman();

    cout << endl << "It's my turn!" << endl;
    cout << "My words (" << boggle.getNumComputerWords() << "): " << computerWords << endl;
    cout << "My score: " << compScore  << endl;
    for (string str : computerWords) {
        BoggleGUI::recordWord(str, BoggleGUI::COMPUTER);
    }
    BoggleGUI::setScore(boggle.getScoreComputer(), BoggleGUI::COMPUTER);

    // win lose cases
    if(compScore > humanScore) cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl << endl;
    else cout << "WOW, you defeated me! Congratulations!" << endl << endl;
}
