# from handleData init import *
from handleData import *
from init import *
import select, sys

def sendErrorMsg(sockfd, error):
    sockfd.send("\33[31m\33[1m " + error + "\33[0m\n")

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
                    sockfd.send("\33[32m\33[1m Welcome to `Dictionary of errors :P `. Enter 'exit' anytime to exit\n\33[0m")

            # Request from client
            else:
                try:
                    data1 = sock.recv(buffer)
                    #print "sock is: ",sock
                    data=data1[:data1.index("\n")]
                    print "\ndata received: ",data
                    
                    #get addr of client sending the message
                    i,p=sock.getpeername()
                    rec = activeUsers[(i,p)]
                    out, err = handle(data, rec[0], True) if (rec[1] == True ) else handle(data)
                    if(err == "end"):
                        print "Client (%s, %s) is offline" % (i,p)," [",activeUsers[(i,p)][0],"]"
                        del activeUsers[(i,p)]
                        connected_list.remove(sock)
                        sock.close()
            
                except:
                    (i,p)=sock.getpeername()
                    print "Client (%s, %s) is offline (error)" % (i,p)," [",activeUsers[(i,p)][0],"]\n"
                    del activeUsers[(i,p)]
                    connected_list.remove(sock)
                    sock.close()
                    continue

    server_socket.close()

