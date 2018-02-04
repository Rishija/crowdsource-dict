from runCommand import *
from handleDetails import *

def handle(data, sock, user = "", isAdmin = False):

	# print "In handle"

	if data == "exit":
		return None, "end"

	action = data.split()[0]
	word = data.split()[1]

	# print "action: ", action

	if action == "add" and word:
		if(isAdmin == False):
			return None, "Permission Denied"
		else:
			err = addDetails(sock, word)
			if(err):
				return None, err

			out, err = run(data + " " + user)
			if(err):
				removeFile(word)
				return None, err
			return " Added", None

	elif action == "find":
		out, err = run(data)
		# print "In find out, err ", out, err
		if(err):
			return None, err
		return (out,None) if (out) else (None, "Error not yet added to the dictionary")

	elif action == "prefix":
		out, err = run(data)
		if(err):
			return None, err
		return (out,None) if (out) else (None, "No error with given prefix")

	else:
		return None, "Invalid action"


