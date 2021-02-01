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
        if user_input == "":
            continue
        user_input = user_input.strip()
        user_input = list(filter(None, user_input.split(" ")))

        payload = bytearray()
        print(user_input)

        if not user_input:  # Just ignore empty strings
            continue
        elif user_input[0] == CommandStrings.ROOMS:
            payload.append(MessageID.REQ_ROOMS)
        elif user_input[0] == CommandStrings.DAYS:
            payload.append(MessageID.REQ_DAYS)
        elif user_input[0] == CommandStrings.TIMESLOTS:
            payload.append(MessageID.REQ_TIMESLOTS)

        elif user_input[0] == CommandStrings.CHECK_ROOM:
            if user_input.__len__() < 2:
                print("Please specify a room to check for!")
                continue
            payload.append(MessageID.REQ_CHECK_ROOM)
            payload = payload + user_input[1].encode()
            payload.append(MessageID.NULL)

        elif user_input[0] == CommandStrings.MAKE_RESERVATION:
            if user_input.__len__() < 4:
                print("Not enough arguments to make a reservation!")
                continue
            payload.append(MessageID.REQ_MAKE_RESERVATION)
            payload = payload + user_input[1].encode()
            payload.append(MessageID.NULL)
            payload = payload + user_input[2].encode()
            payload.append(MessageID.NULL)
            payload = payload + user_input[3].encode()
            payload.append(MessageID.NULL)

        elif user_input[0] == CommandStrings.DELETE_RESERVATION:
            if user_input.__len__() < 4:
                print("Not enough arguments to delete a reservation!")
                continue
            payload.append(MessageID.REQ_DELETE_RESERVATION)
            payload = payload + user_input[1].encode()
            payload.append(MessageID.NULL)
            payload = payload + user_input[2].encode()
            payload.append(MessageID.NULL)
            payload = payload + user_input[3].encode()
            payload.append(MessageID.NULL)

        elif user_input[0] == CommandStrings.STOP_SERVER:
            payload.append(MessageID.REQ_STOP_SERVER)
            break
        else:
            print(ResponseStrings.UNRECOGNIZED_COMMAND)
            continue

        print(payload)
        client_socket.sendto(payload, (server_name, running_port))

    print("Exiting!")
    sys.exit(0)


main()
