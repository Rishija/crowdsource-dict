from runCommand import *

def handle(data, user = "", isAdmin = False):

	# print "In handle"

	if data == "exit":
		return None, "end"

	action = data.split()[0]

	# print "action: ", action

	if action == "add":
		if(isAdmin == False):
			return None, "Permission Denied"
		else:
			out, err = run(data + " " + user)
			if(err):
				return None, err
			return " Added", None

	elif action == "find":
		out, err = run(data)
		# print "In find out, err ", out, err
		if(err):
			return None, err
		if(out):
			return out, None
		else:
			return None, "Error not yet added to the dictionary"

	elif action == "prefix":
		out, err = run(data)
		if(err):
			return None, err
		if(out):
			return out, None
		else:
			return None, "No error with given prefix"

	else:
		return None, "Invalid action"


