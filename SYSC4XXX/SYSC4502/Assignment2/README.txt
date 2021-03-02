The valid inputs for the client are:
rooms
days
timeslots
check [room]
reserve [room] {timeslot} [day]
delete [room] {timeslot} [day]
*resend* Currently not implemented
quit
help

All commands must be entered in lowercase, future versions may allow capitalization.
In addition, all command arguments are currently case sensitive, and times should be entered without spaces.
Timeslots must be in one of these three forms: HH:MM-HH:MM     H:MM-H:MM     H:MM-HH:MM

This time I kinda rushed things, and I didn't do much testing.