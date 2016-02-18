// This is a .h file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    char getLetter(int row, int col);
    bool checkWord(string word);
    bool humanWordSearch(string word);
    Set<string> computerWordSearch();
    int getScoreHuman();
    int getScoreComputer();
    Set<string> getUsedWords();
    int getNumUsedWords();
    int getNumComputerWords();
    string getBoardText();

    friend ostream& operator<<(ostream& out, Boggle& boggle);

private:
    string boardText;
    Grid<string> board;
    Set<string> usedWords;
    Lexicon dictionary;
    int humanScore;
    int computerScore;
    int resultsSize;
};

#endif // _boggle_h
