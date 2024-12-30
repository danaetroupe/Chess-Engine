#pragma once
#include <raylib.h>
#include <cstdint>
#include <string>

class Board
{
	uint64_t whitePawns = 65280;
	uint64_t whiteRooks = 129;
	uint64_t whiteKnights = 66;
	uint64_t whiteBishops = 36;
	uint64_t whiteQueens = 8;
	uint64_t whiteKing = 16;

	uint64_t blackPawns = 71776119061217280;
	uint64_t blackRooks = 9295429630892703744;
	uint64_t blackKnights = 4755801206503243776;
	uint64_t blackBishops = 2594073385365405696;
	uint64_t blackQueens = 576460752303423488;
	uint64_t blackKing = 1152921504606846976;

	uint64_t allWhite;
	uint64_t allBlack;
	uint64_t allPieces;

	// Rendering
	Texture2D texture;
	

public:
	Board();
	~Board() {};

	void Show(int);
	void Hide();
	Rectangle GetTarget(int, float);
	void DrawPiece(uint64_t, int, int, int, std::string);
};

