from subprocess import *

def run(input):

	try:
		run = Popen("./dict", cwd = "./dictionary", stdin = PIPE, stdout = PIPE, stderr = PIPE)
	except:
		return (None, "Some error occured. Try again later")

	out, err = run.communicate(input.encode())
	if(err):
		return (None, err.decode('utf-8'))
	return (out, None)