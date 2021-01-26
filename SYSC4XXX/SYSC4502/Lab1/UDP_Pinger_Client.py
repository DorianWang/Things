# UDP_Pinger_Client.py

from socket import *
import sys
import time
import datetime

# Create a UDP socket
# Notice the use of SOCK_DGRAM for UDP packets
clientSocket = socket(AF_INET, SOCK_DGRAM)

# Set default values for server location
target_address = 'localhost'
target_port = 12000

# Retrieve IP address and port number from inputs, I set defaults for faster testing. Validating addresses is harder :(
if len(sys.argv) > 1:
    target_address = sys.argv[1]
if len(sys.argv) > 2:
    try:
        target_port = int(sys.argv[2])
    except ValueError:
        print("Port value must be an integer!")
        exit()

for i in range(10):
    new_message = ("Ping " + str(i + 1) + " ").encode()
    new_message = new_message + datetime.datetime.now().__str__().encode()
    send_time = time.time()  # This might be inaccurate, but there wasn't any requirements about RTT accuracy.
    clientSocket.sendto(new_message, tuple((target_address, target_port)))
    clientSocket.settimeout(1.0)  # 1 second timeout
    # Receive the client packet along with the address it is coming from
    try:
        message, address = clientSocket.recvfrom(1024)
    except timeout:
        # print("Message " + str(i + 1) + " timed out!")
        print("Request timed out.")
        continue

    print("Reply from " + address.__str__() + ": " + message.decode())  # includes port with address, I guess it's fine
    print('RTT is: %.10f' % (time.time() - send_time))

clientSocket.close()
