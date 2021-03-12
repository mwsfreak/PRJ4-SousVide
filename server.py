#!/usr/bin/python3

import sys
from socket import socket, AF_INET, SOCK_STREAM, SOL_SOCKET, SO_REUSEADDR
from lib import Lib
import json

HOST = ''
PORT = 9000
BUFSIZE = 1000

def main(argv):

	serverSocket = socket(AF_INET, SOCK_STREAM)
	serverSocket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1) # SOL_SOCKET: Handle option on Socket layer
	serverSocket.bind((HOST, PORT))

	serverSocket.listen(1)
	
	while 1:
		print("Server is ready to accept connection.")
		connectionSocket, addr = serverSocket.accept()
		print("Server accepted socket connection on ", addr)
		
		closeMe = 0
		
		while (closeMe == 0):
			# Get message from client
			message = Lib.readTextTCP(connectionSocket)
			print("Received message from client: ", message)
			
			# Interpret message
			closeMe = decode(message)

		# Close socket
		print("Closing socket")
		connectionSocket.close()


def decode(message):
	#message = struct/byte-sekvens/list something
	#message.command = 0x01
	messageDecoded = json.loads(message)
	command = messageDecoded.get("command")

	switcher = {
		"start": startCook,
		"stop": stopCook,
		"close": lambda message: -1
	}
	# Get the function from switcher dictionary
	func = switcher.get(command, lambda message: 0 )
	# Execute the function
	funcReturn = func(messageDecoded)
	print(funcReturn)

	if (funcReturn == -1):
		return 1
	else:
		return 0

def startCook(message):
	print(f"Starting cook at {message.get('temp')} degrees C, for {message.get('time')} minutes!")
	return 0

def stopCook(message):
	print("Stopping cook")
	return 0

if __name__ == "__main__":
   main(sys.argv[1:])


# Reading from a file: https://www.kite.com/python/answers/how-to-read-bytes-from-a-binary-file-in-python