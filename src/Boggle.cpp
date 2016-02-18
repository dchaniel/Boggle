/*****************************************
 * Boggle.cpp
 *
 * Names: Claire Shu & Daniel Chan
 * Section Leader: Marco Ximenes Rego Monteiro
 *
 * Handles the algorithmic methods in Boggle 
 * to perform major game functions. 
 *
 *****************************************/
 

#include "Boggle.h"
#include "grid.h"
#include "strlib.h"
#include "shuffle.h"
#include "bogglegui.h"

void randGenBoard(Grid<string>& board, string &boardText);
bool recursiveSearch(int row, int col, string word, Grid<string>& board, Grid<bool> &usedBlocks);
void updateScore(int length, int humanScore);
void recursiveComputer(int row, int col, string word, Grid<string>& board, Lexicon& dictionary, Set<string>& results, Grid<bool>& chosenCubes, int& computerScore, Set<string>& usedWords);
void loadBoard(string boardText, Grid<string>& board);

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

// the dimensions of the grid (number of rows and cols)
const int DIMENSIONS = 4;

/*
 * Initializes the Boggle object 
 */
Boggle::Boggle(Lexicon& dictionary, string boardTextIn) {
    this->dictionary = dictionary;

    //Initialize Variables
    humanScore = 0;
    computerScore = 0;
    usedWords = {};
    board.resize(DIMENSIONS, DIMENSIONS);
    boardText = boardTextIn;
    
    if (boardText == "") {
        randGenBoard(board, boardText);
    } else {
        loadBoard(boardText, board);
    }
}
/*
 * Load (non random) board
 *
 * Fills board from given 16 character string
 */
void loadBoard(string boardText, Grid<string>& board){
    for (size_t i = 0; i < boardText.length(); i++) {
        string str = charToString(boardText[i]);
        board[i/4][i%4] = toUpperCase(str);
    }
}

/*
 * Random Generate Board
 *
 * Randomly generates the board by shuffling the cubes
 */
void randGenBoard(Grid<string>& board, string &boardText){
    shuffle(CUBES, DIMENSIONS * DIMENSIONS);
    for (int i = 0; i < (DIMENSIONS * DIMENSIONS); i++) {
        string currCube = shuffle(CUBES[i]);
        board[i/4][i%4] = currCube[0];
        boardText += currCube[0];
    }
}

/*
 * Get Letter
 *
 * Retruns the character in the Boggle board at the 
 * given row and col 
 */
char Boggle::getLetter(int row, int col) {
    return stringToChar(board[row][col]);   // remove this
}

/*
 * Check Word
 *
 * Checks the input word by the human player to
 * see if it is valid 
 */
bool Boggle::checkWord(string word) {
    word = toLowerCase(word);
    if (!dictionary.contains(word)) return false;
    word = toUpperCase(word);

    if (word.length() < 4) return false;
    if (usedWords.contains(word)) return false;
    return true ;
}

/*
 * Human Word Search
 *
 * Returns a boolean based on whether the word input
 * by the human player is found on the Boggle board
 * Wrapper for Recursive Search
 */
bool Boggle::humanWordSearch(string word) {
    for(int i = 0; i < DIMENSIONS; i++){
        for(int j = 0; j < DIMENSIONS; j++){
            word = toUpperCase(word);
            Grid<bool> usedBlocks(DIMENSIONS, DIMENSIONS, false);
            if(recursiveSearch(i, j, word, board, usedBlocks)){
                usedWords.add(word);
                humanScore += word.length() - 3;
                return true;
            }
        }
    }
    return false;
}

/*
 * Recursive Search (Human Word Search)
 *
 * Recursively searches the Boggle board to find if
 * the word can be found the board and returns
 * a boolean accordingly
 */
bool recursiveSearch(int row, int col, string word, Grid<string>& board, Grid<bool>& usedBlocks){
    if(!board.inBounds(row, col)) return false;
    if(word == "") {
        return true;
    }
    BoggleGUI::setHighlighted(row, col, true);

    if(board[row][col] == charToString(word[0])){
        usedBlocks[row][col] = true;
        BoggleGUI::setAnimationDelay(100);

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

/*
 * Get Score Human
 *
 * Returns the current score of the human player
 */
int Boggle::getScoreHuman() {
    return humanScore;
}

/*
 * Get Number of Used Words
 *
 * Returns the number of words that the human
 * player has guessed correctly so far 
 */
int Boggle::getNumUsedWords() {
    return usedWords.size();
}

/*
 * Get Used WOrds
 *
 * Returns the set of words that have already
 * been guessed correctly by the human player
 */
Set<string> Boggle::getUsedWords() {
    return usedWords;
}

/*
 * Computer Word Search
 *
 * Searches the Boggle board for all the possible
 * words that can be made on the board and returns
 * them in a set. Excludes the words that have
 * already been found by the human player 
 */
Set<string> Boggle::computerWordSearch() {
    Set<string> result;   
    for(int i = 0; i < DIMENSIONS; i++){
        for(int j = 0; j < DIMENSIONS; j++){
            Grid<bool> visitedCubes(DIMENSIONS, DIMENSIONS, false);
            recursiveComputer(i, j, "", board, dictionary, result, visitedCubes, computerScore, usedWords);
        }
    }
    resultsSize = result.size();
    return result;
}

/*
 * Recursive Computer Search
 *
 * Recursively searches the Boggle board to find all the
 * words that can be made on the board and haven't
 * been guessed correctly by the human player 
 */
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

/*
 * Get Compute Score
 *
 * Returns the current score of the computer player
 */
int Boggle::getScoreComputer() {
    return computerScore;
}

/*
 * Get Number Computer Words
 *
 * Returns the number of words that have been
 * found by the computer on the board
 */
int Boggle::getNumComputerWords() {
    return resultsSize;
}

/*
 * ostream
 *
 * Handeles printing a Boggle object to the console 
 */
ostream& operator<<(ostream& out, Boggle& boggle) {
    for (int i = 0; i < DIMENSIONS; i++) {
        for (int j = 0; j < DIMENSIONS; j++) {
            out << boggle.board[i][j];
        }
        out << endl;
    }
    return out;

}

/*
 * Get Board Text
 *
 * Returns the string of 16 letters that you put in 
 */
string Boggle::getBoardText(){
    return boardText;
}

