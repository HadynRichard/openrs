/*
 * libpyapi.h
 *
 * Functions to provide Python with an API for reading and modifying the
 * game server state.
 *
 * Author: blakeman8192
 */

#ifndef LIBPYAPI_H
#define LIBPYAPI_H

#include <Python.h>
#include <arpa/inet.h>

#include "world.h"
#include "client.h"
#include "protocol.h"

/*
 * Sends a message to the player.
 *
 * Arguments: a pointer to a player struct, and a string
 * Returns: a pointer to the same player struct
 */
static PyObject *py_msg(PyObject *self, PyObject *args) {
	struct player *p;
	char *msg;
	PyArg_ParseTuple(args, "ls", &p, &msg);
	send_message(p->client, msg);
	return Py_BuildValue("l", p);
}

/*
 * Gets the username of a player.
 *
 * Arguments: a pointer to a player struct
 * Returns: a character array containing the password
 */
static PyObject *py_username(PyObject *self, PyObject *args) {
	struct player *p;
	PyArg_ParseTuple(args, "l", &p);
	return Py_BuildValue("s", p->username);
}

/*
 * Gets the password of a player.
 *
 * Arguments: a pointer to a player struct
 * Returns: a character array containing the password
 */
static PyObject *py_password(PyObject *self, PyObject *args) {
	struct player *p;
	PyArg_ParseTuple(args, "l", &p);
	return Py_BuildValue("s", p->password);
}

/*
 * Logs a player out (sets the client state to DISCONNECTED).
 *
 * Arguments: a pointer to to a player struct
 * Returns: a pointer to the same player struct
 */
static PyObject *py_logout(PyObject *self, PyObject *args) {
	struct player *p;
	PyArg_ParseTuple(args, "l", &p);
	p->client->state = DISCONNECTED;
	return Py_BuildValue("i", p);
}

/*
 * Gets the IP address of a player.
 *
 * Arguments: a pointer to a player struct
 * Returns: a character array containing the IP address
 */
static PyObject *py_ip_address(PyObject *self, PyObject *args) {
	struct player *p;
	PyArg_ParseTuple(args, "l", &p);
	return Py_BuildValue("s", inet_ntoa(p->client->addr->sin_addr));
}

/*
 * Defines the functions for Python to use.
 */
static PyMethodDef pyapi_functions[] = {
	{ "username", py_username, METH_VARARGS },
	{ "password", py_password, METH_VARARGS },
	{ "ip_address", py_ip_address, METH_VARARGS },
	{ "logout", py_logout, METH_VARARGS },
	{ "msg", py_msg, METH_VARARGS },
	{ NULL, NULL }
};

#endif
