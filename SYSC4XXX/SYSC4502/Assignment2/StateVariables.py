from threading import Event, Lock
# As a module is only run once, when loaded, definition and declaration can both happen here.
# I don't expect any more global variables, but just in case...
is_running = True
got_message = Event()

hash_list_mutex = Lock()
hash_counter: int
last_x_ids: list
last_x_commands: list
