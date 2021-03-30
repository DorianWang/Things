from socket import *
import sys
import struct
import select
import signal


def handler(signal_received, frame):
    global is_running
    is_running = False
    print("Intercepted Quit, Cleaning Up!")


# helper functions to create messages and to split messages
def createMessage(seq_, msg):
    return str(seq_) + "@" + msg


def splitMessage(mess_in):
    return mess_in[:mess_in.find('@')], mess_in[mess_in.find('@') + 1:]


# Get the server hostname and port as command line arguments
argv = sys.argv                      
if len(argv) != 3:
    print("Incorrect number of command-line arguments")
    print("Invoke client with: python", argv[0], "<MulticastIP> <port>")
    exit()

host = argv[1]
port = int(argv[2])

clientSocket = socket(AF_INET, SOCK_DGRAM)
# Set socket timeout as 1 second
clientSocket.settimeout(1)

# Set the time-to-live for messages to 1 so they do not go past the
# local network segment.
ttl = struct.pack('b', 1)
clientSocket.setsockopt(IPPROTO_IP, IP_MULTICAST_TTL, ttl)

# Create a pair of sockets to listen to interrupts, and attach a signal handler too
error_listener, error_writer = socketpair()
error_listener.setblocking(False)
signal.set_wakeup_fd(error_writer.fileno())
is_running = True
signal.signal(signal.SIGINT, handler)
signal.signal(signal.SIGBREAK, handler)

# we will get a (potentially) large number of replies for each request
# add a sequence number to the messages and read replies until we get
# to the first one with a new sequence number

msg_sequence = 0

while True:

    msg_sequence += 1
    message = createMessage(msg_sequence, input("Next command: "))
    
    try:
        clientSocket.sendto(message.encode(), (host, port))
        ready_sockets = select.select([error_listener, serverSocket], [], [])
        if ready_sockets[0][0] == error_listener or ready_sockets[0].__len__() == 2:
            break  # An error was caught!
        reply, serverAddress = clientSocket.recvfrom(2048)
        (seq, reply) = splitMessage(reply.decode())
        while int(seq) < msg_sequence:
            # print("Discard old reply")
            reply, serverAddress = clientSocket.recvfrom(2048)
            (seq, reply) = splitMessage(reply.decode())

        print(reply)
        if reply == "bye":
            break
    except:
        print("Message to server timed out, please retry")

# once we break out of the loop, close socket and terminate
clientSocket.close()
