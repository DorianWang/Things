
# This file holds all the command definitions, so as to keep the case statements tidy and synchronized.
# Both the client and server will use this file, but some messages are specific to the client.

class NetConsts:
    """Currently only has 1 value, this enum will likely increase in size when new functions are required"""
    TIMEOUT = 1  # assume that a packet is lost after 1 second


# http://www.ee.unb.ca/cgi-bin/tervo/polygen2.pl?d=4&p=11111&s=1&c=1&a=0&g=1
# Had lots of fun learning how this works, even though I don't think I'll ever write any algorithm like that.
# The generator used was x^4 + x^3 + 1, and I could even waste a few hours writing EC code later.
# There's nothing interesting here otherwise.
class MessageID:
    """
    These values specify what the first byte should be for each type of message.

    Names starting with REQ_ are for client requests to the server,
    while RES_ are for responses from the server.
    """
    NULL = 0x00
    REQ_ROOMS = 0x19
    REQ_DAYS = 0x2B
    REQ_TIMESLOTS = 0x32
    REQ_CHECK_ROOM = 0x4F
    REQ_MAKE_RESERVATION = 0x56
    REQ_DELETE_RESERVATION = 0x64

    REQ_RESEND = 0xE3  # currently not implemented
    REQ_STOP_SERVER = 0xFA

    RES_SUCCESS = 0x7D
    RES_FAILURE = 0x87
    RES_ERROR = 0x9E
    RES_DATA = 0xAC

    _UNUSED_COMMAND_11 = 0xB5
    _UNUSED_COMMAND_12 = 0xC8
    _UNUSED_COMMAND_13 = 0xD1


class CommandStrings:
    """These values specify what string corresponds to which command. Used for the client input."""
    ROOMS = "rooms"
    DAYS = "days"
    TIMESLOTS = "timeslots"
    CHECK_ROOM = "check"
    MAKE_RESERVATION = "reserve"
    DELETE_RESERVATION = "delete"
    STOP_SERVER = "quit"
    HELP = "help"


class ResponseStrings:
    """These strings may later be used with format() to provide the user feedback."""
    UNRECOGNIZED_COMMAND = "The input does not appear to be a valid command."
    SUCCESS = "The server has completed the request."
    FAILURE = "The server has not completed your request due to being invalid."
    ERROR = "The server experienced an error while processing your request. It has not been completed."
    CLIENT_ERROR = "The client has received an unexpected response."
    DATA_RECEIVED = "Server response:"

    HELP = "No help is coming, because I wasted too much time daydreaming."


