/*****************************************************************************************
/* File: Board.h
/* Desc: Board of the game. A matrix of n x n holes.
/*****************************************************************************************/

#ifndef _BOARD_
#define _BOARD_

#include "Pieces.h"

class Board
{

public:

	Board (int screen_width, int screen_height);

	int GetXPosInPixels(int pos) const;
	int GetYPosInPixels(int pos) const;
	bool IsFreeBlock(int x, int y) const;
	bool IsGameOver() const;
	bool IsPossibleMovement(int x, int y, int piece, int rotation) const;

	void StorePiece(int x, int y, int piece, int rotation);
	int DeletePossibleLines();

	int BoardPosition() const;						// Center position of the board from the left of the screen

	static const int kBoardLineWidth = 6;			// Width of each of the two lines that delimit the board
	static const int kBlockSize = 16;				// Width and Height of each block of a piece
	static const int kBoardWidth = 10;				// Board width in blocks 
	static const int kBoardHeight = 20;				// Board height in blocks
	static const int kMinVerticalMargin = 5;		// Minimum vertical margin for the board limit 		
	static const int kMinHorizontalMargin = 5;		// Minimum horizontal margin for the board limit
	static const int kPieceBlocks = 5;				// Number of horizontal and vertical blocks of a matrix piece

private:

	enum { ePosFree, ePosFilled };			// POS_FREE = free position of the board; POS_FILLED = filled position of the board
	int board_ [kBoardWidth][kBoardHeight];	// Board that contains the pieces
	int screen_height_;
	int screen_width_;

	void InitBoard();
	void DeleteLine(int y);

};

#endif // _BOARD_
