from threading import Lock, Event
# As a module is only run once, when loaded, definition and declaration can both happen here.
# I don't expect any more global variables, but just in case...
is_running = True
got_message = Event()

