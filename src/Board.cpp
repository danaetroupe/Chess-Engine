
#include <iostream>
#include <string>
#include <vector>

#include "Board.h"

/**************************************** Constructor ****************************************/
Board::Board()
{
	allWhite = whitePawns | whiteRooks | whiteKnights | whiteBishops | whiteQueens | whiteKing;
	allBlack = blackPawns | blackRooks | blackKnights | blackBishops | blackQueens | blackKing;;
	allPieces = allWhite | allBlack;

	texture = LoadTexture("board.png");
};

/**************************************** Show Board ****************************************/
void Board::Show(int SIZE)
{
	// drawing
	BeginDrawing();

	// Setup the back buffer for drawing (clear color and depth buffers)
	ClearBackground(BLACK);

	// Draw board
	Rectangle source = { 0, 0, texture.width, texture.height };
	Rectangle target = { 0, 0, SIZE-50, SIZE - 50};
	
	DrawTexturePro(this->texture, source, target, {0,0}, 0, WHITE);

	// Draw pieces
	int INCREMENT = (SIZE-50) / 8;
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

	// Draw labels
	for (int i = 0; i < 8; i++)
	{
		char* letter = new char[2]; // Allocate space for 1 character + null terminator
		letter[0] = 'A' + i;        // Assign the calculated character
		letter[1] = '\0';           // Null-terminate the string
		DrawText(letter, INCREMENT*i + 10, SIZE - 40, 24, RAYWHITE);
		letter[0] = '1' + i;        // Assign the calculated character
		letter[1] = '\0';           // Null-terminate the string
		DrawText(letter, SIZE - 40, INCREMENT*(7-i)+10, 24, RAYWHITE);
	}

	// end the frame and get ready for the next one  (display frame, poll input, etc...)
	EndDrawing();
}

/**************************************** Hide Board ****************************************/
void Board::Hide()
{
	UnloadTexture(texture);
}

/**************************************** Draw Piece ****************************************/
void Board::DrawPiece(bitboard board, int spriteX, int spriteY, int increment, std::string pieceName) {
	Texture2D pieces = LoadTexture("pieces.png");
	int square = 0;

	while (board != 0) {
		if (board % 2) {
			// Calculate position
			int y = square / 8;
			int x = square % 8;
			char column = 'A' + x;
			char row = '1' + y;
			Rectangle position = { x * increment, (7 - y) * increment, increment, increment }; // use 7 because <8
			Rectangle piece = { spriteX, spriteY, 200, 200 };
			DrawTexturePro(pieces, piece, position, { 0, 0 }, 0, WHITE);
			
			//std::cout << pieceName << " on " << column << row << std::endl;
		}
		board /= 2;
		square++;
	}
}

/******************************** Determine Target Square From Position ********************************/
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

/*********************************** Determine Bitboard From Position ***********************************/
bitboard Board::PositionToBitboard(char position[2])
{
	char column = position[0]; // expecting: A, B, C...
	char row = position[1]; // expecting : 1, 2, 3...

	// Convert first to square number
	int shift = (row - '1')*8;
	//std::cout << shift << std::endl;
	if (column <= 'z' && column >= 'a') 
	{
		shift += column - 'a';
	}
	else
	{
		shift += column - 'A';
	}
	//std::cout << shift << std::endl;
	bitboard board = (uint64_t)1 << shift;
	return board;
}

/***************************************** Move Piece *****************************************/
bool Board::MovePiece(char p1[2 ], char p2[2]) {
	bitboard startPos = PositionToBitboard(p1);
	bitboard endPos = PositionToBitboard(p2);

	// Make sure startPos is a valid piece
	if (!(startPos & allPieces)) {
		return false; // TODO: Validate in user input (don't allow user to select square without a piece!)
	}

	// Check for color collision
	if ((startPos & allWhite && endPos & allWhite) || (startPos & allBlack && endPos & allBlack)) { return false; }
	
	if (startPos & (whitePawns | blackPawns)) 
	{
		return MovePawn(startPos, endPos);
	}
	else if (startPos & (whiteRooks | blackRooks)) {
		return true;
	}
	else if (startPos & (whiteBishops | blackBishops)) {
		return true;
	}
	else if (startPos & (whiteKnights | blackBishops)) {
		return true;
	}
	else if (startPos & (whiteQueens | blackQueens)) {
		return true;
	}
	else if (startPos & (whiteKing | blackKing)) {
		return true;
	}
	else {
		std::cout << "[ERROR] Type at " << p1 << " not recognized." << std::endl;
	}

}

/***************************************** Move Pawn *****************************************/
bool Board::MovePawn(bitboard start, bitboard end) {
	bool isWhite = start & allWhite;
	bitboard startRow = (isWhite) ? 65280 : 71776119061217280;
	bitboard endRow = (isWhite) ? 18374686479671623680 : 255;
	std::vector<bitboard> validMoves;

	if (isWhite)
	{
		// Check for spaces ahead
		if (!(start << 8 & allPieces))
		{
			validMoves.push_back(start << 8);
			if ((startRow & start) && !(start << 16 & allPieces))
			{
				validMoves.push_back(start << 16);
			}
		}
		// Check for ability to take
		if (start << 7 & allBlack) {
			validMoves.push_back(start << 7);
		}
		if (start << 9 & allBlack) {
			validMoves.push_back(start << 9);
		}
	}
	else
	{
		// Check for spaces ahead
		if (!(start >> 8 & allPieces))
		{
			validMoves.push_back(start >> 8);
			if ((startRow & start) && !(start >> 16 & allPieces))
			{
				validMoves.push_back(start >> 16);
			}
		}
		// Check for ability to take
		if (start >> 7 & allWhite) {
			validMoves.push_back(start >> 7);
		}
		if (start >> 9 & allWhite) {
			validMoves.push_back(start >> 9);
		}
	}

	// Search valid moves to see if end position is valid
	for (bitboard move : validMoves) {
		if (move == end) {
			// Handle pawn promotion
			if (end & endRow) 
			{
				if (isWhite) {
					whitePawns = whitePawns ^ start;
					whiteQueens = whiteQueens ^ start;
				}
				else
				{
					blackPawns = blackPawns ^ start;
					blackQueens = blackQueens ^ start;
				}
			}
			UpdateBoard(start, end);
			return true; 
		}
	}
	return false;
}

/***************************************** Update Board *****************************************/
void Board::UpdateBoard(bitboard startPos , bitboard endPos)
{
	// If there is a piece on the end position remove it first
	if (endPos & allPieces)
	{
		bitboard* oppOboard = GetIndividalBoard(endPos);
		*oppOboard = *oppOboard ^ endPos;
		if (endPos & allWhite) {
			allWhite = allWhite ^ endPos;
		}
		else
		{
			allBlack = allBlack ^ endPos;
		}
	}
	
	// Update piece that is moving
	bitboard* attackBoard = GetIndividalBoard(startPos);
	*attackBoard = *attackBoard ^ startPos ^ endPos;
	if (startPos & allWhite) {
		allWhite = allWhite ^ startPos ^ endPos;
	}
	else
	{
		allBlack = allBlack ^ startPos ^ endPos;
	}
	allPieces = allBlack | allWhite;
}

/***************** Determine the individual bitboard that the piece resides on *****************/
bitboard* Board::GetIndividalBoard(bitboard piece)
{
	bitboard* boards[]{ &whitePawns, &blackPawns, 
		&whiteRooks, &blackRooks, 
		&whiteBishops, &blackBishops, 
		&whiteQueens, &blackQueens, 
		&whiteKing, &blackKing };

	for (bitboard* board: boards)
	{
		if (*board & piece) { return board; }
	}
}

/***************************************** Move Knight *****************************************/
bool Board::MoveKnight(bitboard startPos, bitboard endPos)
{
	// function not completeley implemented
	std::vector<bitboard> validMoves;
	int numbers[]{ 6, 8, 15, 17 };
	return true;
}

/***************************************** Run Tests *****************************************/
void Board::RunTests()
{
	std::cout << "[RUNNING UNIT TESTS]" << std::endl;

	std::cout << "[PositionToBitboard]" << std::endl;
	std::cout << "A1: " << PositionToBitboard("A1") << "\nExpecting: 1" << std::endl;
	std::cout << "B1: " << PositionToBitboard("B1") << "\nExpecting: 2" << std::endl;
	std::cout << "C1: " << PositionToBitboard("C1") << "\nExpecting: 4" << std::endl;
	std::cout << "A2: " << PositionToBitboard("A2") << "\nExpecting: 256" << std::endl;
	std::cout << "H3: " << PositionToBitboard("H3") << "\nExpecting: 8388608" << std::endl;
	std::cout << "D5: " << PositionToBitboard("D5") << "\nExpecting: 34359738368" << std::endl;
	std::cout << "H8: " << PositionToBitboard("H8") << "\nExpecting: 9223372036854775808" << std::endl;

	std::cout << "[Testing Pawns]" << std::endl;
	std::cout << "A2->A3: " << MovePiece("A2", "A3") << "   Expecting: 1" << std::endl;
	std::cout << "H2->H3: " << MovePiece("H2", "H3") << "   Expecting: 1" << std::endl;
	std::cout << "D7->D6: " << MovePiece("D7", "D6") << "   Expecting: 1" << std::endl;
	std::cout << "F2->F4: " << MovePiece("F2", "F4") << "   Expecting: 1" << std::endl;
	std::cout << "F2->E3: " << MovePiece("F2", "E3") << "   Expecting: 0" << std::endl;
	std::cout << "F2->A8: " << MovePiece("F2", "A8") << "   Expecting: 0" << std::endl;
	std::cout << "A2->B2: " << MovePiece("A2", "B2") << "   Expecting: 0" << std::endl;
	std::cout << "A2->A4: " << MovePiece("A2", "A4") << "   Expecting: 1" << std::endl;
}

/***************************************** Show Bitboard *****************************************/
void Board::ShowBitboard(bitboard board)
{
	for (int i = 8; i > 0; i--)
	{
		std::cout << i << ' ';
		for (int j = 0; j < 8; j++) 
		{
			char square[] = { char('A' + j), char('0' + i) };
			bitboard pos = PositionToBitboard(square);
			int val = (pos & board) ? 1 : 0;
			std::cout << ' ' << val;
		}
		std::cout << std::endl;
	}
	std::cout << "\n ";
	for (int i = 0; i < 8; i++) {
		std::cout << ' ' << char('A' + i);
	}
	std::cout << std::endl;
}