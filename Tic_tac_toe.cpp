#include <iostream>
#include <pthread.h>
#include "Board.h"
using namespace std;

class TicTacToe 
{
private:
    Board board;
    int invalids[2];
    bool gameOver;
    pthread_t thread1, thread2;
    pthread_mutex_t mutex;
    char currentPlayer;

public:
    TicTacToe() 
    {
        invalids[0] = 0;
        invalids[1] = 0;
        gameOver = false;
        currentPlayer = 'X'; // Player 1 starts the game
        pthread_mutex_init(&mutex, nullptr);
    }

    void playGame(char player) 
    {
        while (!gameOver) 
        {
            pthread_mutex_lock(&mutex);    // Locking the critical section
            if (gameOver) 
            {
                pthread_mutex_unlock(&mutex);
                break;
            }
            // Display current state of the board and player's turn
            board.display();
            cout << "Player"<<(player == 'X' ? 1 : 2)<<"'s turn (" << player << ")...!\n";
            int row, col;
            cout << "Enter the row and column where you want to put '"<<player<<"' (1-3): ";
            cin >> row >> col;
            row--;
            col--;
            
            if (updateBoard(row, col, player)) 
            {
                // Check if the current player has won
                if (checkWinner(player)) 
                {
                    board.display();
                    cout << "Player " << (player == 'X' ? 1 : 2) << " wins!\n";
                    gameOver = true;  
                    pthread_mutex_unlock(&mutex);   
                    break;
                }
                // Check if the board is full (resulting in a draw)
                if (board.isFull()) 
                {
                    board.display();
                    cout << "It's a draw!\n";
                    gameOver = true;   
                    pthread_mutex_unlock(&mutex); 
                    break;
                }
                player = (player == 'X' ? 'O' : 'X'); // Switch player's turn
            } 
            else 
            {
                cout << "Invalid move. Try again.\n";
                invalids[(player == 'X' ? 0 : 1)]++;       // Increment invalid move count
                if (invalids[(player == 'X' ? 0 : 1)] > 3) 
                {
                    gameOver = true;     // Set gameover flag if invalid moves exceed 3
                    pthread_mutex_unlock(&mutex); 
                    break;
                }
            }
            pthread_mutex_unlock(&mutex);    // Unlock mutex
        }
    }
    
    // Functions for player 1 and player 2 moves
    void* player1Move() 
    {
        playGame('X');
        return nullptr;
    }

    void* player2Move() 
    {
        playGame('O');
        return nullptr;
    }

    bool updateBoard(int row, int col, char player) 
    {
        return board.update(row, col, player);
    }
    
    bool checkWinner(char player) 
    {
        // Check rows and columns for winning combinations
        for (int i = 0; i < 3; ++i) 
        {
            if ((board.getCell(i, 0) == player && board.getCell(i, 1) == player && board.getCell(i, 2) == player) ||
                (board.getCell(0, i) == player && board.getCell(1, i) == player && board.getCell(2, i) == player)) 
            {
                return true; // Player has a winning row or column
            }
        }

        // Check diagonals for winning combinations
        if ((board.getCell(0, 0) == player && board.getCell(1, 1) == player && board.getCell(2, 2) == player) ||
            (board.getCell(0, 2) == player && board.getCell(1, 1) == player && board.getCell(2, 0) == player)) 
        {
            return true; // Player has a winning diagonal
        }

        return false; // No winning combinations found
    }

    void startGame() 
    {
        pthread_create(&thread1, nullptr, &TicTacToe::player1MoveHelper, this);    // Create thread for player 1
        pthread_join(thread1, nullptr);     // Wait for player 1's move to finish
        
        pthread_create(&thread2, nullptr, &TicTacToe::player2MoveHelper, this);    // Create thread for player 2
        pthread_join(thread2, nullptr);    // Wait for player 2's move to finish

        if (invalids[0] > 3) 
        {
            cout << "Player 2 wins! Player 1 has exceeded invalid entries..\n";
        } 
        else if (invalids[1] > 3) 
        {
            cout << "Player 1 wins! Player 2 has exceeded invalid entries..\n";
        } 
        else if (gameOver) {
            cout << "Game Over.\n";
        }
    }
    
    
    static void* player1MoveHelper(void* context)   // Helper functions to manage player moves using threads
    {
        reinterpret_cast<TicTacToe*>(context)->player1Move();
        return nullptr;
    }

    static void* player2MoveHelper(void* context) 
    {
        reinterpret_cast<TicTacToe*>(context)->player2Move();
        return nullptr;
    }

    ~TicTacToe()   // Destructor to destroy mutex lock
    {
        pthread_mutex_destroy(&mutex);
    }
};

int main() 
{
    TicTacToe game;
    game.startGame();
    return 0;
}

