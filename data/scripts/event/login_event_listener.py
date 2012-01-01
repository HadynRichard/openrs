from event import *

def handle_login(event):
	print username(event.player), 'has logged in from', ip_address(event.player)

def handle_logout(event):
	print username(event.player), 'from', ip_address(event.player), 'has logged out'

on(EventType.PLAYER_LOGIN, handle_login)
on(EventType.PLAYER_LOGOUT, handle_logout)
