#pragma once
class UserInput
{
public:
	char* GetMoves();
	char* GetPieceFromMouse(int);
	bool Validate(char*);
};

