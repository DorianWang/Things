import sys


def startup():
    if len(sys.argv) < 3:
        print("Missing argument: Port")
        sys.exit()
    else:
        server_name = sys.argv[1]
        running_port = int(sys.argv[2])
        print("Client will send to : " + server_name + ":" + str(running_port))
    for (open)



def cleanup():

    sys.exit(0)





def main():
    while(1):
        print("What command do you wish to send to ")


