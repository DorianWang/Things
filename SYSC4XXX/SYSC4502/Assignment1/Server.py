import sys
if sys.version_info < (3, 7):
    print("This script requires Python 3.7 or newer to run!")
    sys.exit(1)
import socket
import DateTimeslot
import signal
import select
from CommandDefinitions import NetConsts, MessageID


RESERVATIONS_FILE_NAME = "reservations.txt"
ROOMS_FILE_NAME = "rooms.txt"
DAYS_FILE_NAME = "days.txt"
TIMESLOTS_FILE_NAME = "timeslots.txt"
_is_running = True


def handler(signal_received, frame):
    global _is_running
    _is_running = False
    print("Intercepted Quit, Cleaning Up!")


class Server:

    # Signal Handling Sockets
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

        # Creates a pair of sockets so that certain signals will instead get the server to export and clean up.
        self.error_listener, self.error_writer = socket.socketpair()
        self.error_listener.setblocking(False)
        signal.set_wakeup_fd(self.error_writer.fileno())

        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.server_socket.bind(('localhost', running_port))

        # Initializes the database with the static files. Rebuilding the database will suck later on though...
        self.reservation_manager = DateTimeslot.\
            RoomDateTimeslotManager(ROOMS_FILE_NAME, DAYS_FILE_NAME, TIMESLOTS_FILE_NAME, RESERVATIONS_FILE_NAME)

    @staticmethod
    def parse_request(client_data) -> tuple:
        if client_data.__len__() == 0:
            return MessageID.NULL, []

        message_id = client_data[0]
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
            if last_slice_position + 1 < payload.__len__():  # If the message was not 0 terminated add the last part.
                data.append(payload[last_slice_position+1:])
        return message_id, data

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
                continue
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
                    try:
                        temp = (chr(MessageID.NULL)).join(
                            self.reservation_manager.get_reservations_with_values(room=data[0])).encode()
                        payload.append(MessageID.RES_DATA)
                        payload = payload + temp
                        payload.append(MessageID.NULL)
                    except ValueError:
                        # This happens if the provided room does not exist.
                        payload.append(MessageID.RES_ERROR)

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

            elif request_type == MessageID.REQ_STOP_SERVER:
                self.reservation_manager.export_reservations(RESERVATIONS_FILE_NAME)
                _is_running = False
                payload.append(MessageID.RES_SUCCESS)

            self.server_socket.sendto(payload, client_address)
            old_payload = (payload, client_address)

        print("Server is stopping!")
        self.server_socket.close()


if __name__ == "__main__":
    signal.signal(signal.SIGINT, handler)
    signal.signal(signal.SIGBREAK, handler)
    new_server = Server(sys.argv)
    new_server.main_loop()

    sys.exit(0)
