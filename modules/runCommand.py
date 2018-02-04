from subprocess import *

def run(input):

	try:
		run = Popen("./dict", cwd = "./dictionary", stdin = PIPE, stdout = PIPE, stderr = PIPE)
	except:
		return (None, "Some error occured. Try again later")

	out, err = run.communicate(input.encode())
	print "from run out, err ", out, err
	if(err):
		return (None, err.decode('utf-8'))
	if(out == "-1"):	# invalid word format
		return (None, "Invalid format")
	return (out, None)