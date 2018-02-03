from subprocess import *

def run(input):
	print "in run command with input ",input
	run = Popen("./dict", cwd = "./dictionary", stdin = PIPE, stdout = PIPE)
	out, err = run.communicate(input.encode())
	# print "from run command out, err: ", out, err
	if(err):
		return None, err.decode('utf-8')
	return out, None