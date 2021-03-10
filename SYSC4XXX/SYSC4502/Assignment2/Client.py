import random
import sys
import time

if sys.version_info < (3, 7):
    print("This script requires Python 3.7 or newer to run!")
    sys.exit(1)
import struct
import socket

import StateVariables
from CommandDefinitions import MessageID, CommandStrings, ResponseStrings, NetConsts


StateVariables.is_running = True
current_packet_number = 0


def parse_response(server_data: bytes) -> tuple:
    """
    This function takes data returned from the server and returns the MessageID byte
    together with string values in a list. The resulting list may be empty.
    """

    if server_data.__len__() < 6:
        return MessageID.NULL, 0, 0, []

    message_id = server_data[0]
    message_key_num = server_data[1]
    message_timestamp = struct.unpack("<L", server_data[2:6])[0]
    data = []
    if server_data.__len__() > 6:
        payload = (server_data[6:]).decode("utf-8")
        last_slice_position = -1
        for i in range(payload.__len__()):
            if payload[i] == '\x00':  # if the character is null, it is the end of the string.
                data.append(payload[last_slice_position + 1:i])
                last_slice_position = i
                continue

            if payload[i] == '\n':  # Added support for windows line endings because I felt like it.
                if i > 0 and payload[i - 1] == '\r':
                    data.append(payload[last_slice_position + 1:i - 1])
                    last_slice_position = i
                else:
                    data.append(payload[last_slice_position + 1:i])
                    last_slice_position = i
        if last_slice_position + 1 < payload.__len__():  # If the message was not 0 terminated add the last part.
            data.append(payload[last_slice_position + 1:])
    return message_id, message_key_num, message_timestamp, data



def main():

    if len(sys.argv) < 3:
        print("Missing argument(s), requires: HOSTNAME PORT")
        sys.exit()
    else:
        server_name = sys.argv[1]
        running_port = int(sys.argv[2])
        print("Client will send to : " + server_name + ":" + str(running_port))

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    client_socket.settimeout(NetConsts.CLIENT_TIMEOUT)

    # stopping = 0  # This variable is used to close the client after sending the stop command to the server.

    print("What command do you wish to send to the server?")
    while True:
        user_input = input("Enter Command: ")
        if user_input == "":  # ignoring empty inputs
            continue
        user_input = user_input.strip()
        user_input = list(filter(None, user_input.split(" ")))  # this removes any empty inputs, when there are 2 spaces

        payload = bytearray()
        key_val = random.randint(0, 255)
        timestamp = int(time.time())
        centre_data = bytearray()
        centre_data.append(key_val)
        centre_data = centre_data + struct.pack("<L", timestamp)

        if not user_input:  # Just ignore empty strings
            continue
        elif user_input[0] == CommandStrings.ROOMS:
            payload.append(MessageID.REQ_ROOMS)
            payload += centre_data
        elif user_input[0] == CommandStrings.DAYS:
            payload.append(MessageID.REQ_DAYS)
            payload += centre_data
        elif user_input[0] == CommandStrings.TIMESLOTS:
            payload.append(MessageID.REQ_TIMESLOTS)
            payload += centre_data

        elif user_input[0] == CommandStrings.CHECK_ROOM:
            if user_input.__len__() < 2:
                print("Please specify a room to check for!")
                continue
            payload.append(MessageID.REQ_CHECK_ROOM)
            payload += centre_data
            payload = payload + user_input[1].encode()
            payload.append(MessageID.NULL)

        elif user_input[0] == CommandStrings.MAKE_RESERVATION:
            if user_input.__len__() < 4:
                print("Not enough arguments to make a reservation!")
                continue
            payload.append(MessageID.REQ_MAKE_RESERVATION)
            payload += centre_data
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
            payload += centre_data
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
            # payload.append(MessageID.REQ_RESEND)
            print(ResponseStrings.RESEND_CUE)
            continue

        elif user_input[0] == CommandStrings.STOP_SERVER:
            # payload.append(MessageID.REQ_STOP_SERVER)
            # stopping = 1  # mark client for stopping
            break
        else:
            print(ResponseStrings.UNRECOGNIZED_COMMAND)
            continue

        print(payload)
        print(payload.__len__())
        client_socket.sendto(payload, (server_name, running_port))

        try:
            message_id = MessageID.RES_ERROR
            extra_data = []
            while StateVariables.is_running:
                server_response = client_socket.recv(NetConsts.MAX_BUFFER)
                message_id, key_id, rec_timestamp, extra_data = parse_response(server_response)
                if key_id == key_val and rec_timestamp == timestamp:
                    break
        except TimeoutError:
            print(ResponseStrings.SOCKET_TIMED_OUT)
            # if stopping:
            #    break
            continue
        except socket.timeout:
            print(ResponseStrings.SOCKET_TIMED_OUT)
            # if stopping:
            #    break
            continue

        if message_id == MessageID.RES_SUCCESS:
            print(ResponseStrings.SUCCESS)
            # if stopping:
            #    break
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


main()
sys.exit(0)
