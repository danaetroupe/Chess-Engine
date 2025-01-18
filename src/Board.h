#pragma once
#include <raylib.h>
#include <cstdint>
#include <string>

typedef uint64_t bitboard;

class Board
{
	// Starting Positions
	bitboard whitePawns = 65280; // A1 is 1
	bitboard whiteRooks = 129;
	bitboard whiteKnights = 66;
	bitboard whiteBishops = 36;
	bitboard whiteQueens = 8;
	bitboard whiteKing = 16;

	bitboard blackPawns = 71776119061217280;
	bitboard blackRooks = 9295429630892703744;
	bitboard blackKnights = 4755801206503243776;
	bitboard blackBishops = 2594073385365405696;
	bitboard blackQueens = 576460752303423488;
	bitboard blackKing = 1152921504606846976;

	//bitboard whiteAttacking;
	//bitboard blackAttacking;

	bitboard allWhite;
	bitboard allBlack;
	bitboard allPieces;

	bitboard ranks[8]; // Horizontal rows 
	bitboard files[8]; // Vertical columns

	bitboard hold = 0;
	int holdX = 0;
	int holdY = 0;

	bool whiteKingMoved = false;
	bool blackKingMoved = false;
	bool whiteARookMoved = false;
	bool blackARookMoved = false;
	bool whiteHRookMoved = false;
	bool blackHRookMoved = false;

	bool whiteTurn = true;

	// Rendering
	Texture2D texture, pieces;
	Sound moveSound, captureSound, illegalSound, gameEndSound;

public:
	Board();
	~Board();

	bool MovePiece(char[3], char[3]);
	bool MovePiece(bitboard,bitboard);
	bool IsValidPiece(bitboard);
	
	void Show(int, bitboard);

	// Testing Function
	void RunTests();
	void ShowBitboard(bitboard);
	
	bitboard PositionToBitboard(char[3]);
private:
	bitboard* GetIndividalBoard(bitboard);

	Rectangle GetTarget(int, float);

	bool MovePawn(bitboard, bitboard);
	bool MoveKnight(bitboard, bitboard);
	bool MoveKing(bitboard, bitboard);
	bool MoveBishop(bitboard, bitboard);
	bool MoveRook(bitboard, bitboard);
	bool MoveQueen(bitboard, bitboard);

	void UpdateBoard(bitboard, bitboard);
	void DrawPieces(bitboard, int, int, int, std::string);
	void DrawGrabPiece(int);

	int getRank(bitboard); // row
	int getFile(bitboard); // column
};

