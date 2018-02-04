import socket, select, string, sys

def display() :
	header = "\33[32m\33[1m"+"> "+"\33[0m"
	sys.stdout.write(header)
	sys.stdout.flush()

def main():

    if len(sys.argv)<2:
        host = raw_input("Server's ip address: ")
    else:
        host = sys.argv[1]

    port = 5001

    mySock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    mySock.settimeout(2)
    
    # connect to host
    try :
        mySock.connect((host, port))
    except :
        print "\33[31m\33[1m Can't connect to the server \33[0m"
        sys.exit()

    #if connected
    name=raw_input("\33[32m\33[1m Enter username: \33[0m")
    mySock.send(name)

    while 1:
        socket_list = [sys.stdin, mySock]
        
        # Get the list of sockets which are readable
        readList, writeList, errorList = select.select(socket_list , [], [])
        
        for sock in readList:
            #incoming message from server
            if sock == mySock:
                data = sock.recv(4096)
                if not data :
                    print '\33[31m\33[1m\r DISCONNECTED!!\n \33[0m'
                    sys.exit()
                elif data == "get" :
                    details = raw_input("\rEnter error details: \n")
                    mySock.send(details)
                else:
                    sys.stdout.write(data + "\n")
                display()
        
            #user entered a message
            else :
                msg=sys.stdin.readline()
                mySock.send(msg)
                display()

if __name__ == "__main__":
    main()
