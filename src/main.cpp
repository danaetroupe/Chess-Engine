/***************************************\
========================================
			Danae Troupe
		    Chess Program
	Started: Sunday, December 29, 2024
========================================
\***************************************/

#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

// Program Files
#include "Board.h"
#include "UserInput.h"

#include <iostream>

/***************************************** Main Function *****************************************/
int main()
{
	int SIZE = 450;

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(SIZE, SIZE, "Chess Trainer");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("assets");

	// General Variables
	Vector2 mousePosition = { 0 };
	Vector2 windowPosition = { 500, 200 };
	Vector2 panOffset = mousePosition;
	bool dragWindow = false;

	// Initalize Game Board
	Board board;

	// Initalize User Input
	UserInput input;
	bool isDragging = false;
	bitboard grab = NULL;

	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		// char* moves = input.GetMoves();
		// bool valid = board.MovePiece(moves, moves + 3);
		// if (valid) 
		// {
		// 	board.Show(SIZE);
		// }
		// else
		// {
		// 	std::cout << "Move was not valid. Please try again. " << std::endl;
		// }

		//Check for mouse click
		if (!isDragging && IsMouseButtonDown(0))
		{
			isDragging = true;
			char* position = input.GetPieceFromMouse(SIZE);
			bitboard piece = board.PositionToBitboard(position);
			if (input.Validate(position) && board.IsValidPiece(piece)) { grab = piece; }
			delete[] position;
			// Get piece position
			// Check that piece is correct color (TODO)
			// If true, Show with bitboard and set isDragging to true
		}
		else if (isDragging && IsMouseButtonUp(0))
		{
			isDragging = false;
			char* position = input.GetPieceFromMouse(SIZE);
			bitboard endPos = board.PositionToBitboard(position);
			if (input.Validate(position)) { board.MovePiece(grab, endPos); }
			grab = NULL;
		}
		board.Show(SIZE, grab);
	}

	// cleanup
	// unload our texture so it can be cleaned up
	board.Hide();

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
