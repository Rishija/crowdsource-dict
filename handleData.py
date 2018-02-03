def handle(data, user = "", isAdmin = False):
	if data == "exit":
		return None, "end"
	elif data == "add":
		if(isAdmin == False):
			return None, "Permission Denied"
		else:
			# -----
			return


