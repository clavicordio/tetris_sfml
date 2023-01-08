/*****************************************************************************************
/* File: Board.cpp
/* Desc: Board of the game. A matrix of n x n holes.
/*****************************************************************************************/

#include "Board.h"
#include <iostream>


Board::Board(int screen_width, int screen_height)
{
	this->screen_width_ = screen_width;
	this->screen_height_ = screen_height;

	//Init the board blocks with free positions
	this->InitBoard();
}

void Board::InitBoard()
{
	for (int i = 0; i < Board::kBoardWidth; i++)
		for (int j = 0; j < Board::kBoardHeight; j++)
			this->board_[i][j] = Board::ePosFree;
}

/* 
======================================									
Store a piece in the board by filling the blocks

Parameters:

>> x:		Horizontal position in blocks
>> y:		Vertical position in blocks
>> piece:	Piece to draw
>> rotation:	1 of the 4 possible rotations
====================================== 
*/
void Board::StorePiece (int x, int y, int piece, int rotation)
{
	// Store each block of the piece into the board
	for (int i1 = x, i2 = 0; i1 < x + Board::kPieceBlocks; i1++, i2++)
	{
		for (int j1 = y, j2 = 0; j1 < y + Board::kPieceBlocks; j1++, j2++)
		{	
			// Store only the blocks of the piece that are not holes
			if (Pieces::GetBlockType (piece, rotation, j2, i2) != 0)		
				this->board_[i1][j1] = Board::ePosFilled;
		}
	}
}

/* 
======================================									
Check if the game is over becase a piece have achived the upper position

Returns true or false
====================================== 
*/
bool Board::IsGameOver() const
{
	//If the first line has blocks, then, game over
	for (int i = 0; i < Board::kBoardWidth; i++)
	{
		if (this->board_[i][0] == Board::ePosFilled) 
			return true;
	}

	return false;
}

/* 
======================================									
Delete a line of the board by moving all above lines down

Parameters:

>> y:		Vertical position in blocks of the line to delete
====================================== 
*/
void Board::DeleteLine (int y)
{
	// Moves all the upper lines one row down
	for (int j = y; j > 0; j--)
	{
		for (int i = 0; i < Board::kBoardWidth; i++)
		{
			this->board_[i][j] = this->board_[i][j-1];
		}
	}	
}

/* 
======================================									
Delete all the lines that should be removed

Returns number of lines deleted.
====================================== 
*/
int Board::DeletePossibleLines()
{
	int lines_deleted_count = 0;

	for (int j = 0; j < Board::kBoardHeight; j++)
	{
		int i = 0;
		while (i < Board::kBoardWidth)
		{
			if (this->board_[i][j] != Board::ePosFilled) 
				break;
			i++;
		}

		if (i == Board::kBoardWidth)
		{
			this->DeleteLine(j);
			lines_deleted_count += 1;
		}
	}

	return lines_deleted_count;
}

/* 
======================================									
Returns 1 (true) if the this block of the board is empty, 0 if it is filled

Parameters:

>> x:		Horizontal position in blocks
>> y:		Vertical position in blocks
====================================== 
*/
bool Board::IsFreeBlock (int x, int y) const
{
	return this->board_[x][y] == Board::ePosFree;
}

int Board::BoardPosition() const
{
	return this->screen_width_ / 2;
}

/* 
======================================									
Returns the horizontal position (isn pixels) of the block given like parameter

Parameters:

>> pos:	Horizontal position of the block in the board
====================================== 
*/
int Board::GetXPosInPixels (int pos) const
{	
	return (Board::BoardPosition() - Board::kBlockSize * Board::kBoardWidth / 2) + pos * Board::kBlockSize;
}

/* 
======================================									
Returns the vertical position (in pixels) of the block given like parameter

Parameters:

>> pos:	Horizontal position of the block in the board
====================================== 
*/
int Board::GetYPosInPixels (int pos) const
{
	return (this->screen_height_ - (Board::kBlockSize * Board::kBoardHeight)) + (pos * Board::kBlockSize);
}

/* 
======================================									
Check if the piece can be stored at this position without any collision
Returns true if the movement is  possible, false if it not possible

Parameters:

>> x:		Horizontal position in blocks
>> y:		Vertical position in blocks
>> piece:	Piece to draw
>> rotation:	1 of the 4 possible rotations
====================================== 
*/
bool Board::IsPossibleMovement (int x, int y, int piece, int rotation) const
{
	// Checks collision with pieces already stored in the board or the board limits
	// This is just to check the 5x5 blocks of a piece with the appropiate area in the board
	for (int i1 = x, i2 = 0; i1 < x + Board::kPieceBlocks; i1++, i2++)
	{
		for (int j1 = y, j2 = 0; j1 < y + Board::kPieceBlocks; j1++, j2++)
		{	
			// Check if the piece is outside the limits of the board
			if (i1 < 0 || i1 > Board::kBoardWidth - 1 || j1 > Board::kBoardHeight - 1)
			{
				if (Pieces::GetBlockType(piece, rotation, j2, i2) != 0)
					return 0;
			}

			// Check if the piece have collisioned with a block already stored in the map
			if (j1 >= 0)	
			{
				if ((Pieces::GetBlockType(piece, rotation, j2, i2) != 0) &&
					(!this->IsFreeBlock(i1, j1))	)
					return false;
			}
		}
	}

	// No collision
	return true;
}
