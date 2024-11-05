#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
using namespace std;

class Cell
{
    public:
    Cell ():isMine (false), isRevealed (false), isFlagged (false),adjacentMines (0)
{}

bool isMine;
bool isRevealed;
bool isFlagged;
int adjacentMines;
};

class Board
{
    public:
    Board (int numrows, int numcols, int mines):numrows (numrows),numcols (numcols), mines (mines)
{
    cells.resize (numrows, vector < Cell > (numcols));
    srand (time (NULL));
    int numMinesPlaced = 0;
    while (numMinesPlaced < mines)
    {
        int row = rand () % numrows;
        int col = rand () % numcols;
        if (!cells[row][col].isMine)
        {
	      cells[row][col].isMine = true;
	      numMinesPlaced++;
        }
    }
    for (int row = 0; row < numrows; row++)
    {
        for (int col = 0; col < numcols; col++)
        {
            if (cells[row][col].isMine)
            {
                continue;
            }
            int count = 0;
            for (int i = max (0, row - 1); i <= min (numrows - 1, row + 1);
            i++)
            {
                for (int j = max (0, col - 1);
                j <= min (numcols - 1, col + 1); j++)
                {
                    if (cells[i][j].isMine)
                    {
                        count++;
                    }
                }
            }
            cells[row][col].adjacentMines = count;
        }
    }
}

int getNumRows () const
{
    return numrows;
}

int getNumCols () const
{
    return numcols;
}

int getNumMines () const
{
    return mines;
}

const Cell & getCell (int row, int col) const
{
    return cells[row][col];
}

Cell & getCell (int row, int col)
{
    return cells[row][col];
}
private:
int numrows;
int numcols;
int mines;
vector < vector < Cell >> cells;
};

class Game
{
public:
Game (int numrows, int numcols, int mines):board (numrows, numcols,mines),isGameOver (false), numCellsRevealed (0), hasWon (false)
{
}

void run ()
{
    while (!isGameOver)
    {
        displayboard ();
        int row, col;
        cout << "Enter row and column coordinates: ";
        cin >> row >> col;
        if (row < 0 || row >= board.getNumRows () || col < 0|| col >= board.getNumCols ())
        {
            cout << "Invalid coordinates." << endl;
            continue;
        }
        if (board.getCell (row, col).isFlagged)
        {
            cout << "Cell is flagged. Unflag it first." << endl;
            continue;
        }
        if (board.getCell (row, col).isRevealed)
        {
            cout << "Cell is already revealed." << endl;
	        continue;
        }
        revealCell (row, col);
        checkGameOver ();
    }
    displayboard ();
    if (hasWon)
    {
        cout << "You won the game, congratulations :-)" << endl;
    }
    else
    {
        cout << "Game over" << endl;
    }
}

private:
Board board;
bool isGameOver;
bool hasWon;
int numCellsRevealed;

void displayboard () const
{
    cout << " ";
    for (int col = 0; col < board.getNumCols (); col++)
    {
        cout << " " << col;
    }
    cout << endl;
    for (int row = 0; row < board.getNumRows (); row++)
    {
        cout << row << "|";
	for (int col = 0; col < board.getNumCols (); col++)
	{
	    const Cell & cell = board.getCell (row, col);
	    if (cell.isFlagged)
	    {
	        cout << "F|";
	    }
	    else if (!cell.isRevealed)
	    {
	        cout << ".|";
	    }
	    else if (cell.isMine)
	    {
	        cout << "*|";
	    }
	    else
	    {
	        cout << cell.adjacentMines << "|";
	    }
	}
	cout << endl;
    }
}

void revealCell (int row, int col)
{
    Cell & cell = board.getCell (row, col);
    cell.isRevealed = true;
    numCellsRevealed++;
    if (cell.isMine)
    {
        isGameOver = true;
    }
    else if (cell.adjacentMines == 0)
    {
        for (int i = max (0, row - 1);
        i <= min (board.getNumRows () - 1, row + 1); i++)
        {
            for (int j = max (0, col - 1);
            j <= min (board.getNumCols () - 1, col + 1); j++)
            {
                if (i == row && j == col)
	        {
	            continue;
	        }
	        Cell & adjacentCell = board.getCell (i, j);
	        if (!adjacentCell.isRevealed && !adjacentCell.isFlagged)
	        {
	            revealCell (i, j);
	        }
	    }
	}
    }
}

void checkGameOver ()
{
    if (numCellsRevealed == board.getNumRows () * board.getNumCols () - board.getNumMines ())
    {
        isGameOver = true;
        hasWon = true;
    }
  }
};

int main ()
{
    int numrows = 10;
    int numcols = 10;
    int mines = 10;
    Game game (numrows, numcols, mines);
    game.run ();
    return 0;
}
