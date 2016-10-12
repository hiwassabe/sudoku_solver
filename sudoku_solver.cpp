#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <chrono>

//Globals
#define SIZE 9

// prints Sudoku puzzle
void printPuzzle(int puzzle[SIZE][SIZE]);

// finds an empty square in puzzle; returns false if no squares are empty
bool foundEmptyGrid(int puzzle[SIZE][SIZE], int &row, int &col);

// checks if a number exists in a given row
bool foundInRow(int puzzle[SIZE][SIZE], int row, int num);

// checks if a number exists in a given column
bool foundInCol(int puzzle[SIZE][SIZE], int col, int num);

// checks if a number exists in a given grid
bool foundInGrid(int puzzle[SIZE][SIZE], int row, int col, int num);

// implements the backtracking algorithm 
bool solver(int puzzle[SIZE][SIZE]);


int main(int argc, char * argv[]){
	int puzzle[SIZE][SIZE];
	std::string filename;

	std::cout << "Enter Sudoku puzzle file: ";
	std::cin >> filename;

	std::ifstream myfile(filename);
	if(myfile.fail()){
		std::cerr << "File cannot be found or opened." << std::endl;
		exit(1);
	}

	// populate puzzle array and print out initial board
	for(int i = 0; i < SIZE; i++){
		for(int j = 0; j < SIZE; j++)
			myfile >> puzzle[i][j];
	}
	myfile.close();
	printPuzzle(puzzle);

	// get start time of sudoku solver
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

	if(solver(puzzle)){
		// get end time of sudoku solver
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		printPuzzle(puzzle);
		std::chrono::duration<double> elapsed_sec = end - start;
		std::cout << "Elapsed time: " << elapsed_sec.count() << "s\n";
	}
	else
		std::cout << "Invalid puzzle: cannot be solved" << std::endl;

	return 0;
}

void printPuzzle(int puzzle[SIZE][SIZE]){
	for(int i = 0; i < SIZE; i++){
		for(int j = 0; j < SIZE; j++){
			std::cout << puzzle[i][j] << " ";
			if(j == 2 || j == 5)
				std::cout << "| ";
		}
		std::cout << std::endl;
		if(i == 2 || i == 5)
			std::cout << "---------------------" << std::endl;
	}
	std::cout << std::endl;
}

// checks to see if grid is empty
bool foundEmptyGrid(int puzzle[SIZE][SIZE], int &row, int &col){
	for(int i = 0; i < SIZE; i++){
		for(int j = 0; j < SIZE; j++){
			if(puzzle[i][j] == 0){
				row = i;
				col = j;
				return true;
			}
		}
	}
	return false;
}

// checks if a number exists in a given row
bool foundInRow(int puzzle[SIZE][SIZE], int row, int num){
	for(int i = 0; i < SIZE; i++){
		if(puzzle[row][i] == num)
			return true;
	}
	return false;
}


// checks if a number exists in a given column
bool foundInCol(int puzzle[SIZE][SIZE], int col, int num){
	for(int i = 0; i < SIZE; i++){
		if(puzzle[i][col] == num)
			return true;
	}
	return false;
}

// checks if a number exists in a given 3x3 grid
bool foundInGrid(int puzzle[SIZE][SIZE], int row, int col, int num){
	// calculate where a 3x3 grid begins (top and leftmost corner)
	int gridRowBegin = row - (row % 3);
	int gridColBegin = col - (col % 3);

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			if(puzzle[i + gridRowBegin][j + gridColBegin] == num)
				return true;
		}
	}
	return false;
}

// returns true if puzzle is solved
bool solver(int puzzle[SIZE][SIZE]){
	int row = 0;
	int col = 0;

	// check if there is an empty grid in puzzle and set row/col
	if(!foundEmptyGrid(puzzle, row, col)){
		return true; //puzzle solved
	}

	//std::cout << row << " " << col << std::endl;

	for(int i = 1; i < 10; i++){
		if(foundInRow(puzzle, row, i))
			continue; // go to next number if found in row
		if(foundInCol(puzzle, col, i))
			continue; // go to next number if found in column
		if(foundInGrid(puzzle, row, col, i))
			continue; // go to next number if found in 3x3 grid

		puzzle[row][col] = i; // set empty square with valid number

		if(solver(puzzle)){
			return true;
		}
		else
			puzzle[row][col] = 0; // failure occurred, redo
	}
	return false;
}