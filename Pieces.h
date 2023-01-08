/*****************************************************************************************/
/* File: Pieces.h 
/* Desc: All the Tetris pieces with their respective rotations and displacements for the hotspot
/*****************************************************************************************/

#ifndef _PIECES_
#define _PIECES_

namespace Pieces 
{
	int GetBlockType		(int piece, int rotation, int x, int y);
	int GetXInitialPosition (int piece, int rotation);
	int GetYInitialPosition (int piece, int rotation);
}

#endif // _PIECES_
