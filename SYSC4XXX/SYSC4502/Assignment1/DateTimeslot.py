import sys
if sys.version_info < (3, 7):
    print("This script requires Python 3.7 or newer to run!")
    sys.exit(1)
import math
from dataclasses import dataclass, field

# Flags
CROSS_DATE_TIMESLOTS = False

# Constants
MINUTES_IN_HOUR = 60
MINUTES_IN_DAY = 1440
HOURS_IN_DAY = 24


# Timeslots measure time in minutes, with 0 minutes signifying 00:00, and MINUTES_IN_HOUR * HOURS_IN_DAY for 24:00
# 00:00 is the beginning of a day and functionally equal to 24:00 of the previous day.
@dataclass(unsafe_hash=True)
class _Timeslot:
    """Holds a start value and duration of a timeslot."""
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
        start_time = str(math.floor(self.start / 60)).zfill(2) + ":" + str(self.start % 60).zfill(2)

        if CROSS_DATE_TIMESLOTS:
            end_time = ""
            assert NotImplementedError
        else:
            end_time = str(math.floor(self.end + self.end / 60)).zfill(2) + ":" + str(self.end % 60).zfill(2)
        return start_time, end_time

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
    days_dict: dict
    timeslots_dict: dict
    rooms_dict: dict
    all_slots_flags: list  # For now it will just be a static list of reservations.

    def __init__(self, room_list, date_list, timeslot_list, current_reservations=[]):
        self.rooms_dict = dict()
        self.days_dict = dict()
        self.timeslots_dict = dict()

        for room in room_list:
            room = room.strip(" \t\r\n,.")
            self.rooms_dict[room] = self.rooms_dict.__len__()

        for day in date_list:
            day = day.strip(" \t\r\n,.")  # remove leading and trailing spaces and punctuation
            self.days_dict[day] = self.days_dict.__len__()

        for timeslot_line in timeslot_list:
            self.timeslots_dict[_Timeslot.new_timeslot_24hour(timeslot_line)] = (self.timeslots_dict.__len__())

        # Creates a 2D array with the first value as the day value, and the second the timeslot value
        self.all_slots_flags = [[[False for i in range(self.timeslots_dict.__len__())]
                                 for j in range(self.days_dict.__len__())]
                                for k in range(self.rooms_dict.__len__())]

        for reservation in current_reservations:
            # All these values are assumed to be clean.
            room, timeslot, day = reservation.split()
            self.all_slots_flags[self.rooms_dict[room]][self.days_dict[day]][
                self.timeslots_dict[_Timeslot.new_timeslot_24hour(timeslot)]] = True

        print(self.rooms_dict)
        print(self.days_dict)
        print(self.timeslots_dict)
        print(self.all_slots_flags)




















