#tictactoe_client.py
import socket
import sys
import tictactoe_support as ttts
from tictactoe_support import EMPTY_SPACE
from tictactoe_support import CLIENT_SPACE
from tictactoe_support import SERVER_SPACE

def main():
	while True:
		try:
			print ()
			print ()
			print ()
			print ()
			print ()

			serverPort = 12000
			clientSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
			#Client port number is dynamically allocated by the OS if it is not specified in code
			
			# check for the server-information command line inputs
			if ('-s' in sys.argv):
				index1 = sys.argv.index('-s')
				if (index1 + 1 >= len(sys.argv)):
					print ("Error: Invalid Server IP Address Input")
					return;
				else:
					try:
						socket.inet_aton(sys.argv[index1 + 1])
					except socket.error:
						print ("Error: Invalid IP Address")
						return;
					serverName = sys.argv[index1 + 1]
			
			
					try:
						clientSocket.connect((serverName, serverPort))
						clientSocket.shutdown(2)
						clientSocket.close()
						print ("Connecting to server ", end="")
						print (serverName + ", port: " + str(serverPort))

					except:
						print ("Cannot connect to ")
						print (serverName + ", port: " + str(serverPort))

		 			#do this again because it disconnected

					clientSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
					

					# check if client dictated what they wanted to go first
					if ('-c' in sys.argv):
						clientSocket.sendto("CLIENT_FIRST".encode(),(serverName, serverPort))
					else:
						clientSocket.sendto("SERVER_FIRST".encode(),(serverName, serverPort))

					while True:
						try :
							#Obtain various types of messages from the server
							input_str, serverAddress = clientSocket.recvfrom(2048)
							clientSocket.settimeout(3.0)

						except socket.timeout:
							print ("Socket timeout ...")
							clientSocket.shutdown(2)
							clientSocket.close()
							sys.exit()


						input_str = input_str.decode();

						if (input_str == "CLIENT"):
							print()
							print()
							print()

							ttts.print_board(board)
							print()
							print ("Congratulations. You've won!!")
							clientSocket.close()
							sys.exit()


						elif(input_str == "SERVER"):
							print()
							print()
							print()

							ttts.print_board(board)
							board_str, serverAddress = clientSocket.recvfrom(2048)
							board_str = board_str.decode()
							board = ttts.StrToList(board_str)
							print()
							print()
							print()
							ttts.print_board(board)
							print()
							print ("You lose.")
							clientSocket.close()
							sys.exit()


						elif(input_str == "DRAW"):
							print()
							print()
							print()

							ttts.print_board(board)
							board_str, serverAddress = clientSocket.recvfrom(2048)
							board_str = board_str.decode()
							board = ttts.StrToList(board_str)
							print()
							print()
							print()
							ttts.print_board(board)
							print()
							print ("It's a draw.")
							clientSocket.close()
							sys.exit()


						elif(input_str == "CHEATER"):
							print ("You are a cheater ...")
							clientSocket.close();
							sys.exit()


						else:
							board = ttts.StrToList(input_str)

							print()
							print()
							print()
							ttts.print_board(board)	

							print()
							print()
							print ("Your turn!")
							print()

							c_row = input('Pick row (1-3): ')
							c_col = input('Pick column (1-3): ')

							if (ttts.IsInputValid(board, c_row, c_col) == 1):
								board[int(c_row) - 1][int(c_col) - 1] = CLIENT_SPACE;


							else:
								while (ttts.IsInputValid(board, c_row, c_col) == 0):
									print()
									print ("Invalid Input. Please try again.")
									c_row = input('Pick row (1-3): ')
									c_col = input('Pick column (1-3): ')

								board[int(c_row) - 1][int(c_col) - 1] = CLIENT_SPACE;
							# now we have valid input for the board

							clientSocket.sendto(ttts.ListToStr(board).encode(),(serverName, serverPort)) 

			
			else: # the address was not specified properly in the command line argument
				print("ERROR: Required arguments '-s' <SERVER IP_ADDRESS>")
				return

		except KeyboardInterrupt:
			#quitting now:
			print("")
			print("")
			print("")
			print ("Triggered Ctrl-c keyboard interrupt ...")
			print ("Terminating program ...")
			print()
			clientSocket.sendto("QUITTING".encode(),(serverName, serverPort))
			clientSocket.close()
			sys.exit()


main()