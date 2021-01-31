import sys
if sys.version_info < (3, 7):
    print("This script requires Python 3.7 or newer to run!")
    sys.exit(1)
from CommandDefinitions import MessageID, CommandStrings, ResponseStrings, NetConsts
import socket

def parse_response(server_data: list) -> tuple:
    if server_data.__len__() == 0:
        return MessageID.NULL, []

    ID = server_data[0]
    data = []
    if server_data.__len__() > 1:
        payload = (server_data[1:-1]).decode("utf-8")
        last_slice_position = -1
        for i in range(payload.__len__()):
            if payload[i] == '\n':  # Added support for windows line endings because I felt like it.
                if i > 0 and payload[i-1] == '\r':
                    data.append(payload[last_slice_position+1:i-1])
                    last_slice_position = i
                else:
                    data.append(payload[last_slice_position+1:i])
                    last_slice_position = i
        if last_slice_position != payload.__len__():
            data.append(payload[last_slice_position+1:])
    return ID, data



def main():

    if len(sys.argv) < 3:
        print("Missing argument(s), requires: HOSTNAME PORT")
        sys.exit()
    else:
        server_name = sys.argv[1]
        running_port = int(sys.argv[2])
        print("Client will send to : " + server_name + ":" + str(running_port))

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    print("What command do you wish to send to the server?")
    while True:
        user_input = input("Enter Command: ")
        user_input = user_input.strip()
        user_input = user_input.split(" ")

        payload = bytearray()

        if not user_input:  # Just ignore empty strings
            continue
        elif user_input == CommandStrings.ROOMS:
            pass
        elif user_input == CommandStrings.DAYS:
            pass
        elif user_input == CommandStrings.TIMESLOTS:
            pass
        elif user_input == CommandStrings.CHECK_ROOM:
            pass
        elif user_input == CommandStrings.MAKE_RESERVATION:
            pass
        elif user_input == CommandStrings.DELETE_RESERVATION:
            pass
        elif user_input == CommandStrings.STOP_SERVER:

            break
        else:
            print(ResponseStrings.UNRECOGNIZED_COMMAND)
            continue

        client_socket.send()

    print("Exiting!")
    sys.exit(0)

a = ""
b = "asdf wefe"
print(a.strip())
print(a.split())
print(b.split())
