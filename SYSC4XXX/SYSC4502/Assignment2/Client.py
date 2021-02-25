import sys
if sys.version_info < (3, 7):
    print("This script requires Python 3.7 or newer to run!")
    sys.exit(1)
from CommandDefinitions import MessageID, CommandStrings, ResponseStrings, NetConsts
import socket
from threading import Thread

_is_running = True
current_packet_number = 0


def listener(s_local, port_local):
    global _is_running
    print("AAAA!")
    while _is_running == 1:
        buf, address = s_local.recvfrom(port_local)
        if len(buf):
            print("Received %s bytes from %s: %s " % (len(buf), address, buf.decode('utf-8')))
        if _is_running == 0:
            break
    print("Out of listener!")


def parse_response(server_data: bytes) -> tuple:
    """
    This function takes data returned from the server and returns the MessageID byte
    together with string values in a list. The resulting list may be empty.
    """
    if server_data.__len__() == 0:
        return MessageID.NULL, []

    message_id = server_data[0]
    data = []
    if server_data.__len__() > 1:
        payload = (server_data[1:]).decode("utf-8")
        last_slice_position = -1
        for i in range(payload.__len__()):
            if payload[i] == '\x00':  # if the character is null, it is the end of the string.
                temp = payload[last_slice_position+1:i]
                if temp:
                    data.append(temp)
                last_slice_position = i
                continue

            if payload[i] == '\n':  # Added support for windows line endings because I felt like it.
                if i > 0 and payload[i-1] == '\r':
                    temp = payload[last_slice_position+1:i-1]
                    if temp:
                        data.append(temp)
                    last_slice_position = i
                else:
                    temp = payload[last_slice_position+1:i]
                    if temp:
                        data.append(temp)
                    last_slice_position = i

        if last_slice_position != payload.__len__():
            if payload[last_slice_position+1:]:  # This adds a final slice if the data was not 0 terminated.
                data.append(payload[last_slice_position+1:])
    return message_id, data


def main():

    if len(sys.argv) < 3:
        print("Missing argument(s), requires: HOSTNAME PORT")
        sys.exit()
    else:
        server_name = sys.argv[1]
        running_port = int(sys.argv[2])
        print("Client will send to : " + server_name + ":" + str(running_port))

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    client_socket.settimeout(NetConsts.TIMEOUT)

    stopping = 0  # This variable is used to close the client after sending the stop command to the server.

    print("What command do you wish to send to the server?")
    while True:
        user_input = input("Enter Command: ")
        if user_input == "":  # ignoring empty inputs
            continue
        user_input = user_input.strip()
        user_input = list(filter(None, user_input.split(" ")))  # this removes any empty inputs, when there are 2 spaces

        payload = bytearray()
        # print(user_input)

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

        elif user_input[0] == CommandStrings.HELP:
            print(ResponseStrings.HELP)
            continue

        elif user_input[0] == CommandStrings.RESEND:
            payload.append(MessageID.REQ_RESEND)
            print(ResponseStrings.RESEND_CUE)

        elif user_input[0] == CommandStrings.STOP_SERVER:
            payload.append(MessageID.REQ_STOP_SERVER)
            stopping = 1  # mark client for stopping
        else:
            print(ResponseStrings.UNRECOGNIZED_COMMAND)
            continue

        # print(payload)
        client_socket.sendto(payload, (server_name, running_port))

        try:
            server_response = client_socket.recv(NetConsts.MAX_BUFFER)
            message_id, extra_data = parse_response(server_response)
        except TimeoutError:
            print(ResponseStrings.SOCKET_TIMED_OUT)
            if stopping:
                break
            continue
        except socket.timeout:
            print(ResponseStrings.SOCKET_TIMED_OUT)
            if stopping:
                break
            continue

        if message_id == MessageID.RES_SUCCESS:
            print(ResponseStrings.SUCCESS)
            if stopping:
                break
        elif message_id == MessageID.RES_FAILURE:
            print(ResponseStrings.FAILURE)
        elif message_id == MessageID.RES_ERROR:
            print(ResponseStrings.ERROR)
        else:
            if message_id == MessageID.RES_DATA:
                print(ResponseStrings.DATA_RECEIVED)
                if extra_data.__len__():
                    for line in extra_data:
                        print(line)
                else:
                    print(ResponseStrings.DATA_EMPTY)
            elif message_id == MessageID.REQ_RESEND:
                print(ResponseStrings.RESEND_NO_DATA)
            else:
                print(ResponseStrings.CLIENT_ERROR)

    print("Client is closing!")


thread = Thread(target=listener, args=(in_s, port + 1, ))
main()
sys.exit(0)
