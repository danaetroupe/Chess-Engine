
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "Board.h"

/**************************************** Constructor ****************************************/
Board::Board()
{
	allWhite = whitePawns | whiteRooks | whiteKnights | whiteBishops | whiteQueens | whiteKing;
	allBlack = blackPawns | blackRooks | blackKnights | blackBishops | blackQueens | blackKing;;
	allPieces = allWhite | allBlack;

	// Fill out rows
	for (bitboard i = 0; i < 8; i++)
	{
		bitboard value = 0;
		for (bitboard j = 0; j < 8; j++) {
			value += (bitboard)1 << ((bitboard)8 * i) << j;
		}
		ranks[i] = value;
	}

	// Fill out columns
	for (bitboard i = 0; i < 8; i++)
	{
		bitboard value = 0;
		for (bitboard j = 0; j < 8; j++) {
			value += (bitboard)1 << i << ((bitboard)8 * j);
		}
		files[i] = value;
	}

	// Load textures
	texture = LoadTexture("board.png");
	pieces = LoadTexture("pieces.png");

	// Initalize audio
	InitAudioDevice();
	moveSound = LoadSound("audio/move-self.mp3");
	captureSound = LoadSound("audio/capture.mp3");
	illegalSound = LoadSound("audio/illegal.mp3");
	gameEndSound = LoadSound("audio/game-end.mp3");
};

Board::~Board()
{
	CloseAudioDevice();
	UnloadTexture(texture);
	UnloadTexture(pieces);
}

/**************************************** Show Board ****************************************/
void Board::Show(int SIZE, bitboard grabPiece = NULL)
{
	hold = grabPiece;

	// drawing
	BeginDrawing();

	// Setup the back buffer for drawing (clear color and depth buffers)
	ClearBackground(BLACK);

	// Draw board
	Rectangle source = { 0, 0, texture.width, texture.height };
	Rectangle target = { 0, 0, SIZE-50, SIZE - 50};
	
	DrawTexturePro(this->texture, source, target, {0,0}, 0, WHITE); 
	// todo: draw squares instead to incorporate custom themes

	// Draw pieces
	int INCREMENT = (SIZE-50) / 8;
	DrawPieces(whitePawns, 1000, 0, INCREMENT, "White Pawn");
	DrawPieces(whiteRooks, 800, 0, INCREMENT, "White Rook");
	DrawPieces(whiteBishops, 400, 0, INCREMENT, "White Bishop");
	DrawPieces(whiteKnights, 600, 0, INCREMENT, "White Knight");
	DrawPieces(whiteQueens, 200, 0, INCREMENT, "White Queen");
	DrawPieces(whiteKing, 0, 0, INCREMENT, "White King");

	DrawPieces(blackPawns, 1000, 200, INCREMENT, "Black Pawn");
	DrawPieces(blackRooks, 800, 200, INCREMENT, "Black Rook");
	DrawPieces(blackBishops, 400, 200, INCREMENT, "Black Bishop");
	DrawPieces(blackKnights, 600, 200, INCREMENT, "Black Knight");
	DrawPieces(blackQueens, 200, 200, INCREMENT, "Black Queen");
	DrawPieces(blackKing, 0, 200, INCREMENT, "Black King");

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

	// Draw grab piece
	if (grabPiece) { DrawGrabPiece(INCREMENT); }

	// end the frame and get ready for the next one  (display frame, poll input, etc...)
	EndDrawing();
}

/**************************************** Draw Pieces ****************************************/
void Board::DrawPieces(bitboard board, int spriteX, int spriteY, int increment, std::string pieceName) {
	int square = 0;
	if (board & hold) { 
		holdX = spriteX; holdY = spriteY; 
		board = board ^ hold;
	}
	

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

/**************************************** Draw Pieces ****************************************/
void Board::DrawGrabPiece(int increment) {
	bitboard board = hold;
	int square = 0;
	while (board != 0) {
		if (board % 2) {
			// Calculate position
			int y = square / 8;
			int x = square % 8;
			Rectangle rect = { x * increment, (7 - y) * increment, increment, increment };
			DrawRectangleLinesEx(rect, 5, RAYWHITE);
			break;
		}
		board /= 2;
		square++;
	}

	// Get mouse position
	// Draw square at bitboard position
	// Draw piece under mouse cursor (draw on midway point)
	Rectangle piece = { holdX, holdY, 200, 200 };
	Rectangle position = { GetMouseX() - increment/2, GetMouseY() - increment/2, increment, increment};
	DrawTexturePro(pieces, piece, position, { 0, 0 }, 0, WHITE);
}

/******************************** Determine Target Square From Position ********************************/
Rectangle Board::GetTarget(int position, float increment)
{
	// start from bottom left corner
	int y = position / 8;
	int x = position % 8;
	char column = 'A' + x;
	char row = '1' + y;
	return { x * increment, (7 - y) * increment, increment, increment }; // use 7 because <8
}

/*********************************** Determine Bitboard From Position ***********************************/
bitboard Board::PositionToBitboard(char position[3])
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
bool Board::MovePiece(char p1[3], char p2[3]) {
	bitboard startPos = PositionToBitboard(p1);
	bitboard endPos = PositionToBitboard(p2);

	return MovePiece(startPos, endPos);
}

/***************************************** Move Piece *****************************************/
bool Board::MovePiece(bitboard startPos, bitboard endPos) {
	// Make sure startPos is a valid piece
	if (!(startPos & allPieces)) {
		return false; // TODO: Validate in user input (don't allow user to select square without a piece!)
	}

	// Make sure correct color is moving
	if ((whiteTurn && (startPos & allBlack)) || (!whiteTurn && (startPos & allWhite)))
	{
		return false;
	}

	// Check for color collision
	if ((startPos & allWhite && endPos & allWhite) || (startPos & allBlack && endPos & allBlack)) { return false; }

	// Check for lack of movement 
	if (startPos == endPos) { return false; }

	bool valid = false;
	if (startPos & (whitePawns | blackPawns))
	{
		valid = MovePawn(startPos, endPos);
	}
	else if (startPos & (whiteRooks | blackRooks)) {
		valid = MoveRook(startPos, endPos);
	}
	else if (startPos & (whiteBishops | blackBishops)) {
		valid = MoveBishop(startPos, endPos);
	}
	else if (startPos & (whiteKnights | blackKnights)) {
		valid = MoveKnight(startPos, endPos);
	}
	else if (startPos & (whiteQueens | blackQueens)) {
		valid = MoveQueen(startPos, endPos);
	}
	else if (startPos & (whiteKing | blackKing)) {
		valid = MoveKing(startPos, endPos);
	}

	if (valid)
	{
		if (endPos & allPieces) { PlaySound(captureSound); }
		else { PlaySound(moveSound); }
		UpdateBoard(startPos, endPos);
		whiteTurn = (endPos & allBlack);
	}
	else 
	{ 
		PlaySound(illegalSound); 
	}

	return valid;
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
			return true; 
		}
 	}
	return false;
}

/***************************************** Move King *****************************************/
bool Board::MoveKing(bitboard startPos, bitboard endPos)
{
	std::vector<bitboard> validMoves;
	int rank = getRank(startPos);
	int file = getFile(startPos);
	bitboard color = (startPos & allWhite) ? allWhite : allBlack;

	
	bitboard numbers[]{ 1, 7, 8, 9 };
	for (bitboard number : numbers)
	{
		// TODO: Check that king is not in check
		if (!(startPos << number & color)) { validMoves.push_back(startPos << number); }
		if (!(startPos >> number & color)) { validMoves.push_back(startPos >> number); }
	}

	if ((allWhite & startPos) && !whiteKingMoved) 
	{
		if (endPos == startPos >> 2 && !whiteARookMoved) 
		{
			if (!(startPos >> 1 & allPieces))
			{
				MovePiece(1, 8); // Move A Rook
				return true;
			}
		}
		if (endPos == startPos << 2 && !whiteHRookMoved)
		{
			if (!(startPos << 1 & allPieces))
			{
				MovePiece(128, 32); // Move H Rook
				return true;
			}
		}
	}
		
	if ((allBlack & startPos) && !blackKingMoved) 
	{
		if (endPos == startPos >> 2 && !blackARookMoved)
		{
			return MovePiece(72057594037927936, 576460752303423488); // Move A Rook
		}
		if (endPos == startPos << 2 && !blackHRookMoved)
		{
			return MovePiece(9223372036854775808, 2305843009213693952); // Move H Rook
		}
	}

	// Check for endpos
	for (bitboard move : validMoves) {
		if (move == endPos) {
			if (color == allWhite) { whiteKingMoved = true; }
			else { blackKingMoved = true; }
			return true;
		}
	}
	return false;
}

/***************************************** Move Knight *****************************************/
bool Board::MoveKnight(bitboard startPos, bitboard endPos)
{
	std::vector<bitboard> validMoves;
	int rank = getRank(startPos);
	int file = getFile(startPos);
	bitboard color = (startPos & allWhite) ? allWhite : allBlack;

	if (rank < 6 && file > 0 && !(startPos << 15 & color)) {
		validMoves.push_back(startPos << 15);
	}
	if (rank < 6 && file < 7 && !(startPos << 17 & color)) {
		validMoves.push_back(startPos << 17);
	}
	if (rank < 7 && file < 6 && !(startPos << 10 & color)) {
		validMoves.push_back(startPos << 10);
	}
	if (rank < 7 && file > 1 && !(startPos << 6 & color)) {
		validMoves.push_back(startPos << 6);
	}
	if (rank > 0 && file > 1 && !(startPos >> 10 & color)) {
		validMoves.push_back(startPos >> 10);
	}
	if (rank > 1 && file > 0 && !(startPos >> 17 & color)) {
		validMoves.push_back(startPos >> 17);
	}
	if (rank > 1 && file < 7 && !(startPos >> 15 & color)) {
		validMoves.push_back(startPos >> 15);
	}
	if (rank > 0 && file < 6 && !(startPos >> 6 & color)) {
		validMoves.push_back(startPos >> 6);
	}

	// Check for endpos
	for (bitboard move : validMoves) {
		if (move == endPos) {
			return true;
		}
	}
	return false;
}

/***************************************** Move Bishop *****************************************/
bool Board::MoveBishop(bitboard startPos, bitboard endPos)
{
	bitboard color = (startPos & allWhite) ? allWhite : allBlack;
	
	int startRank = getRank(startPos);
	int startFile = getFile(startPos);
	int endRank = getRank(endPos);
	int endFile = getFile(endPos);

	if (abs(startRank - endRank) == abs(startFile - endFile) && !(endPos & color)) // Check to make sure movement is the same
	{
		// Check there are no pieces blocking
		int diff = abs(startRank - endRank);
		int rankIncrement = (endRank - startRank) / abs(startRank - endRank);
		int fileIncrement = (endFile - startFile) / abs(startFile - endFile);

		for (int i = 1; i < diff; i++)
		{
			char position[]{ 'A' + startFile + (i * fileIncrement), '1' + startRank + (i * rankIncrement) };
			if (PositionToBitboard(position) & allPieces) { return false; }
		}
		return true;
	}

	return false;
}

/***************************************** Move Rook *****************************************/
bool Board::MoveRook(bitboard startPos, bitboard endPos)
{
	bitboard color = (startPos & allWhite) ? allWhite : allBlack;

	int startRank = getRank(startPos);
	int startFile = getFile(startPos);
	int endRank = getRank(endPos);
	int endFile = getFile(endPos);

	if (startRank == endRank && startFile != endFile) // A-H
	{
		int diff = abs(startFile - endFile);
		int fileIncrement = (endFile - startFile) / abs(startFile - endFile);

		for (int i = 1; i < diff; i++)
		{
			char position[]{ 'A' + startFile + (i * fileIncrement), '1' + endRank, '\0'};
			if (PositionToBitboard(position) & allPieces) { return false; }
		}
		return true;
	}
	else if (startFile == endFile && startRank != endRank) // 1-8
	{
		int diff = abs(startRank - endRank);
		int rankIncrement = (endRank - startRank) / abs(startRank - endRank);

		for (int i = 1; i < diff; i++)
		{
			char position[]{ 'A' + endFile, '1' + startRank + (i*rankIncrement), '\0' };
			if (PositionToBitboard(position) & allPieces) { return false; }
		}
		return true;
	}
	return false;
}

/***************************************** Move Queen *****************************************/
bool Board::MoveQueen(bitboard startPos, bitboard endPos)
{
	return (MoveBishop(startPos, endPos) || MoveRook(startPos, endPos));
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

	// Update attack boards

}

/***************** Determine the individual bitboard that the piece resides on *****************/
bitboard* Board::GetIndividalBoard(bitboard piece)
{
	bitboard* boards[]{ &whitePawns, &blackPawns, 
		&whiteRooks, &blackRooks,
		&whiteKnights, &blackKnights,
		&whiteBishops, &blackBishops, 
		&whiteQueens, &blackQueens, 
		&whiteKing, &blackKing };

	for (bitboard* board: boards)
	{
		if (*board & piece) { return board; }
	}
	return 0;
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

/******************************************* Show Bitboard *******************************************/
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

/********************************************** Get Rank **********************************************/
int Board::getRank(bitboard piece)
{
	for (int i = 0; i < 8; i++)
	{
		if (ranks[i] & piece) { return i; }
	}
	return -1;
}

/********************************************** Get File **********************************************/
int Board::getFile(bitboard piece)
{
	for (int i = 0; i < 8; i++)
	{
		if (files[i] & piece) { return i; }
	}
	return -1;
}

/**************************************** Check If Piece Is Valid ****************************************/
bool Board::IsValidPiece(bitboard piece)
{
	return ((whiteTurn && (piece & allWhite)) || (!whiteTurn && (piece & allBlack)));
}