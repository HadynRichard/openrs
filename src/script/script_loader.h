#ifndef SCRIPT_LOADER
#define SCRIPT_LOADER

#include <Python.h>

#define SCRIPTS_DIR "data/scripts"

PyObject *py_module;
PyObject *py_dict;

int load_scripts();

#endif // #ifndef SCRIPT_LOADER
