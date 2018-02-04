import os

def addDetails(sock, filename):
	try:
		sock.send("get")
		details = sock.recv(1024)

		filePath = os.path.join(os.getcwd(), "details", filename)
		try:
			with open(filePath, 'w') as fp:		# write description in file
				fp.write(details)
		except:
			return "Some error occured while adding details!"
		return None
	except:
		return "end"


def removeFile(filename):
	filePath = os.path.join(os.getcwd(), "details", filename)
	os.remove(filePath)


def getDetails(filename):
	filePath = os.path.join(os.getcwd(), "details", filename)
	try:
		with open(filePath, 'r') as fp:
			data = fp.readline()
			return (data, None)
	except:
		return (None, "Details not yet added")