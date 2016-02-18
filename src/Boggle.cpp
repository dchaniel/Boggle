// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "Boggle.h"
#include "grid.h"
#include "strlib.h"
#include "shuffle.h"
#include "bogglegui.h"

void randGenBoard(Grid<string>& board, string &boardText);
bool recursiveSearch(int row, int col, string word, Grid<string>& board, Grid<bool> &usedBlocks);
void updateScore(int length, int humanScore);
void recursiveComputer(int row, int col, string word, Grid<string>& board, Lexicon& dictionary, Set<string>& results, Grid<bool>& chosenCubes, int& computerScore, Set<string>& usedWords);


// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
    "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
    "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
    "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
    "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
    "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

const int DIMENSIONS = 4;

Boggle::Boggle(Lexicon& dictionary, string boardTextIn) {
    // STYLE: declaring private instance variables
    this->dictionary = dictionary;

    humanScore = 0;
    computerScore = 0;
    usedWords = {};
    board.resize(DIMENSIONS, DIMENSIONS);
    boardText = boardTextIn;

    if (boardText == "") {
        randGenBoard(board, boardText);
    } else {
        for (size_t i = 0; i < boardText.length(); i++) {
            string str = charToString(boardText[i]);
            board[i/4][i%4] = toUpperCase(str);
        }
    }
}


// ERROR CHECKING
void randGenBoard(Grid<string>& board, string &boardText){
    shuffle(CUBES, 16);
    //TODO: pls don't leave 16 here
    for (int i = 0; i < 16; i++) {
        string currCube = shuffle(CUBES[i]);
        board[i/4][i%4] = currCube[0];
        boardText += currCube[0];
    }
}

char Boggle::getLetter(int row, int col) {
    return stringToChar(board[row][col]);   // remove this
}

bool Boggle::checkWord(string word) {
    // TODO: implement
    word = toLowerCase(word);
    cout << usedWords << endl;
    if (!dictionary.contains(word)) return false;
    word = toUpperCase(word);

    if (word.length() < 4) return false;
    if (usedWords.contains(word)) return false;
    cout << "Check word returns true" << endl;
    return true ;
}

bool Boggle::humanWordSearch(string word) {
    //cout << usedWords << "wer" << endl;
    //if (usedWords.contains(word)) return false;
    for(int i = 0; i < DIMENSIONS; i++){
        for(int j = 0; j < DIMENSIONS; j++){
            word = toUpperCase(word);
            Grid<bool> usedBlocks(DIMENSIONS, DIMENSIONS, false);
            if(recursiveSearch(i, j, word, board, usedBlocks)){
                usedWords.add(word);
                //cout << usedWords << endl;
                humanScore += word.length() - 3;
                return true;
            }
        }
    }
    return false;
}

bool recursiveSearch(int row, int col, string word, Grid<string>& board, Grid<bool>& usedBlocks){
    if(!board.inBounds(row, col)) return false;
    BoggleGUI::setAnimationDelay(100);
    cout << "Recursive call to humanRecurse: " << endl;
    cout << "Row: " << row << " Col: " << col << " Word: " << word << " Character here: " << board[row][col] << endl;

    if(word == "") {
        return true;
    }
    if(board[row][col] == charToString(word[0])){
        usedBlocks[row][col] = true;
        cout << "first character matches" << endl;
        BoggleGUI::setHighlighted(row, col, true);
        for(int i = -1; i <= 1; i++){
            for(int j = -1; j <= 1; j++){
                if(!(i == 0 && j == 0)){
                    if(usedBlocks.inBounds(row + i, col + j) && !usedBlocks[row + i][col + j]) {
                        if (recursiveSearch(row + i, col + j, word.substr(1), board, usedBlocks)){
                            return true;
                        }
                    }
                }
            }
        }
    }
    BoggleGUI::setHighlighted(row, col, false);
    return false;
}

int Boggle::getScoreHuman() {
    return humanScore;
}

int Boggle::getNumUsedWords() {
    //cout << usedWords << "getnum" << endl;
    return usedWords.size();
}

Set<string> Boggle::getUsedWords() {
    return usedWords;
}

Set<string> Boggle::computerWordSearch() {
    Set<string> result;   // remove this
    //cout << usedWords << "cossf" << endl;
    for(int i = 0; i < DIMENSIONS; i++){
        for(int j = 0; j < DIMENSIONS; j++){
            Grid<bool> visitedCubes(DIMENSIONS, DIMENSIONS, false);

            recursiveComputer(i, j, "", board, dictionary, result, visitedCubes, computerScore, usedWords);
        }
    }
    resultsSize = result.size();
    return result;        // remove this
}
void recursiveComputer(int row, int col, string word, Grid<string>& board, Lexicon& dictionary, Set<string>& results, Grid<bool>& visitedCubes, int& computerScore, Set<string>& usedWords){
    if(!board.inBounds(row, col)) return;
    if(dictionary.contains(word) && word.length() >= 4) {

        if (!results.contains(word) && !usedWords.contains(word)) {
            computerScore += word.length() - 3;
            results.add(word);
        }
    }

    if(!dictionary.containsPrefix(word)){
        return;
    }
    word = word + board[row][col];

    visitedCubes[row][col] = true;

    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if(visitedCubes.inBounds(row+i, col+j) && !visitedCubes.get(row+i, col+j)){
                recursiveComputer(row + i, col + j, word, board, dictionary, results, visitedCubes, computerScore, usedWords);
            }
        }
    }
    visitedCubes[row][col] = false;
}

int Boggle::getScoreComputer() {
    return computerScore;
}

int Boggle::getNumComputerWords() {
    return resultsSize;
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    for (int i = 0; i < DIMENSIONS; i++) {
        for (int j = 0; j < DIMENSIONS; j++) {
            out << boggle.board[i][j];
        }
        out << endl;
    }
    return out;

}
string Boggle::getBoardText(){
    return boardText;
}

