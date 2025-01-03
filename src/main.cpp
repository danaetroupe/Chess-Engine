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

	// Initalize Game Board
	Board board;
	board.Show(SIZE);
	// game loop

	// Initalize User Input
	UserInput input;
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		SetWindowFocused();
		char* moves = input.GetMoves();
		bool valid = board.MovePiece(moves, moves + 2);
		if (valid) 
		{
			board.Show(SIZE);
		}
		else
		{
			std::cout << "Move was not valid. Please try again. " << std::endl;
		}
	}

	// cleanup
	// unload our texture so it can be cleaned up
	board.Hide();

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
