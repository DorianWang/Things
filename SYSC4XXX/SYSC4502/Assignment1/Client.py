import sys


def startup():
    if len(sys.argv) < 3:
        print("Missing argument: Port")
        exit()
    else:
        serverIP = sys.argv[1]
        running_port = int(sys.argv[2])
        print("Client will send to : " + serverIP + ":" + str(running_port))













def main():
    while(1):
        print("What command do you wish to send to ")


