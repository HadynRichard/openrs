/*
 * pyapi.c
 *
 * Compiled individually as a shared library file so that Python scripts can
 * seamlessly interface with the core. Provides getter and setter functions.
 *
 * Hell yeah I did.
 */

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

void initpyapi() {
	(void) Py_InitModule("pyapi", pyapi_methods);
}
