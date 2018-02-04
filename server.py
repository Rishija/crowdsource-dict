import select, sys
sys.path.append("modules/")
from handleInput import *
from init import *

def sendErrorMsg(sockfd, error):
    sockfd.send("\33[31m\33[1m\r " + error + "\33[0m\n")

if __name__ == "__main__":

    compile = compile()

    if(compile == False):
        print "\33[31m\33[1m Compile Error! "
        sys.exit(1)

    print "\33[32m\33[1m\n \t\t\t\tSERVER WORKING \33[0m" 

    while 1:
        # Get the list sockets which are ready to be read 
        readList, writeList, errorList = select.select(connected_list,[],[])

        for sock in readList:
            #New connection
            if sock == server_socket:
                # Handle the case in which there is a new connection recieved through server_socket
                sockfd, addr = server_socket.accept()
                name=sockfd.recv(buffer)
                connected_list.append(sockfd)

                userNames = []
                for eachUser in activeUsers.keys():
                    userNames.append(activeUsers[eachUser][0])

                activeUsers[addr]=[]

                #if repeated username
                if name in userNames:
                    sendErrorMsg(sockfd, "Already logged in")
                    del activeUsers[addr]
                    connected_list.remove(sockfd)
                    sockfd.close()
                    continue
                else:
                    isAdmin = name in admins
                    activeUsers[addr]=[name, isAdmin]    # add in active connections
                    print "Client (%s, %s) connected" % addr," [",activeUsers[addr][0],"]"
                    sockfd.send("\33[33m\33[1m\33[5m\n\t\tWelcome to ONLINE ERRORS DICTIONARY\33[0m\n\n")
                    helpmsg, err = handle("help", sockfd)
                    sockfd.send(helpmsg)

            # Request from client
            else:
                try:
                    data1 = sock.recv(buffer)
                    data=data1[:data1.index("\n")]
                    i,p=sock.getpeername()  #get addr of client sending the message
                    record = activeUsers[(i,p)]

                    out, err = None, None
                    out, err = handle(data, sock, record[0], True) if (record[1] == True ) else handle(data, sock)

                    if(err == "end"):
                        print "Client (%s, %s) is offline" % (i,p)," [",activeUsers[(i,p)][0],"]"
                        del activeUsers[(i,p)]
                        connected_list.remove(sock)
                        sock.close()
                    else:
                        if(err):
                            sendErrorMsg(sock, err)
                        else:
                            sock.send("\r" + out)
            
                except:
                    (i,p)=sock.getpeername()
                    print "Client (%s, %s) is offline (error)" % (i,p)," [",activeUsers[(i,p)][0],"]\n"
                    del activeUsers[(i,p)]
                    connected_list.remove(sock)
                    sock.close()
                    continue

    server_socket.close()

