"""
event_receiver.py

Receives raw events from the server engine and passes them to a more
object-oriented Python event handling system.

Author: blakeman8192
"""

from event.event import EventType
from event.event import PlayerEvent

# The event map data structure.
event_map = { }

def on(event_type, *listeners):
	""" Register a series of functions for handling a specific event type. """
	
	# Initialize the event listener list if it isn't present
	if event_type not in event_map:
		event_map[event_type] = []
		
	# Add the listeners to the listener list
	for listener in listeners:
		event_map[event_type].append(listener)

def broadcast(event_type, event):
	""" Broadcast the event to all interested listeners. """
	for listener in event_map[event_type]:
		listener(event)

def on_login(player):
	""" Handle player login event. """
	broadcast(EventType.PLAYER_LOGIN, PlayerEvent(EventType.PLAYER_LOGIN, player))

def on_logout(player):
	""" Handle player logout event. """
	broadcast(EventType.PLAYER_LOGOUT, PlayerEvent(EventType.PLAYER_LOGOUT, player))
