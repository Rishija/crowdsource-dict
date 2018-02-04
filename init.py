import socket, os
from subprocess import *

def compile():
	make = Popen("make", cwd = "./dictionary", stdout = PIPE, stderr = PIPE)
	code = make.wait()
	if(code != 0):
		return False
	return True


detailsPath = os.path.join(os.getcwd(), "details")
if(os.path.isdir(detailsPath) == False):
	os.mkdir("details")
'''
`Record` is dictionary to store connected clients where
key : socket address of client
value : a list containing username and a flag for client being admin
'''
activeUsers = {}
admins = ["admin1", "admin2"]	# List of admins
connected_list = []	# List to keep track of socket descriptors
buffer = 4096
port = 5001

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)	# Create socket

server_socket.bind(("localhost", port))		# Bind ip address and port number
server_socket.listen(10) # Listen atmost 10 connection at any instance

connected_list.append(server_socket)	# Add server socket to the list of readable connections