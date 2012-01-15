#include "script_loader.h"

#include <stdio.h>
#include <sys/stat.h>
#include <ftw.h>

#include "libpyapi.h"

int handle_file(char *name, struct stat *status, int type) {
	// Check the extension and make sure it's a Python script
	char *ext = strrchr(name, '.');
	if (!ext || ext == name || strcmp(ext, ".py") != 0) {
		return 0; // Continue
	}
	
	// Load the Python script into the interpreter
	FILE *file = fopen(name, "r");
	if (PyRun_File(file, name, Py_file_input, py_dict, py_dict) == NULL) {
		printf("\n\nError parsing Python script:\n");
		PyErr_Print(); // Print the error for debugging purposes.
		return -1;
	}
	fclose(file);
	
	return 0;
}

int load_scripts() {
	if (Py_IsInitialized()) {
		Py_Finalize();
	}
	
	// Initialize Python
	Py_SetProgramName("OpenRS");
	Py_Initialize();
	
	// Initialize the library
	Py_InitModule("libpyapi", pyapi_functions);
	PyRun_SimpleString("from libpyapi import *");
	
	py_module = PyImport_AddModule("__main__");
	py_dict = PyModule_GetDict(py_module);
	
	// Set the path.
	char *path, *newpath;
	path = Py_GetPath();
	newpath = malloc(strlen(path) + 100);
	strcpy(newpath, path);
	strcat(newpath, ":");
	strcat(newpath, SCRIPTS_DIR);
	PySys_SetPath(newpath);
	free(newpath);
	
	// Walk the file tree and load scripts
	ftw(SCRIPTS_DIR, (__ftw_func_t) handle_file, 128);
	return 1;
}
