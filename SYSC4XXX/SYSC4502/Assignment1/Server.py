
import socket
import sys
import time
import datetime
import DateTimeslot
import timeit
import signal
import select
from CommandDefinitions import NetConsts, MessageID


RESERVATIONS_FILE_NAME = "reservations.txt"
_is_running = True


def handler(signal_received, frame):
    global _is_running
    _is_running = False
    print("Intercepted Quit, Cleaning Up!")


class Server:

    error_listener: socket.socket
    error_writer: socket.socket

    server_socket: socket.socket
    reservation_manager: DateTimeslot.RoomDateTimeslotManager

    def __init__(self, args) -> None:
        if len(args) < 2:
            print("Missing argument: Port")
            sys.exit(1)
        else:
            running_port = int(args[1])
            print("Server will run on port: " + str(running_port))

        self.error_listener, self.error_writer = socket.socketpair()
        self.error_listener.setblocking(False)
        signal.set_wakeup_fd(self.error_writer.fileno())

        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.server_socket.bind(('localhost', running_port))

        rooms = []
        days = []
        times = []
        reservations = []
        with open("rooms.txt") as f:
            rooms = f.readlines()

        with open("days.txt") as f:
            days = f.readlines()

        with open("timeslots.txt") as f:
            times = f.readlines()


# print(timeit.timeit("x = DateTimeslot._Timeslot.new_timeslot_24hour('7:30-12:30')", setup='import DateTimeslot'))
# print(timeit.timeit("x = [int('1234'), int('5678')]", setup='import DateTimeslot'))
        self.reservation_manager = DateTimeslot.RoomDateTimeslotManager(rooms, days, times, RESERVATIONS_FILE_NAME)

    @staticmethod
    def parse_request(client_data) -> tuple:
        if client_data.__len__() == 0:
            return MessageID.NULL, []

        ID = client_data[0]
        data = []
        if client_data.__len__() > 1:
            payload = (client_data[1:]).decode("utf-8")
            last_slice_position = -1
            for i in range(payload.__len__()):
                if payload[i] == '\x00':  # if the character is null, it is the end of the string.
                    data.append(payload[last_slice_position + 1:i])
                    last_slice_position = i
                    continue

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

    def main_loop(self):
        global _is_running

        # very primitive resending support
        old_payload = ([], ())

        while _is_running:
            ready_sockets = select.select([self.error_listener, self.server_socket], [], [])
            if ready_sockets[0][0] == self.error_listener or ready_sockets[0].__len__() == 2:
                # Set a flag to dump and clean up here.
                break

            data, client_address = self.server_socket.recvfrom(NetConsts.MAX_BUFFER)
            request_type, data = self.parse_request(data)
            payload = bytearray()
            if request_type == MessageID.NULL:
                print("Message invalid?")
                pass
            elif request_type == MessageID.REQ_ROOMS:
                payload.append(MessageID.RES_DATA)
                payload.extend((chr(MessageID.NULL)).join(self.reservation_manager.get_rooms()).encode("utf-8"))
                payload.append(MessageID.NULL)
            elif request_type == MessageID.REQ_DAYS:
                payload.append(MessageID.RES_DATA)
                payload.extend((chr(MessageID.NULL)).join(self.reservation_manager.get_days()).encode("utf-8"))
                payload.append(MessageID.NULL)
            elif request_type == MessageID.REQ_TIMESLOTS:
                payload.append(MessageID.RES_DATA)
                payload.extend((chr(MessageID.NULL)).join(self.reservation_manager.get_timeslots()).encode("utf-8"))
                payload.append(MessageID.NULL)
            elif request_type == MessageID.REQ_CHECK_ROOM:
                if data.__len__() == 0:
                    payload.append(MessageID.RES_ERROR)
                else:
                    payload.append(MessageID.RES_DATA)
                    payload = payload + (chr(MessageID.NULL)).join(
                        self.reservation_manager.get_reservations_with_values(room=data[0])).encode()
                    payload.append(MessageID.NULL)

            elif request_type == MessageID.REQ_MAKE_RESERVATION:
                if data.__len__() != 3:
                    payload.append(MessageID.RES_ERROR)
                else:
                    value = self.reservation_manager.add_reservation(data[0], data[1], data[2])
                    if value == 0:
                        payload.append(MessageID.RES_SUCCESS)
                    elif value == 1:
                        payload.append(MessageID.RES_FAILURE)
                    else:
                        payload.append(MessageID.RES_ERROR)
            elif request_type == MessageID.REQ_DELETE_RESERVATION:
                if data.__len__() != 3:
                    payload.append(MessageID.RES_ERROR)
                else:
                    value = self.reservation_manager.remove_reservation(data[0], data[1], data[2])
                    if value == 0:
                        payload.append(MessageID.RES_SUCCESS)
                    elif value == 1:
                        payload.append(MessageID.RES_FAILURE)
                    else:
                        payload.append(MessageID.RES_ERROR)
            elif request_type == MessageID.REQ_RESEND:
                if old_payload[1] == client_address:
                    payload = old_payload[0]
                else:
                    payload.append(MessageID.REQ_RESEND)

            self.server_socket.sendto(payload, client_address)
            print(payload)
            old_payload = (payload, client_address)


        print("Out!")
        self.server_socket.close()


if __name__ == "__main__":
    signal.signal(signal.SIGINT, handler)
    signal.signal(signal.SIGBREAK, handler)
    new_server = Server(sys.argv)
    new_server.main_loop()

    sys.exit(0)
