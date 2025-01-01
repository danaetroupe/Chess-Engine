#pragma once
#include <raylib.h>
#include <cstdint>
#include <string>

enum Type { PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING };
enum CColor { WWHITE, BBLACK };
struct Piece {
	Type type;
	CColor color;
	Piece(Type t, CColor c) {
		type = t;
		color = c;
	}
};
typedef uint64_t bitboard;

class Board
{
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

	bitboard allWhite;
	bitboard allBlack;
	bitboard allPieces;

	// Rendering
	Texture2D texture;

public:
	Board();
	~Board() {};

	bitboard PositionToBitboard(char[2]);
	bool MovePiece(char[2], char[2]);
	void UpdateBoard(bitboard, bitboard);
	bitboard* GetIndividalBoard(bitboard);

	// Rendering functions
	void Show(int);
	void Hide();
	void DrawPiece(bitboard, int, int, int, std::string);

	Rectangle GetTarget(int, float);

	bool MovePawn(bitboard, bitboard, CColor);
	bool MoveKnight(bitboard, bitboard);

	// Testing Function
	void RunTests();
};

