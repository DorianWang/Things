import sys
import DateTimeslot


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

    with open("reservations.txt") as f:
        reservations = f.readlines()

    test = DateTimeslot.RoomDateTimeslotManager(rooms, days, times, reservations)
    print(test)
    print("Exiting!")
    sys.exit(0)


main()

