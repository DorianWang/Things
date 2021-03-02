import sys
if sys.version_info < (3, 7):
    print("This script requires Python 3.7 or newer to run!")
    sys.exit(1)
# import threading
from dataclasses import dataclass, field
import os
import DBConsistencyFncs

# Flags
CROSS_DATE_TIMESLOTS = False

# Constants
MINUTES_IN_HOUR = 60
MINUTES_IN_DAY = 1440
HOURS_IN_DAY = 24

TEMP_RESERVATION_FILE_NAME = "reservations_temp"


# Timeslots measure time in minutes, with 0 minutes signifying 00:00, and MINUTES_IN_HOUR * HOURS_IN_DAY for 24:00
# 00:00 is the beginning of a day and functionally equal to 24:00 of the previous day.
@dataclass(unsafe_hash=True)
class _Timeslot:
    """Holds a start and end times of a timeslot."""
    start: int = field(default=0)
    end: int = field(default=0)

    def __init__(self, times: tuple):
        """Creates the timeslot using two 24 hour clock strings in a tuple.
        The starting time cannot be 24:00 and the end time cannot be 00:00.

        Keyword arguments:
        times -- A tuple containing the start time and end time. (Two strings of format "HH:MM" or "H:MM")
        """
        new_start_time, new_end_time = times
        if new_start_time.__len__() < 4 or new_start_time.__len__() > 5 or \
                new_end_time.__len__() < 4 or new_end_time.__len__() > 5:
            raise ValueError("The time strings are not properly sized.")

        hour, minute = new_start_time.split(":")
        start_time = int(hour) * MINUTES_IN_HOUR + int(minute)
        if start_time >= MINUTES_IN_DAY:
            raise ValueError("The start time is not in bounds.")

        hour, minute = new_end_time.split(":")
        end_time = int(hour) * MINUTES_IN_HOUR + int(minute)
        if end_time > MINUTES_IN_DAY or end_time == 0:
            raise ValueError("The end time is not in bounds.")

        diff = end_time - start_time
        if diff > 0:
            self.start = start_time
            self.end = end_time
        elif CROSS_DATE_TIMESLOTS:
            self.start = start_time
            self.end = diff + MINUTES_IN_DAY  # Equal to 24 hours + end - start
        else:
            raise ValueError("The difference between the two times is not in bounds.")

    @classmethod
    def new_timeslot_24hour(cls, new_timeslot: str):
        """Returns a timeslot set using a single timeslot string.
        See __init__() for additional formatting information.

        Keyword arguments:
        new_timeslot -- A string of the form HH:MM-HH:MM, with the starting time first.
        """
        return cls(_Timeslot.split_24hour_timeslot(new_timeslot))

    @staticmethod
    def split_24hour_timeslot(new_timeslot: str) -> tuple:
        start_time, end_time = new_timeslot.strip("\t\r\n,").split("-")
        return start_time, end_time

    def get_timeslot_24hour_tuple(self) -> tuple:
        """Returns two 24 hour clock strings in the format HH:MM."""
        start_time = str(self.start // 60).zfill(2) + ":" + str(self.start % 60).zfill(2)

        if CROSS_DATE_TIMESLOTS:
            end_time = ""
            assert NotImplementedError
        else:
            end_time = str(self.end // 60).zfill(2) + ":" + str(self.end % 60).zfill(2)
        return start_time, end_time

    def __str__(self):
        a, b = self.get_timeslot_24hour_tuple()
        return a + "-" + b

    def __lt__(self, other):
        """Returns true when a timeslot ends before or at the same time that the other starts."""
        if self.end <= other.start:
            return True
        return False

    def __le__(self, other):
        """Returns true when a timeslot starts before the other does, but ends during the other."""
        if self.start < other.start:
            if other.end > self.end > other.start:
                return True
        return False

    def __gt__(self, other):
        """Returns true when a timeslot starts after or at the same time that the other ends."""
        if self.start >= other.end:
            return True
        return False

    def __ge__(self, other):
        """Returns true when a timeslot starts before the other ends, but ends after the other ends."""
        if self.start > other.start:
            if other.end < self.end < other.start:
                return True
        return False

    def __and__(self, other):
        """Returns true when the two timeslots have any overlap in assigned times"""
        if self.end <= other.start or self.start >= other.end:
            return False
        return True


@dataclass
class DateTimeslot:
    """Currently unused, but I'll keep it around in case the next assignment needs it"""
    date: int = field(default=-1)
    time: _Timeslot = field(default_factory=_Timeslot)

    def set_timeslot(self, new_time: _Timeslot):
        self.time = new_time

    def set_date(self, new_date: int):
        self.date = new_date


class RoomDateTimeslotManager:
    _days_dict: dict
    _timeslots_dict: dict
    _rooms_dict: dict
    _all_slots_set: set  # For now it will just be a static list of reservation IDs.
    _DB_hash: int

    def __init__(self, room_file_name="rooms.txt", date_file_name="days.txt", timeslot_file_name="timeslots.txt",
                 reservations_file_name="reservations.txt"):
        self._rooms_dict = dict()
        self._days_dict = dict()
        self._timeslots_dict = dict()
        self._all_slots_set = set()
        self._DB_hash = 0

        with open(room_file_name) as room_file:
            for line in room_file:  # remove leading and trailing spaces and punctuation
                self._rooms_dict[line.strip(" \t\r\n,.")] = self._rooms_dict.__len__()

        with open(date_file_name) as date_file:
            for line in date_file:
                self._days_dict[line.strip(" \t\r\n,.")] = self._days_dict.__len__()

        with open(timeslot_file_name) as timeslot_file:
            for line in timeslot_file:
                self._timeslots_dict[_Timeslot.new_timeslot_24hour(line.strip(" \t\r\n,."))] \
                    = (self._timeslots_dict.__len__())

        # Sets the reservation ID modifiers
        self.__rooms_multiplier = self._timeslots_dict.__len__() * self._days_dict.__len__()
        self.__days_multiplier = self._timeslots_dict.__len__()

        with open(reservations_file_name) as current_reservations:
            self.load_reservations(current_reservations)

    def __res_id_from_values(self, room: str, timeslot: str, day: str):
        # Note all exceptions must be handled by caller.
        return (self._rooms_dict[room] * self.__rooms_multiplier + self._days_dict[day] * self.__days_multiplier
                + self._timeslots_dict[_Timeslot.new_timeslot_24hour(timeslot)])

    def __parse_reservation(self, new_reservation: str) -> int:
        new_reservation = new_reservation.strip(" \n\t")

        room, timeslot, day = new_reservation.split()
        return (self._rooms_dict[room] * self.__rooms_multiplier + self._days_dict[day] * self.__days_multiplier
                + self._timeslots_dict[_Timeslot.new_timeslot_24hour(timeslot)])

    def load_reservations(self, input_file):
        """Takes a file handle (generally from the reservations.txt file), extracts the reservations,
         and then sets them as reserved. This data is assumed to be clean, so any problem is an error.
         The caller is responsible for opening and closing the file.

        Keyword arguments:
        current_reservations -- A list of strings with each string in the format "ROOM TIMESLOT DAY"
        """
        for rev in input_file:
            try:
                self._all_slots_set.add(self.__parse_reservation(rev))
            except KeyError:
                sys.stderr.write("Invalid key in line: " + rev)
                sys.stderr.write("This reservation has been ignored!")

    def add_reservation(self, room: str, timeslot: str, day: str) -> int:
        """Takes 3 string values, the room, timeslot and day, and attempts to add the
         and then sets them as reserved. This function returns True if a reservation is added, or false otherwise.
         Returns 0 if successful, 1 if reservation already exists, and negative values for exceptions.

        Keyword arguments:
        new_reservation -- A string in the format "ROOM TIMESLOT DAY"
        """
        try:
            temp_res = self.__res_id_from_values(room, timeslot, day)
        except KeyError:
            print("Invalid key in add_reservation!")
            return -1

        if temp_res in self._all_slots_set:
            return 1  # Reservation already in set, return error message to client
        self._all_slots_set.add(temp_res)
        return 0

    def remove_reservation(self, room: str, timeslot: str, day: str) -> int:
        """Takes 3 strings defining a reservation, and checks if it exists. If it does the reservation is removed.
         Returns 0 if successful at removal, 1 if reservation does not exists, and negative values for exceptions.

        Keyword arguments:
        reservation_to_remove -- A string in the format "ROOM TIMESLOT DAY"
        """
        try:
            temp_reservation_id = self.__res_id_from_values(room, timeslot, day)
        except KeyError:
            print("Invalid key in add_reservation!")
            return -1

        if temp_reservation_id in self._all_slots_set:
            self._all_slots_set.remove(temp_reservation_id)
            return 0

        return 1

    def export_reservations(self, reservations_file_name="reservations.txt"):
        with open(TEMP_RESERVATION_FILE_NAME, mode="wt") as reservation_file:
            for res_id in self._all_slots_set:
                reservation_file.write(self.get_reservation_string(res_id) + "\n")
        os.replace(TEMP_RESERVATION_FILE_NAME, reservations_file_name)

    def get_days(self):
        return self._days_dict.keys()

    def get_timeslots(self):
        temp = self._timeslots_dict.keys()
        return [timeslot.__str__() for timeslot in temp]

    def get_rooms(self):
        return self._rooms_dict.keys()

    def get_reservation_string(self, res_id):

        timeslot_id = res_id % self.__days_multiplier
        day_id = (res_id % self.__rooms_multiplier) // self.__days_multiplier
        room_id = res_id // self.__rooms_multiplier

        room = None
        timeslot = None
        day = None

        for key, value in self._rooms_dict.items():
            if value == room_id:
                room = key
        for key, value in self._timeslots_dict.items():
            if value == timeslot_id:
                timeslot = key.__str__()
        for key, value in self._days_dict.items():
            if value == day_id:
                day = key

        reservation_string = room + " " + timeslot + " " + day
        return reservation_string

    def get_reservations_with_values(self, room=None, date=None, timeslot=None) -> list:
        matching_reservations = []
        room_id = 0
        date_id = 0
        timeslot_id = 0
        # For each possible
        try:
            if room is not None:
                room_id = self._rooms_dict[room]
            if date is not None:
                date_id = self._days_dict[date]
            if timeslot is not None:
                timeslot_id = self._timeslots_dict[_Timeslot.new_timeslot_24hour(timeslot)]
        except KeyError:
            raise ValueError

        for reservation in self._all_slots_set:

            # If all the checked values equal to the provided one, then add the value to the array
            if room is not None:
                if reservation // self.__rooms_multiplier != room_id:
                    continue
            if date is not None:
                if (reservation % self.__rooms_multiplier) // self.__days_multiplier != date_id:
                    continue
            if timeslot is not None:
                if reservation % self.__days_multiplier != timeslot_id:
                    continue

            matching_reservations.append(self.get_reservation_string(reservation))

        return matching_reservations
