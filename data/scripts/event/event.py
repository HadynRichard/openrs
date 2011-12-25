"""
event.py

Defines game events and their values.

Author: blakeman8192
"""

class EventType:
	""" Define event type constants. """
	PLAYER_LOGIN = 1
	PLAYER_LOGOUT = 2

class PlayerEvent:
	""" Define an event associated with a player. """
	def __init__(self, event_type, player):
		self.event_type = event_type
		self.player = player
