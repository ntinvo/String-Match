/**
		Tin Vo
**/

#include <iostream> /// for cout, cin
#include <fstream>  /// for ifstream
#include <string>   /// for string

using namespace std;

/**
1D array version
Use pointer arithmetic to go through the array
input:  char*: board
char*: a word
int: num rows
int: num cols
output: char*: the solution board
*/
char* findMatch1D(char*, const char*, const int, const int);


/**
print the board
1D array version
Use pointer arithmetic to go through the array
input:  char*: board
int: num rows
int: num cols
*/
void print1D(char*, const int, const int);

/**
2D array version
Use array indexing to go through the array
input:  char**: board
char*: a word
int: num rows
int: num cols
output: char**: the solution board
*/
char** findMatch2D(char**, const char*, const int, const int);

/**
print the board
2D array version
input:  char**: board
int: num rows
int: num cols
*/
void print2D(char**, const int, const int);

// my own functions to find matches for two versions
void checkVersion(ifstream&, char*, const int, const int, const int);
void findMatches2DVersion(char**, char**, const char*, const int, const int, const int, const int);
void updateBoard2D(char**, char**, int*, const int, const int, const int);
void findMatches1DVersion(char*, char*, const char*, const int, const int, const int);
void updateBoard1D(char*, char*, int*, const int, const int, const int, const int);
int wordLen;

int main(int argc, char const **argv) {
	// variables
	string filename, word;
	int nRows, nCols, dimensions;
	char* pWord;
	
	// prompt user inputs
	cout << "Please enter a word: " << endl;
	getline(cin, word);
	cout << "Please enter a filename: " << endl;
	getline(cin, filename);
	cout << "Please enter dimensions: " << endl;
	cin >> dimensions;

	//get rows, cols, and board from file
	ifstream file(filename.c_str());
	file >> nRows;
	file >> nCols;

	//length of the word
	wordLen = word.length();
	pWord = new char[wordLen];
	for (int i = 0; i < wordLen; i++)
		pWord[i] = word[i];

	// check dimensions and find
	checkVersion(file, pWord, dimensions, nRows, nCols);
	
	int a;
	cin >> a;
	return 0;
}

// check version, find, and print out the boards
void checkVersion(ifstream &file, char* pWord, const int dimensions, const int nRows, const int nCols) {
	if (dimensions == 1) {
		char* board = new char[nRows * nCols];
		for (int index = 0; index < nRows * nCols; index++)
			file >> board[index];
		cout << "Original board:    " << endl;;
		print1D(board, nRows, nCols);
		cout << endl;
		cout << "Result board:      " << endl;
		board = findMatch1D(board, pWord, nRows, nCols);
		print1D(board, nRows, nCols);
	} else {
		//create double pointer for board
		char **board;
		board = new char*[nCols];
		for(int index = 0; index < nRows; index++)
			board[index] = new char[nCols];

		//initialize with elements from file
		for (int row = 0; row < nRows; row++)
			for (int col = 0; col < nCols; col++)
				file >> board[row][col];

		// find matches and print board
		cout << "Original board:    " << endl;;
		print2D(board, nRows, nCols);
		cout << endl;
		cout << "Result board:      " << endl;		
		board = findMatch2D(board, pWord , nRows, nCols); 
		print2D(board, nRows, nCols);
	}
}

// find matches of word for 1D version
char* findMatch1D(char* board, const char* word, const int nRows, const int nCols) {
	// create and initialize newBoard
	char* newBoard = new char[nRows * nCols];
	for(int index = 0; index < nRows * nCols; index++)
		newBoard[index] = '.';

	// loop through board to find matches
	for(int index = 0; index < nRows * nCols; index++)
		if(board[index] == word[0])
			findMatches1DVersion(board, newBoard, word, index, nRows, nCols);

	// return board
	return newBoard;
}

// find matches and update board for 1D version
void findMatches1DVersion(char* board, char* newBoard, const char* word, const int index, const int nRows, const int nCols) {
	// variables
	int range = wordLen - 1;
	int wordIndex;
	int row = index / nCols;
	int col = index % nCols;
	int* counter;

	// initialize counter
	counter = new int[8];
	for(int i = 0; i < 8; i++)
		counter[i] = 0;	

	// assign current element to newBoard
	newBoard[index] = board[index];

	// count left, upLeft, up, and UpRight
	wordIndex = wordLen - 1;
	for(int i = -range ; i < 0; i++){
		if((row + i) >= 0 && board[(row + i) * nCols + col] == word[wordIndex]) 								// up direction
			counter[0]++;
		if((col + i) >= 0 && board[row * nCols + (col + i)] == word[wordIndex])									// left direction
			counter[1]++;
		if((row + i) >= 0 && (col + i) >= 0 && board[(row + i) * nCols + (col + i)] == word[wordIndex])			// upLeft direction
			counter[2]++;
		if((row + i) >= 0 && (col - i) < nRows && board[(row + i) * nCols + (col - i)] == word[wordIndex])		// upRight direction
			counter[3]++;				
		wordIndex--;
	}

	// count right, downLeft, down, and downRight
	wordIndex = 1;
	for(int i = 1; i <= range; i++) {
		if((row + i) < nRows && board[(row + i) * nCols + col] == word[wordIndex])								// down direction
			counter[4]++;
		if((col + i) < nCols && board[row * nCols + (col + i)] == word[wordIndex])								// right direction
			counter[5]++;
		if((row + i) < nRows && (col - i) >= 0 && board[(row + i) * nCols + (col - i)] == word[wordIndex])		// downLeft direction
			counter[6]++;
		if((row + i) < nRows && (col + i) < nRows && board[(row + i) * nCols + (col + i)] == word[wordIndex])	// downRight direction
			counter[7]++;
		wordIndex++;
	}

	// update board
	updateBoard1D(board, newBoard, counter, range, row, col, nCols);
}

// update board for 1D version
void updateBoard1D(char* board, char* newBoard, int* counter, const int range, const int row, const int col, const int nCols) {
	bool matchFound = false;

	if(counter[0] == range) {																			// up direction
		matchFound = true;
		for(int i = -range; i < 0; i++)
			newBoard[(row + i) * nCols + col] = board[(row + i) * nCols + col];
	}
	if(counter[1] == range) {																			// left direction
		matchFound = true;
		for(int i = -range; i < 0; i++)
			newBoard[row * nCols + (col + i)] = board[row * nCols + (col + i)];
	}
	if(counter[4] == range) {																			// down direction
		matchFound = true;
		for(int i = 1; i <= range; i++)
			newBoard[(row + i) * nCols + col] = board[(row + i) * nCols + col];
	}
	if(counter[5] == range) {																			// right direction
		matchFound = true;
		for(int i = 1; i <= range; i++)
			newBoard[row * nCols + (col + i)] = board[row * nCols + (col + i)];
	}
	if(counter[2] == range) {																			// upLeft direction
		matchFound = true;
		for(int i = -range; i < 0; i++)
			newBoard[(row + i) * nCols + (col + i)] = board[(row + i) * nCols + (col + i)];
	}
	if(counter[3] == range) {																			// upRight direction
		matchFound = true;
		for(int i = -range; i < 0; i++)
			newBoard[(row + i) * nCols + (col - i)] = board[(row + i) * nCols + (col - i)];
	}
	if(counter[6] == range) {																			// downLeft direction
		matchFound = true;
		for(int i = 1; i <= range; i++)
			newBoard[(row + i) * nCols + (col - i)] = board[(row + i) * nCols + (col - i)];	
	}
	if(counter[7] == range) {																			// downright direction
		matchFound = true;
		for(int i = 1; i <= range; i++)
			newBoard[(row + i) * nCols + (col + i)] = board[(row + i) * nCols + (col + i)];
	}
	if(!matchFound)																						// no match found, reset board
		newBoard[row * nCols + col] = '.';
}


// print board for 1D version
void print1D(char* board, const int nRows, const int nCols) {
	int index = 0;
	for (int row = 0; row < nRows; row++){
		for (int col = 0; col < nCols; col++) {
			cout << board[index];
			index++;
		}
		cout << endl;
	}
}

// find matches of word for 2D version
char** findMatch2D(char** board, const char* word , const int nRows, const int nCols) {
	// create newBoard
	char** newBoard;
	newBoard = new char *[nCols];
	for(int index = 0; index < nRows; index++)
		newBoard[index] = new char[nCols];

	// initialize newBoard
	for (int row = 0; row < nRows; row++)
		for(int col = 0; col < nCols; col++)
			newBoard[row][col] = '.';

	// loop through board to find matches
	for(int row = 0; row < nRows; row++) {
		for(int col = 0; col < nCols; col++) {
			if(board[row][col] == word[0])
				findMatches2DVersion(board, newBoard, word, row, col, nRows, nCols);
		}
	}
	return newBoard;
}

// find matches and update board for 2D version
void findMatches2DVersion(char** board, char** newBoard, const char* word, const int row, const int col, const int nRows, const int nCols) {
	// instance variables
	int range = wordLen - 1;
	int* counter;	
	int index;

	// initialize counter
	counter = new int[8];
	for(int i = 0; i < 8; i++)
		counter[i] = 0;

	// assign current element to newBoard
	newBoard[row][col] = board[row][col];

	// count left, upLeft, up, and UpRight
	index = wordLen - 1;
	for(int i = -range ; i < 0; i++){
		if((row + i) >= 0 && board[row + i][col] == word[index]) 								// up direction
			counter[0]++;
		if((col + i) >= 0 && board[row][col + i] == word[index])								// left direction
			counter[1]++;
		if((row + i) >= 0 && (col + i) >= 0 && board[row + i][col + i] == word[index])			// upLeft direction
			counter[2]++;
		if((row + i) >= 0 && (col - i) < nRows && board[row + i][col - i] == word[index])		// upRight direction
			counter[3]++;				
		index--;
	}

	// count right, downLeft, down, and downRight
	index = 1;
	for(int i = 1; i <= range; i++) {
		if((row + i) < nRows && board[row + i][col] == word[index])								// down direction
			counter[4]++;
		if((col + i) < nCols && board[row][col + i] == word[index])								// right direction
			counter[5]++;
		if((row + i) < nRows && (col - i) >= 0 && board[row + i][col - i] == word[index])		// downLeft direction
			counter[6]++;
		if((row + i) < nRows && (col + i) < nRows && board[row + i][col + i] == word[index])	// downRight direction
			counter[7]++;
		index++;
	}

	// update board
	updateBoard2D(board, newBoard, counter, range, row, col);
}

// update board for 2D version
void updateBoard2D(char** board, char** newBoard, int* counter, const int range, const int row, const int col) {
	bool matchFound = false;

	if(counter[0] == range) {										// up direction				
		matchFound = true;
		for(int i = -range; i < 0; i++)
			newBoard[row + i][col] = board[row + i][col];
	}
	if(counter[1] == range) {										// left direction
		matchFound = true;
		for(int i = -range; i < 0; i++)
			newBoard[row][col + i] = board[row][col + i];
	}				
	if(counter[4] == range) {										// down direction
		matchFound = true;
		for(int i = 1; i <= range; i++)
			newBoard[row + i][col] = board[row + i][col];
	}
	if(counter[5] == range) {										// right direction
		matchFound = true;
		for(int i = 1; i <= range; i++)
			newBoard[row][col + i] = board[row][col + i];
	}
	if(counter[2] == range) {										// upLeft direction
		matchFound = true;
		for(int i = -range; i < 0; i++)
			newBoard[row + i][col + i] = board[row + i][col + i];
	}
	if(counter[3] == range) {										// upRight direction
		matchFound = true;	
		for(int i = -range; i < 0; i++)
			newBoard[row + i][col - i] = board[row + i][col - i];
	}
	if(counter[6] == range) {										// downLeft direction
		matchFound = true;
		for(int i = 1; i <= range; i++)
			newBoard[row + i][col - i] = board[row + i][col - i];	
	}
	if(counter[7] == range) {										// downRight direction
		matchFound = true;
		for(int i = 1; i <= range; i++)
			newBoard[row + i][col + i] = board[row + i][col + i];
	}
	if(!matchFound)													// no match found, reset board
		newBoard[row][col] = '.';
}

// print board for 2D version
void print2D(char **board, const int nRows, const int nCols) {
	for (int row = 0; row < nRows; row++) {
		for (int col = 0; col < nCols; col++) {
			cout << board[row][col];
		}
		cout << endl;
	}
}