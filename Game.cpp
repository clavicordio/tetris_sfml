/*****************************************************************************************
/* File: Game.cpp
/* Desc: General class for the game
/*****************************************************************************************/

// ------ Includes -----
#include "Game.h"
#include <assert.h>

Game::Game() 
{
	this->io_ = std::make_unique<IO>();
	this->board_ = std::make_unique<Board>(this->io_->GetScreenWidth(), this->io_->GetScreenHeight());
	this->InitGame();
}

void Game::Loop()
{
	this->DrawScene();			
	this->ProcessEvents();
	this->GameLogic();
}

bool Game::IsRunning() const
{
	return this->io_->WindowIsOpen();
}

/* 
======================================									
Get a random int between to integers

Parameters:
>> a: First number
>> b: Second number
====================================== 
*/
int Game::GetRand (int a, int b)
{
	return rand() % (b - a + 1) + a;
}

void Game::InitGame()
{
	srand((unsigned int) time(NULL));

	// First piece
	this->piece_			= Game::GetRand(0, 6);
	this->rotation_			= Game::GetRand(0, 3);
	this->pos_x_ 			= (Board::kBoardWidth / 2) + Pieces::GetXInitialPosition(this->piece_, this->rotation_);
	this->pos_y_ 			= Pieces::GetYInitialPosition(this->piece_, this->rotation_);

	//  Next piece
	this->next_piece_ 		= Game::GetRand(0, 6);
	this->next_rotation_ 	= Game::GetRand(0, 3);
	this->next_pos_x_ 		= Board::kBoardWidth + 5;
	this->next_pos_y_ 		= 5;
}

/* 
======================================									
Create a random piece
====================================== 
*/
void Game::CreateNewPiece()
{
	// New piece
	this->piece_		= this->next_piece_;
	this->rotation_		= this->next_rotation_;
	this->pos_x_ 		= (Board::kBoardWidth / 2) + Pieces::GetXInitialPosition (this->piece_, this->rotation_);
	this->pos_y_ 		= Pieces::GetYInitialPosition (this->piece_, this->rotation_);

	// Random next piece
	this->next_piece_ 		= GetRand (0, 6);
	this->next_rotation_ 	= GetRand (0, 3);
}

/* 
======================================									
Draw piece

Parameters:

>> x:		Horizontal position in blocks
>> y:		Vertical position in blocks
>> piece:	Piece to draw
>> rotation:	1 of the 4 possible rotations
====================================== 
*/
void Game::DrawPiece (int x, int y, int piece, int rotation)
{
	// Obtain the position in pixel in the screen of the block we want to draw
	int pixels_x = this->board_->GetXPosInPixels(x);
	int pixels_y = this->board_->GetYPosInPixels(y);

	// Travel the matrix of blocks of the piece and draw the blocks that are filled
	for (int i = 0; i < Board::kPieceBlocks; i++)
	{
		for (int j = 0; j < Board::kPieceBlocks; j++)
		{
			int block_type = Pieces::GetBlockType(piece, rotation, j, i);

			if (block_type == 0)
				continue;

			IO::Color color = (block_type == 1) ? IO::eGreen : IO::eCyan;	// Color of the block 
			
			this->io_->DrawRectangle(pixels_x + i * Board::kBlockSize, 
				pixels_y + j * Board::kBlockSize, 
				(pixels_x + i * Board::kBlockSize) + Board::kBlockSize - 1, 
				(pixels_y + j * Board::kBlockSize) + Board::kBlockSize - 1, 
				color);
		}
	}
}

/* 
======================================									
Draw board

Draw the two lines that delimit the board
====================================== 
*/
void Game::DrawBoard ()
{
	// Calculate the limits of the board in pixels	
	int x1 = this->board_->BoardPosition() - (Board::kBlockSize * (Board::kBoardWidth / 2)) - 1;
	int x2 = this->board_->BoardPosition() + (Board::kBlockSize * (Board::kBoardWidth / 2));
	int y  = this->io_->GetScreenHeight() - (Board::kBlockSize * Board::kBoardHeight);
	
	// Check that the vertical margin is not to small
	assert (y > Board::kMinVerticalMargin);

	// Rectangles that delimits the board
	this->io_->DrawRectangle(x1 - Board::kBoardLineWidth, y, x1, this->io_->GetScreenHeight() - 1, IO::eBlue);
	this->io_->DrawRectangle(x2, y, x2 + Board::kBoardLineWidth, this->io_->GetScreenHeight() - 1, IO::eBlue);
	
	// Check that the horizontal margin is not to small
	assert (x1 > Board::kMinHorizontalMargin);

	// Drawing the blocks that are already stored in the board
	x1 += 1;
	for (int i = 0; i < Board::kBoardWidth; i++)
	{
		for (int j = 0; j < Board::kBoardHeight; j++)
		{	
			// Check if the block is filled, if so, draw it
			if (!this->board_->IsFreeBlock(i, j))
				this->io_->DrawRectangle(x1 + i * Board::kBlockSize,
										y + j * Board::kBlockSize, 
										(x1 + i * Board::kBlockSize) + Board::kBlockSize - 1, 
										(y + j * Board::kBlockSize) + Board::kBlockSize - 1, 
										IO::eRed);
		}
	}	
}

void Game::DrawScore()
{
	this->io_->DrawText(Game::kScoreX, 
		Game::kScoreY, 
		"Score: " + std::to_string(this->score_), 
		Game::kFontSize, 
		IO::eGreen);
}

void Game::DrawGameOver()
{
	assert(this->board_->IsGameOver());
	// align with score
	this->io_->DrawText(
		Game::kScoreX,
		Game::kScoreY + Game::kFontSize + Game::kLineSpace,
		"GAME OVER",
		Game::kFontSize,
		IO::eGreen);
}

void Game::DrawTextNext()
{
	int x = this->board_->GetXPosInPixels(this->next_pos_x_);
	int y = this->board_->GetYPosInPixels(this->next_pos_y_) - Game::kFontSize - Game::kLineSpace;

	this->io_->DrawText(
		x,
		y,
		"NEXT:",
		Game::kFontSize,
		IO::eGreen
	);
}

void Game::DrawControls()
{
	int board_height_pixels = this->board_->GetYPosInPixels(Board::kBoardHeight);
	// align with score
	int x = Game::kScoreX;
	int y = board_height_pixels - Game::kScoreY - Game::kFontSize * 2 - Game::kLineSpace;

	this->io_->DrawText(
		x,
		y,
		"Rotate=Z\nDrop=X",
		Game::kFontSize,
		IO::eGreen
	);

}

/* 
======================================									
Draw scene

Draw all the objects of the scene
====================================== 
*/
void Game::DrawScene ()
{	
	this->io_->ClearScreen();

	this->DrawBoard ();							// Draw the delimitation lines and blocks stored in the board
	this->DrawPiece		(this->pos_x_, 
						this->pos_y_, 
						this->piece_, 
						this->rotation_);		// Draw the playing piece
	this->DrawPiece		(this->next_pos_x_, 
						this->next_pos_y_, 
						this->next_piece_, 
						this->next_rotation_);	// Draw the next piece

	this->DrawScore();

	if (this->board_->IsGameOver())
	{
		this->DrawGameOver();
	}

	this->DrawTextNext();
	this->DrawControls();

	this->io_->UpdateScreen();					// Put the graphic context in the screen
}

void Game::ProcessEvents()
{
	IO::Event event;
	while (this->io_->PollEvent(&event))
	{
		if (event.type == IO::eGameClosed)
		{
			this->io_->CloseWindow();
		}
		else if (event.type == IO::eKeyPressed)
		{
			if (event.key == IO::eKeyEscape)
			{
				this->io_->CloseWindow();
				break;
			}

			if (this->board_->IsGameOver())
				continue;

			// Process keys other than Esc

			switch (event.key)
			{
			case (IO::eKeyRight):
				if (this->board_->IsPossibleMovement(this->pos_x_ + 1, this->pos_y_, this->piece_, this->rotation_))
					this->pos_x_++;
				break;

			case (IO::eKeyLeft):
				if (this->board_->IsPossibleMovement(this->pos_x_ - 1, this->pos_y_, this->piece_, this->rotation_))
					this->pos_x_--;
				break;

			case (IO::eKeyDown):
				if (this->board_->IsPossibleMovement(this->pos_x_, this->pos_y_ + 1, this->piece_, this->rotation_))
					this->pos_y_++;
				break;

			case (IO::eKeyDrop):
				// Check collision from up to down
				while (this->board_->IsPossibleMovement(this->pos_x_, this->pos_y_, this->piece_, this->rotation_))
					this->pos_y_++;

				this->board_->StorePiece(this->pos_x_, this->pos_y_ - 1, this->piece_, this->rotation_);

				this->score_ += this->board_->DeletePossibleLines();

				if (!this->board_->IsGameOver())
					this->CreateNewPiece();

				break;

			case (IO::eKeyRotate):
				if (this->board_->IsPossibleMovement(this->pos_x_, this->pos_y_, this->piece_, (this->rotation_ + 1) % 4))
					this->rotation_ = (this->rotation_ + 1) % 4;
				break;
			}
		}

	}
}

void Game::GameLogic()
{
	if (this->board_->IsGameOver())
		return;
	
	// Vertical movement
	const int kWaitTime = 700;		// Number of milliseconds that the piece remains before going 1 block down

	if (this->io_->ClockGetElapsedTimeMS() > kWaitTime)
	{
		if (this->board_->IsPossibleMovement(this->pos_x_, this->pos_y_ + 1, this->piece_, this->rotation_))
		{
			this->pos_y_++;
		}
		else
		{
			this->board_->StorePiece(this->pos_x_, this->pos_y_, this->piece_, this->rotation_);

			this->score_ += this->board_->DeletePossibleLines();

			if (!this->board_->IsGameOver())
			{
				this->CreateNewPiece();
			}
		}
		this->io_->ClockReset();
	}
}