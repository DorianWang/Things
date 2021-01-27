import sys
import math

if sys.version_info < (3, 7):
    print("This script requires Python 3.7 or newer to run!")
    sys.exit(1)

from dataclasses import dataclass

# Flags
CROSS_DATE_TIMESLOTS = False

# Constants
MINUTES_IN_HOUR = 60
MINUTES_IN_DAY = 1440
HOURS_IN_DAY = 24


# Timeslots measure time in minutes, with 0 minutes signifying 00:00, and MINUTES_IN_HOUR * HOURS_IN_DAY 24:00
# 00:00 is the beginning of a day and functionally equal to 24:00 of the previous day.
# When
@dataclass
class _Timeslot:
    """Holds a start value and duration of a timeslot."""
    start: int
    duration: int

    def set_timeslot_24hour(self, new_start_time: str, new_end_time: str):
        """Sets the timeslot values using two 24 hour clock strings.

        Keyword arguments:
        new_start_time -- the start time (Format HH:MM or H:MM), 24:00 excluded
        new_end_time -- the end time (Format HH:MM or H:MM), 00:00 excluded
        """
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
            self.duration = diff
        elif CROSS_DATE_TIMESLOTS:
            self.start = start_time
            self.duration = diff + MINUTES_IN_DAY
        else:
            raise ValueError("The difference between the two times is not in bounds.")

    def get_timeslot_24hour(self):
        """Returns two 24 hour clock strings in the format HH:MM."""
        start_time = str(math.floor(self.start / 60)) + ":" + str(self.start % 60)
        if CROSS_DATE_TIMESLOTS:
            end_time = ""
            assert NotImplementedError
        else:
            end_time = str(math.floor(self.start + self.duration / 60)) + ":" + str(self.start % 60)
        return start_time, end_time


@dataclass
class _DateTimeslot:
    time: _Timeslot
    date: int





def startup():
    if len(sys.argv) < 3:
        print("Missing argument: Port")
        sys.exit()
    else:
        server_name = sys.argv[1]
        running_port = int(sys.argv[2])
        print("Client will send to : " + server_name + ":" + str(running_port))
    rooms = []
    timeslots = []
    days = []
    reservations = []
    with open("rooms.txt") as f:
        pass



def cleanup():

    sys.exit(0)





def main():
    while 1:
        print("What command do you wish to send to ")
        break
    sys.exit(0)

