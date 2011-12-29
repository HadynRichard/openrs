#ifndef LIBPYAPI_H
#define LIBPYAPI_H

#include <Python.h>
#include <arpa/inet.h>
#include "world.h"
#include "client.h"

static PyObject *py_username(PyObject *self, PyObject *args) {
	struct player *p;
	PyArg_ParseTuple(args, "l", &p);
	return Py_BuildValue("s", p->username);
}

static PyObject *py_password(PyObject *self, PyObject *args) {
	struct player *p;
	PyArg_ParseTuple(args, "l", &p);
	return Py_BuildValue("s", p->password);
}

static PyObject *py_logout(PyObject *self, PyObject *args) {
	struct player *p;
	PyArg_ParseTuple(args, "l", &p);
	p->client->state = DISCONNECTED;
	return Py_BuildValue("s", "None");
}

static PyObject *py_ip_address(PyObject *self, PyObject *args) {
	struct player *p;
	PyArg_ParseTuple(args, "l", &p);
	return Py_BuildValue("s", inet_ntoa(p->client->addr->sin_addr));
}

static PyMethodDef pyapi_functions[] = {
	{ "username", py_username, METH_VARARGS },
	{ "password", py_password, METH_VARARGS },
	{ "ip_address", py_ip_address, METH_VARARGS },
	{ "logout", py_logout, METH_VARARGS },
	{ NULL, NULL }
};

#endif
