
# This file holds all the command definitions, so as to keep the case statements tidy and synchronized.
# Both the client and server will use this file, but some messages are specific to the client.

class NetConsts:
    """Currently only has 2 values, this enum will likely increase in size when new functions are required"""
    TIMEOUT = 1  # assume that a packet is lost after 1 second, this might be a bit too short for my behemoth...
    MAX_BUFFER = 2048  # How large of a buffer should the message data be allocated.

    MESSAGE_SENT_BUFFER = 4
    MESSAGE_RECEIVED_BUFFER = 8
    DB_HASH_BUFFER = 4


# http://www.ee.unb.ca/cgi-bin/tervo/polygen2.pl?d=4&p=11111&s=1&c=1&a=0&g=1
# Had lots of fun learning how this works, even though I don't think I'll ever write any algorithm like that.
# The generator used was x^4 + x^3 + 1, and with it I could even waste a few hours writing EC code later.
# There's nothing interesting here otherwise.
class MessageID:
    """
    These values specify what the first byte should be for each type of message.

    Names starting with REQ_ are generally for client requests to the server,
    while RES_ are for responses from the server.
    """
    NULL = 0x00
    REQ_ROOMS = 0x19
    REQ_DAYS = 0x2B
    REQ_TIMESLOTS = 0x32
    REQ_CHECK_ROOM = 0x4F
    REQ_MAKE_RESERVATION = 0x56
    REQ_DELETE_RESERVATION = 0x64

    REQ_RESEND = 0xE3
    REQ_STOP_SERVER = 0xFA

    RES_SUCCESS = 0x7D
    RES_FAILURE = 0x87
    RES_ERROR = 0x9E
    RES_DATA = 0xAC  # Used to indicate that the client should print additional data, is a form of success.

    SYN_REQ_JOIN = 0xB5
    SYN_MESSAGE = 0xC8

    _UNUSED_COMMAND_13 = 0xD1


class CommandStrings:
    """These values specify what string corresponds to which command. Used for the client input."""
    ROOMS = "rooms"
    DAYS = "days"
    TIMESLOTS = "timeslots"
    CHECK_ROOM = "check"
    MAKE_RESERVATION = "reserve"
    DELETE_RESERVATION = "delete"
    RESEND = "resend"
    STOP_SERVER = "quit"
    HELP = "help"


class ResponseStrings:
    """These strings may later be used with format() to provide the user feedback."""
    UNRECOGNIZED_COMMAND = "The input does not appear to be a valid command."
    SUCCESS = "The server has completed the request."
    FAILURE = "The server has not completed your request due to it being invalid."
    ERROR = "The server experienced an error while processing your request. It has not been completed."
    CLIENT_ERROR = "The client has received an unexpected response."
    SOCKET_TIMED_OUT = 'Client timed out! Use "resend" to see the last request the server processed.'
    DATA_RECEIVED = "Server response:"
    DATA_EMPTY = "No additional data was attached."
    RESEND_CUE = "Last completed request resulted in:"
    RESEND_NO_DATA = "Server does not have a previous result to return."

    HELP = "Try entering one of these commands: rooms days timeslots check reserve delete resend quit\n" \
        + "All values are currently sensitive to capitalization, so values must be entered as the server returns."
