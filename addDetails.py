import os

def addDetails(sock, filename):
	try:
		sock.send("get")
		details = sock.recv(1024)

		filePath = os.path.join(os.getcwd(), "details", filename)
		try:
			with open(filePath, 'w') as fp:
				fp.write(details)
		except:
			return "Some error occured while adding details!"
		return None
	except:
		return "end"

def removeFile(filename):
	filePath = os.path.join(os.getcwd(), "details", filename)
	os.remove(filePath)