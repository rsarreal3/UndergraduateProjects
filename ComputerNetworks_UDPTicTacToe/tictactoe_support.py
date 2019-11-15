#tictactoe_support.py
import socket
import sys

EMPTY_SPACE = 0
CLIENT_SPACE  = 3
SERVER_SPACE= 4


# --------------------------------------------------------------------------- #

#prints the tic-tac-toe board
def print_board (board):
	i = 1
	print ("   1 2 3 ")
	print ("  -------")	
	for line in board:
		print (i, "|", end ="")
		for cell in line:
			if (cell == EMPTY_SPACE):
				print (" |", end ="")
			elif (cell == CLIENT_SPACE):
				print ("X|", end ="")
			elif (cell == SERVER_SPACE):
				print ("O|", end ="")
		print()
		i = i + 1
	print ("  -------")



# --------------------------------------------------------------------------- #

#Converts a 3x3 cell list into a string in the form of concatenated rows
def ListToStr(board):
	my_str = ""
	for row in board:
		for cell in row:
			my_str = my_str + str(cell)
	return my_str


# --------------------------------------------------------------------------- #

#converts a string of single-digit values into a two dimensional, 3x3 list
def StrToList(board_str):
	#initialize an empty board
	board = [
	[0, 0, 0],
	[0, 0, 0],
	[0, 0, 0]
	]

	i = 0
	j = 0
	for my_char in board_str:
		board[i][j] = int(my_char)
		j = j + 1
		if (j >= 3):
			i += 1
			j = 0

	return board

	
# --------------------------------------------------------------------------- #

# checks for a win condition in the tic-tac-toe game
def CheckWin(board):
	# check rows
	for row in board:
		row_sum = sum(row)
		if (row_sum == (CLIENT_SPACE * 3) or row_sum == (SERVER_SPACE * 3)):
			return row_sum / 3

	#check columns
	col_sum = 0;
	for col in range(0,3):
		for row in range(0,3):
			col_sum += board[row][col]
			if ((col_sum == (CLIENT_SPACE * 3)) or (col_sum == (SERVER_SPACE * 3))):
				return col_sum / 3
		col_sum = 0

	# check diagonals
	diag_sum1 = 0
	diag_sum2 = 0

	for i in range(0,3):
		diag_sum1 += board[i][i]
		diag_sum2 += board[i][2-i]
		if (diag_sum1 == (CLIENT_SPACE * 3) or diag_sum1 == (SERVER_SPACE * 3)):
			return diag_sum1 / 3
		if (diag_sum2 == (CLIENT_SPACE * 3) or diag_sum2 == (SERVER_SPACE * 3)):
			return diag_sum2 / 3

	is_full = 1
	for row in board:
		for cell in row:
			if cell == 0:
				is_full = 0

	if is_full == 1:
		return -1
	
	return 0;


# --------------------------------------------------------------------------- #

#Returns 1 if the space is occupied, 0 if it is not
def IsOccupied(board, row, col):
	if (board[row][col] == EMPTY_SPACE):
		return 0

	return 1

	
# --------------------------------------------------------------------------- #

#Determines if the inputted column and row values are valid, returns 1 if it is
#returns 0 if not. Values must be integers between 1 and 3 inclusive
def IsInputValid(board, row, col):
	try:
		check_row = int(row)
	except ValueError:
		return 0

	try:
		check_col = int(col)
	except ValueError:
		return 0

	if (( (int(row) >= 1) and (int(row) <= 3) ) and ( (int(col) >= 1) and (int(col) <= 3) )):
		if (IsOccupied(board, int(row) - 1, int(col) - 1) == 0):
			return 1

	return 0

	
# --------------------------------------------------------------------------- #

# Obtains the index of an object in a list, returns -1 if it is not in the list
def GetID(my_list, addr):
	try:
		ID = my_list.index(addr)
	except ValueError:
		return -1
	return ID


# --------------------------------------------------------------------------- #

#Checks for time out. NOTE: Not implemented. Maybe for future iteration
def CheckTimeout(my_list, addr):
	return 0


