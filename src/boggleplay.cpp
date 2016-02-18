// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "lexicon.h"
#include "Boggle.h"
#include "simpio.h"
#include "console.h"
#include "bogglegui.h"

//TODO: recounts for GOLF and golf

void humanMove(string guess, Boggle &boggle);
void printHumanWordsAndScore(Boggle &boggle);
void computerMove(Boggle &boggle);
void printComputerWordsAndScore(Boggle &boggle, Set<string> &computerWords);

void playOneGame(Lexicon& dictionary) {
    // TODO: implement
    int DIMENSIONS = 4;
    BoggleGUI::initialize(DIMENSIONS, DIMENSIONS);

    if (getYesOrNo("Do you want to generate a random board?")){
        Boggle newBoggle(dictionary, "");
        BoggleGUI::labelAllCubes(newBoggle.getBoardText());

        cout << newBoggle;
        string guess = getLine("Type a word (or Enter to stop):");
        humanMove(guess, newBoggle);
        computerMove(newBoggle);
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
        Boggle newBoggle(dictionary, input);
        BoggleGUI::labelAllCubes(newBoggle.getBoardText());

        cout << newBoggle;

        string guess = getLine("Type a word (or Enter to stop):");
        humanMove(guess, newBoggle);
        computerMove(newBoggle);

    }

}

void humanMove(string guess, Boggle& boggle){
    while(guess != ""){
        BoggleGUI::clearHighlighting();
        clearConsole();
        guess = toUpperCase(guess);
        if(boggle.checkWord(guess)) {
            if(boggle.humanWordSearch(guess)){ // this will update the score and print to console too
                BoggleGUI::recordWord(guess, BoggleGUI::HUMAN);
            }
        }
        printHumanWordsAndScore(boggle);
        guess = getLine("Type a word (or Enter to stop):");
        //cout << boggle.getUsedWords() << "hello" << endl;
    }
}

void printHumanWordsAndScore(Boggle& boggle) {
    cout << "It's your turn!" << endl;
    cout << boggle;
    cout << endl;
    cout << "Your words (" << boggle.getNumUsedWords() << "): " << boggle.getUsedWords() << endl;
    cout << "Your score: " << boggle.getScoreHuman() << endl;
    BoggleGUI::setScore(boggle.getScoreHuman(), BoggleGUI::HUMAN);
}

void computerMove(Boggle& boggle) {
    Set<string> computerWords = boggle.computerWordSearch();
    printComputerWordsAndScore(boggle, computerWords);
}

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
