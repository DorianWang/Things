import select
import socket
from threading import Thread, Lock
import time
import concurrent.futures.thread as cf_thread
import functools
import random
import struct

import StateVariables
import DateTimeslot
from CommandDefinitions import NetConsts, MessageID

_THREAD_POOL = cf_thread.ThreadPoolExecutor(max_workers=8)


def thread_pool_wrapper(f):
    @functools.wraps(f)
    def wrap(*args, **kwargs):
        return _THREAD_POOL.submit(f, *args, **kwargs)

    return wrap


@thread_pool_wrapper
def process_message(reservation_manager: DateTimeslot.RoomDateTimeslotManager,
                    current_message: bytes, sock: socket.socket, address: tuple):

    request_type, key_num, timestamp, data = parse_request(current_message)
    payload = bytearray()

    if request_type == MessageID.SYN_REQ_JOIN:
        payload.append(MessageID.SYN_MESSAGE)
        payload.append(key_num)
        payload = payload + struct.pack("<L", timestamp)
        recieved_hash = struct.unpack("<L", data[0])[0]
        StateVariables.hash_list_mutex.acquire()
        found_hash = -1
        for i in range(NetConsts.DB_HASH_BUFFER):
            if StateVariables.last_x_ids[i] == recieved_hash:
                found_hash = i
                break

        if found_hash == -1:
            # Shrug, this could be a lot of different things.
            print("Possible desync!")
        else:
            while found_hash != StateVariables.hash_counter:
                payload = payload + StateVariables[found_hash]
                payload.append(MessageID.NULL)
                found_hash = (found_hash + 1) % NetConsts.DB_HASH_BUFFER
        sock.sendto(payload, address)
        StateVariables.hash_list_mutex.release()
        return payload

    if request_type == MessageID.NULL:
        print("Message invalid?")
    elif request_type == MessageID.REQ_ROOMS:
        payload.append(MessageID.RES_DATA)
        payload.append(key_num)
        payload.extend(struct.pack("<L", timestamp))
        payload.extend((chr(MessageID.NULL)).join(reservation_manager.get_rooms()).encode("utf-8"))
        payload.append(MessageID.NULL)
    elif request_type == MessageID.REQ_DAYS:
        payload.append(MessageID.RES_DATA)
        payload.append(key_num)
        payload.extend(struct.pack("<L", timestamp))
        payload.extend((chr(MessageID.NULL)).join(reservation_manager.get_days()).encode("utf-8"))
        payload.append(MessageID.NULL)
    elif request_type == MessageID.REQ_TIMESLOTS:
        payload.append(MessageID.RES_DATA)
        payload.append(key_num)
        payload.extend(struct.pack("<L", timestamp))
        payload.extend((chr(MessageID.NULL)).join(reservation_manager.get_timeslots()).encode("utf-8"))
        payload.append(MessageID.NULL)

    elif request_type == MessageID.REQ_CHECK_ROOM:
        if data.__len__() == 0:
            payload.append(MessageID.RES_ERROR)
            payload.append(key_num)
            payload.extend(struct.pack("<L", timestamp))
        else:
            try:
                temp = (chr(MessageID.NULL)).join(
                    reservation_manager.get_reservations_with_values(room=data[0])).encode()
                payload.append(MessageID.RES_DATA)
                payload.append(key_num)
                payload.extend(struct.pack("<L", timestamp))
                payload = payload + temp
                payload.append(MessageID.NULL)
            except ValueError:
                # This happens if the provided room does not exist.
                payload.append(MessageID.RES_ERROR)

    elif request_type == MessageID.REQ_MAKE_RESERVATION:
        if data.__len__() != 3:
            payload.append(MessageID.RES_ERROR)
            payload.append(key_num)
            payload.extend(struct.pack("<L", timestamp))
        else:
            value = reservation_manager.add_reservation(data[0], data[1], data[2])
            if value == 0:
                payload.append(MessageID.RES_SUCCESS)
            elif value == 1:
                payload.append(MessageID.RES_FAILURE)
            else:
                payload.append(MessageID.RES_ERROR)
            payload.append(key_num)
            payload.extend(struct.pack("<L", timestamp))
    elif request_type == MessageID.REQ_DELETE_RESERVATION:
        if data.__len__() != 3:
            payload.append(MessageID.RES_ERROR)
            payload.append(key_num)
            payload.extend(struct.pack("<L", timestamp))
        else:
            value = reservation_manager.remove_reservation(data[0], data[1], data[2])
            if value == 0:
                payload.append(MessageID.RES_SUCCESS)
            elif value == 1:
                payload.append(MessageID.RES_FAILURE)
            else:
                payload.append(MessageID.RES_ERROR)
            payload.append(key_num)
            payload.extend(struct.pack("<L", timestamp))
    elif request_type == MessageID.SYN_MESSAGE:
        pass  # If we weren't expecting a message, just ignore it.
    else:
        print("Unknown message?")
        print(data)

    # elif request_type == MessageID.REQ_RESEND:

    # elif request_type == MessageID.REQ_STOP_SERVER:
    #    reservation_manager.export_reservations(RESERVATIONS_FILE_NAME)
    #    StateVariables.is_running = False
    #    payload.append(MessageID.RES_SUCCESS)

    time.sleep(random.randint(NetConsts.SERVER_MIN_DELAY, NetConsts.SERVER_MAX_DELAY))  # server is "busy"
    if len(address) != 0:
        sock.sendto(payload, address)
    return payload


def parse_request(client_data) -> tuple:
    if client_data.__len__() < 6:
        print("Improper data recieved!")
        return MessageID.NULL, 0, 0, []

    message_id = client_data[0]
    message_key_num = client_data[1]
    message_timestamp = struct.unpack("<L", client_data[2:6])[0]
    data = []
    if client_data.__len__() > 6:
        payload = (client_data[6:]).decode("utf-8")
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


class QueuedNetworking(Thread):
    front: int
    back: int
    _messages: list
    _last_x_ids: list
    server_socket: socket.socket

    error_listener: socket.socket

    front_lock = Lock()

    def __init__(self, error_listener, server_socket):
        Thread.__init__(self)
        self.error_listener = error_listener
        self.server_socket = server_socket
        self.front = 0  # The index of where the first value in the list should be
        self.back = 0  # The index immediately after the last value in the list. Total values = back - front
        self._messages = [(0, 0) for i in range(NetConsts.MESSAGE_RECEIVED_BUFFER)]

    def bootstrap(self, starting_hash, multicast_address) -> list:
        message = bytearray()
        message.append(MessageID.SYN_REQ_JOIN)
        key_val = random.randint(0, 255)
        message.append(key_val)
        message_timestamp = struct.pack("<L", int(time.time()))  # pack() should convert properly, but just making sure
        message = message + message_timestamp
        message = message + struct.pack("<L", starting_hash)
        message.append(MessageID.NULL)
        self.server_socket.sendto(message, multicast_address)
        time.sleep(NetConsts.TIMEOUT)
        temp_buffer = []
        self.server_socket.setblocking(False)
        server_response = None
        while True:
            try:
                packet = self.server_socket.recvfrom(NetConsts.MAX_BUFFER)
                data, address = packet
                if data[0] == MessageID.SYN_MESSAGE:
                    if data[1] == key_val:
                        server_response = data
                    else:
                        pass  # ignore messages that aren't for us.
                elif data[0] == MessageID.SYN_REQ_JOIN:
                    pass  # don't accept join requests until bootstrapping done
                else:
                    # This stores the extra client requests received so that this server can catch up.
                    self.push_packet(packet)
            except BlockingIOError:  # This is the windows error, I'm not going to bother finding the linux or mac ones.
                self.server_socket.setblocking(True)
                break
        if server_response:
            message_id, message_key_num, message_timestamp, data = parse_request(server_response)
            print("Getting a boost from a previous server!")
            print(data)
            return data
        print("I am alone.")
        return []

    def get_next_packet(self, blocking=True) -> tuple:
        if self.front == self.back:
            if blocking:
                StateVariables.got_message.wait()
                if not StateVariables.is_running:
                    return None, None

                self.front_lock.acquire()
                temp = self._messages[self.front]
                self.front = (self.front + 1) % NetConsts.MESSAGE_RECEIVED_BUFFER
                StateVariables.got_message.clear()
                self.front_lock.release()

                return temp
            # Not blocking, just give back nothing
            return None, None
        else:
            self.front_lock.acquire()
            temp = self._messages[self.front]
            self.front = (self.front + 1) % NetConsts.MESSAGE_RECEIVED_BUFFER
            StateVariables.got_message.clear()
            self.front_lock.release()

            return temp

    def push_packet(self, packet: tuple) -> None:
        data, address = packet
        for i in range(NetConsts.MESSAGE_RECEIVED_BUFFER):
            if self._messages[i][1] == data:
                # Message already in buffer, ignore.
                return

        self.front_lock.acquire()
        if (self.back + 1) % NetConsts.MESSAGE_RECEIVED_BUFFER == self.front:
            # The buffer is full!
            # Either respond that buffer is full, or maybe just block until free?
            # I think the best choice for now is to just print an error and drop the packet.
            print("Buffer full! Packet is dropped!")
            self.front_lock.release()
            return

        self._messages[self.back] = (data, address)
        self.back = (self.back + 1) % NetConsts.MESSAGE_RECEIVED_BUFFER
        self.front_lock.release()
        StateVariables.got_message.set()

    def run(self):
        print("Server is now ready to receive.")
        while StateVariables.is_running:
            ready_sockets = select.select([self.error_listener, self.server_socket], [], [])
            if ready_sockets[0][0] == self.error_listener or ready_sockets[0].__len__() == 2:
                # This will force a blocked call to get_next_packet() to finish
                StateVariables.got_message.set()
                break

            packet = self.server_socket.recvfrom(NetConsts.MAX_BUFFER)
            self.push_packet(packet)




