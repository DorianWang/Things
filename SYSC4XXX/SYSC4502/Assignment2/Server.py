import sys
if sys.version_info < (3, 7):
    print("This script requires Python 3.7 or newer to run!")
    sys.exit(1)
import socket
import signal
from threading import Thread
import time
import struct
import random

import StateVariables
import DateTimeslot
import QueuedNetworking
from CommandDefinitions import NetConsts

RESERVATIONS_FILE_NAME = "reservations.txt"
ROOMS_FILE_NAME = "rooms.txt"
DAYS_FILE_NAME = "days.txt"
TIMESLOTS_FILE_NAME = "timeslots.txt"


def handler(signal_received, frame):
    StateVariables.is_running = False
    StateVariables.got_message.set()  # force the server to get a result
    print("Intercepted Quit, Cleaning Up!")


class Server:

    # Signal Handling Sockets
    error_listener: socket.socket
    error_writer: socket.socket

    server_socket: socket.socket
    reservation_manager: DateTimeslot.RoomDateTimeslotManager
    networking_manager: QueuedNetworking.QueuedNetworking

    def __init__(self, args) -> None:
        if len(sys.argv) < 3:
            print("Missing argument(s), requires: HOSTNAME PORT")
            sys.exit()
        else:
            multicast_group_name = sys.argv[1]
            running_port = int(sys.argv[2])

        StateVariables.hash_counter = 0
        StateVariables.last_x_ids = [0 for i in range(NetConsts.DB_HASH_BUFFER)]
        StateVariables.last_x_commands = [tuple() for i in range(NetConsts.DB_HASH_BUFFER)]

        # Creates a pair of sockets so that certain signals will instead get the server to export and clean up.
        self.error_listener, self.error_writer = socket.socketpair()
        self.error_listener.setblocking(False)
        signal.set_wakeup_fd(self.error_writer.fileno())

        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.server_socket.bind(('', running_port))

        group = socket.inet_aton(multicast_group_name)
        mreq = struct.pack('4sL', group, socket.INADDR_ANY)
        self.server_socket.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)

        # Initializes the database with the static files. Rebuilding the database will suck later on though...
        self.reservation_manager = DateTimeslot.\
            RoomDateTimeslotManager(ROOMS_FILE_NAME, DAYS_FILE_NAME, TIMESLOTS_FILE_NAME, RESERVATIONS_FILE_NAME)

        self.networking_manager = QueuedNetworking.QueuedNetworking(self.error_listener, self.server_socket)
        data = self.networking_manager.bootstrap(self.reservation_manager.get_hash(),
                                                 (multicast_group_name, running_port))
        for line in data:
            QueuedNetworking.process_message(self.reservation_manager, line, self.server_socket, tuple())

    def main_loop(self):

        self.networking_manager.start()
        # very primitive resending support
        old_payload = ([], ())
        output_counter = random.randint(0, NetConsts.DB_HASH_BUFFER)

        while StateVariables.is_running:
            data, address = self.networking_manager.get_next_packet(blocking=True)
            if data is None and address is None:
                break

            # Hash of value before command, and command itself.
            StateVariables.hash_list_mutex.acquire()
            StateVariables.last_x_ids[StateVariables.hash_counter] = self.reservation_manager.get_hash()
            StateVariables.last_x_commands[StateVariables.hash_counter] = data
            StateVariables.hash_counter = StateVariables.hash_counter + 1 % NetConsts.DB_HASH_BUFFER
            StateVariables.hash_list_mutex.release()

            # This returns a future value, which I currently do not use.
            QueuedNetworking.process_message(self.reservation_manager, data, self.server_socket, address)
            if StateVariables.hash_counter == output_counter:  # around every 4 commands, it should export.
                self.reservation_manager.export_reservations()

        print("Server is stopping!")
        self.server_socket.close()
        self.reservation_manager.export_reservations()


if __name__ == "__main__":
    signal.signal(signal.SIGINT, handler)
    signal.signal(signal.SIGBREAK, handler)
    new_server = Server(sys.argv)

    new_server.main_loop()

    sys.exit(0)
