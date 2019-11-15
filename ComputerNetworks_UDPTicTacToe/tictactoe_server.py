#tictactoe_server.py
from socket import *
from random import randint
import sys
import tictactoe_support as ttts
from tictactoe_support import EMPTY_SPACE
from tictactoe_support import CLIENT_SPACE
from tictactoe_support import SERVER_SPACE



# ---------------------------------------------------------------------------- #

def main():

	client_addr_ID_list = ['']
	board_save_state = ['']
	while True:
		try:	

			#initialize the server
			serverPort = 12000 
			serverSocket = socket(AF_INET, SOCK_DGRAM) 
			serverSocket.bind(("", serverPort))
			print ('Server is operational.')

			
			#receive messages
			recv_msg, clientAddress = serverSocket.recvfrom(2048)
			#print ("client address", clientAddress)
			recv_msg = recv_msg.decode()

			# if the client that sent the message has not interacted with the server yet
			if clientAddress not in client_addr_ID_list:
				print("Gained a player.")
				empty_board = [
				[0, 0, 0],
				[0, 0, 0],
				[0, 0, 0]
				]
				client_addr_ID_list.append(clientAddress)
				board_save_state.append(empty_board)


			if(recv_msg == "CLIENT_FIRST"):
				print("Gained a player.")
				empty_board = [
				[0, 0, 0],
				[0, 0, 0],
				[0, 0, 0]
				]
				board_str = ttts.ListToStr(empty_board)
				serverSocket.sendto(board_str.encode(), clientAddress)
				board_save_state[ttts.GetID(client_addr_ID_list, clientAddress)] = empty_board


			elif (recv_msg == "SERVER_FIRST"):
				board = [
				[0, 0, 0],
				[0, 0, 0],
				[0, 0, 0]
				]
				invalid = 1;
				while invalid:
					s_col = randint(0, 2)
					s_row = randint(0, 2)
					if (board[s_row][s_col] == EMPTY_SPACE):
						break

				board[s_row][s_col] = SERVER_SPACE
				board_str = ttts.ListToStr(board)
				serverSocket.sendto(board_str.encode(), clientAddress)
				board_save_state[ttts.GetID(client_addr_ID_list, clientAddress)] = board


			elif (recv_msg == "QUITTING"):
				print("Lost a player.")
				board_save_state[ttts.GetID(client_addr_ID_list, clientAddress)] = [
				[0, 0, 0],
				[0, 0, 0],
				[0, 0, 0]
				]


			else: #board is received
				board_str = recv_msg
				recv_board = ttts.StrToList(board_str)
				prev_board = board_save_state[ttts.GetID(client_addr_ID_list, clientAddress)]
				board_check = 0;
				
				# check against the previously stored board if the new board is of a valid change
				for i in range(0,3):
					for j in range(0, 3):
						if (prev_board[i][j] != recv_board[i][j]):
							board_check += 1;
				
				if (board_check > 1): #if more than one amendment to the board was made
					serverSocket.sendto("CHEATER".encode(), clientAddress)
				
				else: # if the client is not a cheater
					board = recv_board
					# check for win from the client board
					if (ttts.CheckWin(board) != EMPTY_SPACE):
						if (ttts.CheckWin(board) == CLIENT_SPACE):
							serverSocket.sendto("CLIENT".encode(), clientAddress)
						elif(ttts.CheckWin(board) == SERVER_SPACE):
							serverSocket.sendto("SERVER".encode(), clientAddress)
						elif(ttts.CheckWin(board) < 0): 
							serverSocket.sendto("DRAW".encode(), clientAddress)
							serverSocket.sendto(ttts.ListToStr(board).encode(), clientAddress)
					
					else: #make server move
						invalid = 1;
						while invalid:
							s_col = randint(0, 2)
							s_row = randint(0, 2)
							if (board[s_row][s_col] == EMPTY_SPACE):
								invalid = 0
			
						board[s_row][s_col] = SERVER_SPACE
						board_save_state[ttts.GetID(client_addr_ID_list, clientAddress)] = board

						# check if server won
						if (ttts.CheckWin(board) != EMPTY_SPACE):
							if (ttts.CheckWin(board) == CLIENT_SPACE):
								serverSocket.sendto("CLIENT".encode(), clientAddress)
							elif(ttts.CheckWin(board) == SERVER_SPACE):
								serverSocket.sendto("SERVER".encode(), clientAddress)
								serverSocket.sendto(ttts.ListToStr(board).encode(), clientAddress)
							elif(ttts.CheckWin(board) < 0): 
								serverSocket.sendto("DRAW".encode(), clientAddress)
								serverSocket.sendto(ttts.ListToStr(board).encode(), clientAddress)

						else:
							board_str = ttts.ListToStr(board)
							serverSocket.sendto(board_str.encode(), clientAddress)


		except KeyboardInterrupt:
			#quitting now:
			print("")
			print("")
			print("")
			print ("Triggered Ctrl-c keyboard interrupt ...")
			print ("Terminating program ...")
			print()
			serverSocket.close()
			sys.exit()



# ---------------------------------------------------------------------------- #


main()