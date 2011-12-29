from event import *

def handle_login(event):
	print username(event.player), 'has logged in'

on(EventType.PLAYER_LOGIN, handle_login)
