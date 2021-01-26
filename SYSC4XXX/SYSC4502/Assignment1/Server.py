
from socket import *
import sys
import time
import datetime


def startup():
    if len(sys.argv) < 2:
        print("Missing argument: Port")
        exit()
    else:
        running_port = int(sys.argv[1])
        print("Server will run on port: " + str(running_port))


startup()


