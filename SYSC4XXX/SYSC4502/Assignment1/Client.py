import sys
if sys.version_info < (3, 7):
    print("This script requires Python 3.7 or newer to run!")
    sys.exit(1)
from CommandDefinitions import CommandStrings, ResponseStrings, NetConsts



def cleanup():

    sys.exit(0)


def main():

    if len(sys.argv) < 3:
        print("Missing argument(s), requires: HOSTNAME PORT")
        sys.exit()
    else:
        server_name = sys.argv[1]
        running_port = int(sys.argv[2])
        print("Client will send to : " + server_name + ":" + str(running_port))

    print("What command do you wish to send to the server?")
    while 0:
        
        break

    print("Exiting!")
    sys.exit(0)


main()

