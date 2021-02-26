import select
import socket
from threading import Thread, Lock

import StateVariables
from CommandDefinitions import NetConsts


class QueuedNetworking(Thread):
    front: int
    back: int
    _messages: list
    server_socket: socket.socket

    error_listener: socket.socket

    front_lock = Lock()

    def __init__(self, error_listener):
        Thread.__init__(self)
        self.error_listener = error_listener

    def get_next_packet(self, blocking=True):
        if self.front == self.back:
            if blocking:
                StateVariables.got_message.wait()

                self.front_lock.acquire()
                temp = self._messages[self.front]
                self.front = (self.front + 1) % NetConsts.MESSAGE_RECEIVED_BUFFER
                StateVariables.got_message.clear()
                self.front_lock.release()

                return temp
            # Not blocking, just give back nothing
            return None
        else:
            self.front_lock.acquire()
            temp = self._messages[self.front]
            self.front = (self.front + 1) % NetConsts.MESSAGE_RECEIVED_BUFFER
            StateVariables.got_message.clear()
            self.front_lock.acquire()

            return temp

    def run(self):
        while StateVariables.is_running:
            ready_sockets = select.select([self.error_listener, self.server_socket], [], [])
            if ready_sockets[0][0] == self.error_listener or ready_sockets[0].__len__() == 2:
                # Stop running, which should end up killing this thread.
                break

            data, client_address = self.server_socket.recvfrom(NetConsts.MAX_BUFFER)
            self.front_lock.acquire()
            if (self.front + 1) % NetConsts.MESSAGE_RECEIVED_BUFFER == self.back:
                # The buffer is full!
                # Either respond that buffer is full, or maybe just block until free?
                # I think the best choice is to
                self.front_lock.release()
                pass
            self.front_lock.release()

            pass


