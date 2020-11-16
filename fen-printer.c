/**************************************************************
fen-printer.c

Prints a chess board position from an FEN file. White pieces
are printed at the bottom, black pieces at the top.

Sample boards:
-------------------------	-------------------------
|bR|bN|bB|bQ|bK|bB|bN|bR|	|  |  |bK|  |  |bR|  |  |
|bP|bP|bP|bP|bP|bP|bP|bP|	|bP|bP|bP|  |  |  |  |  |
|  |  |  |  |  |  |  |  |	|  |bB|  |  |  |bQ|  |  |
|  |  |  |  |  |  |  |  |	|  |  |  |bN|wN|  |  |  |
|  |  |  |  |  |  |  |  |	|wP|wP|  |wP|bP|  |wQ|  |
|  |  |  |  |  |  |  |  |	|  |  |wP|  |wP|  |  |wP|
|wP|wP|wP|wP|wP|wP|wP|wP|	|  |  |  |  |  |wP|wP|  |
|wR|wN|wB|wQ|wK|wB|wN|wR|	|  |  |wR|  |wK|wR|  |  |
-------------------------	-------------------------

**************************************************************/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SQUARES 64
#define RANKS 8


/**************************************************************
Prints a piece from the chess board. The pieces are formatted
as the piece color (lowercase) and the piece letter (the same
letter used in algebraic notation). Pawns are symbolized with
the letter P. For example: bP, wP, wQ, bN, bB.

parameters:	char piece
		    current char that you are reading from 
		    the FEN field. should be validated as
		    being a letter (indicating piece)

return: 	void
*/
void print_piece(char piece)
{
	if (isupper(piece))
	{
		printf("|w%c", piece);
	}
	else if (islower(piece))
	{
		piece = toupper(piece);
		printf("|b%c", piece);
	}
	/* Don't close the pipe '|' because printing
	another piece will close it anyway. At new rows
	and at the end of the board, we will close the
	pipes manually. */
}


/**************************************************************
Prints empty squares. Empty squares are symbolized by one pipe
character, two whitespaces, and a closing pipe: '|  |'

parameters:	char empty_squares
		    char containing a digit, indicating
		    the number of empty squares.

return:		void
*/
void print_empty_squares(char empty_squares)
{
	int square_count = atoi(&empty_squares);
	for (int i = 0; i < square_count; i++)
	{
		printf("|  ");
	}
}


/**************************************************************
MAIN ROUTINE.

User should pass in an FEN file as the only argument:
	usage: ./fen-printer fen_file.fen
*/
int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("usage: ./fen-printer fen_file.fen\n");
		return 1;
	}

	char* file_name = argv[1];
	int file_name_len = strlen(file_name);

	// validate file extension, starting from the period
	int ext_index = file_name_len-1-3;
	// a.fen
	// len = 5
	// ext_index = 5-1-3 = 1

	// if extension is good, int invalid == 0 == false
	int invalid = strcmp(file_name+ext_index, ".fen");
	if (invalid)
	{
		printf("ERROR: invalid .fen file.\n");
		return 2;
	}

	FILE* file = fopen(file_name, "r");
	if (!file)
	{
		printf("ERROR: could not open file.\n");
		return 3;
	}

	// buffer for reading in the fen file
	char* board = calloc(SQUARES+RANKS, sizeof(char));
	// prep for worst case: SQUARES+RANKS = 64 + 8
	// where every square needs its own character, and
	// we need to include the 7 slashes --> 64+7
	// still need one more slot to end the buffer with
	// a sentinel value --> 64+8

	int i = 0;	// buffer index
	char c;		// current char in buffer
			// not an int cause we don't need 
			// to check for EOF
	while ((c = fgetc(file)) != ' ')
	{
		// fields are separated by spaces
		// so read until space is found
		board[i] = c;
		i++;
	}

	printf("\n%s\n", file_name);
	printf("-------------------------\n");
	
	// we can check board[j] != 0 as a sentinel value,
	// because we calloc'd all the memory
	for (int j = 0; board[j] != 0; j++)
	{
		if (isalpha(board[j]))
		{
			print_piece(board[j]);
		}
		else if (isdigit(board[j]))
		{
			print_empty_squares(board[j]);
		}
		else
		{
			// only other possible char is '/'
			// which signifies a new rank	
			printf("|\n");
		}
	}
	// close the last rank
	printf("|\n");
	printf("-------------------------\n");

	free(board);
	fclose(file);
	return 0;
}
