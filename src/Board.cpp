
#include "Board.h"
#include <iostream>
#include <string>

// Initalize board & render
Board::Board()
{
	allWhite = whitePawns | whiteRooks | whiteKnights | whiteBishops | whiteQueens | whiteKing;
	allBlack = blackPawns | blackRooks | blackKnights | blackBishops | blackQueens | blackKing;;
	allPieces = allWhite | allBlack;

	texture = LoadTexture("board.png");
};

void Board::Show(int SIZE)
{
	// drawing
	BeginDrawing();

	// Setup the back buffer for drawing (clear color and depth buffers)
	ClearBackground(BLACK);

	// Draw board
	Rectangle source = { 0, 0, texture.width, texture.height };
	Rectangle target = { 0, 0, SIZE, SIZE };
	
	DrawTexturePro(this->texture, source, target, {0,0}, 0, WHITE);

	// Draw pieces
	int INCREMENT = SIZE / 8;
	DrawPiece(whitePawns, 1000, 0, INCREMENT, "White Pawn");
	DrawPiece(whiteRooks, 800, 0, INCREMENT, "White Rook");
	DrawPiece(whiteBishops, 400, 0, INCREMENT, "White Bishop");
	DrawPiece(whiteKnights, 600, 0, INCREMENT, "White Knight");
	DrawPiece(whiteQueens, 200, 0, INCREMENT, "White Queen");
	DrawPiece(whiteKing, 0, 0, INCREMENT, "White King");

	DrawPiece(blackPawns, 1000, 200, INCREMENT, "Black Pawn");
	DrawPiece(blackRooks, 800, 200, INCREMENT, "Black Rook");
	DrawPiece(blackBishops, 400, 200, INCREMENT, "Black Bishop");
	DrawPiece(blackKnights, 600, 200, INCREMENT, "Black Knight");
	DrawPiece(blackQueens, 200, 200, INCREMENT, "Black Queen");
	DrawPiece(blackKing, 0, 200, INCREMENT, "Black King");

	// end the frame and get ready for the next one  (display frame, poll input, etc...)
	EndDrawing();
}

void Board::Hide()
{
	UnloadTexture(texture);
}

void Board::DrawPiece(uint64_t bitboard, int spriteX, int spriteY, int increment, std::string pieceName) {
	Texture2D pieces = LoadTexture("pieces.png");
	int square = 0;

	while (bitboard != 0) {
		if (bitboard % 2) {
			// Calculate position
			int y = square / 8;
			int x = square % 8;
			char column = 'A' + x;
			char row = '1' + y;
			Rectangle position = { x * increment, (7 - y) * increment, increment, increment }; // use 7 because <8
			Rectangle piece = { spriteX, spriteY, 200, 200 };
			DrawTexturePro(pieces, piece, position, { 0, 0 }, 0, WHITE);
			
			std::cout << pieceName << " on " << column << row << std::endl;
		}
		bitboard /= 2;
		square++;
	}
}

Rectangle Board::GetTarget(int position, float increment)
{
	// start from bottom left corner
	int y = position / 8;
	int x = position % 8;
	char column = 'A' + x;
	char row = '1' + y;
	std::cout << "Pawn on " << column << row << std::endl;
	return { x * increment, (7 - y) * increment, increment, increment }; // use 7 because <8
}