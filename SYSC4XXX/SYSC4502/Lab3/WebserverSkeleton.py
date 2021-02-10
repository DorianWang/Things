# Import socket module
from socket import *
import sys  # In order to terminate the program

# I used https://stackoverflow.com/questions/33784127/minimum-http-server-reply to help with the headers
# https://www.w3.org/Protocols/rfc1341/7_1_Text.html for text stuff
# https://stackoverflow.com/questions/4726515/what-http-response-headers-are-required for more header stuff

# Create a TCP server socket
# (AF_INET is used for IPv4 protocols)
# (SOCK_STREAM is used for TCP)

serverSocket = socket(AF_INET, SOCK_STREAM)

# Fill in start
serverSocket.bind(('localhost', 7777))  # hardcoded values, enough for lab
serverSocket.listen()
# Fill in end

# Server should be up and running and listening to the incoming connections

while True:
    print('The server is ready to receive')

    # Set up a new connection from the client
    connectionSocket, addr = serverSocket.accept()  #Fill in here

    # If an exception occurs during the execution of try clause
    # the rest of the clause is skipped
    # If the exception type matches the word after except
    # the except clause is executed
    try:
        # Receives the request message from the client
        message = connectionSocket.recv(1024)  # Fill in here, I used a random buffer size, should be large enough.

        # print complete message received from browser (for information purposes)
        print(message)
        # Extract the path of the requested object from the message
        # The path is the second part of HTTP header, identified by [1] (when splitting on whitespace)
        filename = message.split()[1]
        # Because the extracted path of the HTTP request includes
        # a character '\', we read the path from the second character
        f = open(filename[1:])

        # Store the entire content of the requested file in a temporary buffer
        outputdata = f.read()  #Fill in here
        f.close()

        # Send the HTTP response header line to the connection socket

        # Fill in start
        OK_header = "HTTP/1.0 200 OK\r\nContent-Type:text/html; charset=utf-8\r\n\r\n"
        for i in range(0, len(OK_header)):
            connectionSocket.send(OK_header[i].encode())
        # Fill in end

        # Send the content of the requested file to the connection socket
        for i in range(0, len(outputdata)):
            connectionSocket.send(outputdata[i].encode())
        connectionSocket.send("\r\n".encode())

        # Close the client connection socket
        connectionSocket.close()

    except IOError:
        # Send HTTP response message for file not found
        # Fill in start
        Not_Found_header = "HTTP/1.0 404 Not Found\r\nContent-Length:22; Content-Type:text/plain; charset=utf-8\r\n\r\n"
        Not_Found_message = "Nothing here, go away."  # extra message so it isn't a blank screen
        Not_Found_header = Not_Found_header + Not_Found_message

        for i in range(0, len(Not_Found_header)):
            connectionSocket.send(Not_Found_header[i].encode())
        # Fill in end

        # Close the client connection socket
        connectionSocket.close()  # Fill in here
    finally:
        pass  # I guess you could kill the loop here, or in the exception. Currently the server just doesn't end.

serverSocket.close()
sys.exit()  # Terminate the program after sending the corresponding data
