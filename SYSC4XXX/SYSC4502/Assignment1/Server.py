
import socket
import sys
import time
import datetime
import DateTimeslot
import timeit
import signal
import select

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

    def __init__(self, args) -> tuple:
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

    def main_loop(self):
        global _is_running
        while _is_running:
            # print("Entered running!")
            ready_sockets = select.select([self.error_listener, self.server_socket], [], [])
            # print(ready_sockets)
            if ready_sockets[0][0] == self.error_listener or ready_sockets[0].__len__() == 2:
                # Set a flag to dump and clean up here.
                break
            data = self.server_socket.recv(2048)
            data = data.decode()

            data = data.strip(" \n\t")

        print("Out!")
        self.server_socket.close()


if __name__ == "__main__":
    signal.signal(signal.SIGINT, handler)
    signal.signal(signal.SIGBREAK, handler)
    new_server = Server(sys.argv)
    new_server.main_loop()

    sys.exit(0)
