from pyapi import username
from event.event import EventType

def logged_in(event):
	print username(event.player), " has logged in"
	
def logged_out(event):
	print username(event.player), " has logged out"
	
# Register the event listener function
on(EventType.PLAYER_LOGIN, logged_in)
on(EventType.PLAYER_LOGOUT, logged_out)
