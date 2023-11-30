#include <iostream>
using namespace std;

class Board {
private:
    char board[3][3];

public:
    Board() {
        initialize();
    }

    void initialize() {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                board[i][j] = '-';
            }
        }
    }

    void display() 
    {
        cout<<endl;
    	cout << "   1   2   3  " << endl;
    	cout << " +---+---+---+" << endl;
    	for (int i = 0; i < 3; ++i) 
    	{
            cout << i + 1 << "| ";
            for (int j = 0; j < 3; ++j) 
            {
            	cout << board[i][j] << " | ";
            }
            cout << endl << " +---+---+---+" << endl;
        }
    }
    
    bool update(int row, int col, char player) {
        if (row < 0 || row >= 3 || col < 0 || col >= 3 || board[row][col] != '-') {
            return false;
        }
        board[row][col] = player;
        return true;
    }

    bool isFull() {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == '-') {
                    return false;
                }
            }
        }
        return true;
    }

    char getCell(int row, int col) const {
        return board[row][col];
    }
};
