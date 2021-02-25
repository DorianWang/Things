from threading import Thread, Lock
import socket

import StateVariables
from CommandDefinitions import NetConsts


class QueuedNetworking(Thread):
    front: int
    back: int
    _messages: list
    server_socket: socket.socket

    front_lock = Lock()

    def __init__(self):
        Thread.__init__(self)

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
            pass


