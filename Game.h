/******************************************************************************************/
// File: Game.h
// Desc: General class for the game
/******************************************************************************************/

#ifndef _GAME_
#define _GAME_

#include "Board.h"
#include "Pieces.h"
#include "IO.h"
#include <memory>

class Game
{
public:

	Game();

	bool IsRunning() const;
	void Loop();

private:

	static const int kScoreX = 10;			// Score position
	static const int kScoreY = 10;
	static const int kFontSize = 24;
	static const int kLineSpace = kFontSize / 3;

	int next_pos_x_, next_pos_y_;			// Position of the next piece (blocks)
	int next_piece_, next_rotation_;		// Kind and rotation of the next piece

	int score_;								// Number of cleared lines

	std::unique_ptr<Board> board_;
	std::unique_ptr<IO> io_;

	static int GetRand(int a, int b);

	void InitGame();

	void DrawScene();
	void DrawPiece(int x, int y, int piece, int rotation);
	void DrawBoard();

	void DrawScore();
	void DrawGameOver();
	void DrawTextNext();
	void DrawControls();

	void ProcessEvents();
	void GameLogic();

	void CreateNewPiece();

	int pos_x_, pos_y_;				// Position of the piece that is falling down
	int piece_, rotation_;			// Kind and rotation the piece that is falling down
};

#endif // _GAME
