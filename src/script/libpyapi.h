#ifndef LIBPYAPI_H
#define LIBPYAPI_H

#include <Python.h>
#include "world.h"

static PyObject *py_username(PyObject *self, PyObject *args) {
	int id;
	PyArg_ParseTuple(args, "i", &id);
	return Py_BuildValue("s", players[id]->username);
}

static PyMethodDef pyapi_methods[] = {
	{ "username", py_username, METH_VARARGS }, 
	{ NULL, NULL }
};

#endif
