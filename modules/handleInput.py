import sys
sys.path.append("modules/")
from runCommand import *
from handleDetails import *

helpMsg = """\33[32m\33[1m help
	For this help message

 add newError
	Add error 'newError' to the dictionary

 find thisError
	Know about error 'thisError'

 prefix pre
	Get list of all errors with prefix 'pre'

 exit
	To exit\33[0m
	"""

def handle(data, sock, user = "", isAdmin = False):

	if data == "exit":
		return (None, "end")

	args = data.split()
	action, word = "", ""
	if(len(args) > 0):
		action = args[0]

	if(len(args) > 1):
		word = args[1]

	if action == "add" and word:
		if(isAdmin == False):
			return (None, "Permission Denied")
		else:
			err = addDetails(sock, word)
			if(err):
				return (None, err)

			out, err = run(data + " " + user)
			if(err):
				removeFile(word)
				return (None, err)
			return ("\33[32m\33[1m Added\n", None)

	elif action == "find":
		description, err = getDetails(word)		# get details of word from file
		if(err):
			return (None, err)

		out, err = run(data)
		if(err):	# error in finding word to dictionary
			return (None, err)

		# out is NULL is word is not found in the dictionary
		# else append its description to the output
		return (out + description + "\n", None) if (out) else (None, "Error not yet added to the dictionary")

	elif action == "prefix":
		out, err = run(data)	# get words with given prefix from dictionary
		if(err):
			return (None, err)

		# out is NULL if there is no word with entered prefix
		return (out,None) if (out) else (None, "No error with given prefix")

	elif action == "help":
		return (helpMsg, None)

	else:
		return (None, "Invalid action")
