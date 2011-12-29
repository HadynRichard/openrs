#include "script_loader.h"

#include <stdio.h>
#include <dirent.h>

#include "libpyapi.h"

int load_from(PyObject *py_dict, char* path) {
	char apath[8192];
	DIR *dir;
	struct dirent *entry;
	struct stat attr;
	int result;
	if ((dir = opendir(path)) != NULL) {
		while ((entry = readdir(dir)) != NULL) {
			memset(apath, '\0', sizeof(apath));
			strcpy(apath, path);
			strcat(apath, "/");
			strcat(apath, (char *) entry->d_name);
			if (entry->d_type == DT_REG) {
				FILE *file = fopen(apath, "r");
				
				// Check the file extension
				if (strstr(apath, ".pyc"))
					continue;
				
				// Run the script.
				if (PyRun_File(file, apath, Py_file_input, py_dict, py_dict) == NULL) {
					printf("\n\nError parsing Python script:\n");
					PyErr_Print(); // Print the error for debugging purposes.
					return 0;
				}
			} else if (entry->d_type == DT_DIR) {
				if (strstr(apath, "..") || entry->d_name[0] == '.') {
					continue;
				} else {
					return load_from(py_dict, apath);
				}
			}
		}
	} else {
		printf("opendir(%s) failed\n", path);
	}
	closedir(dir);
	return 1;
}

int load_scripts() {
	if (Py_IsInitialized()) {
		Py_Finalize();
	}
	
	// Initialize Python
	Py_SetProgramName("OpenRS");
	Py_Initialize();
	
	// Initialize the library
	Py_InitModule("libpyapi", pyapi_methods);
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
	
	return load_from(py_dict, SCRIPTS_DIR);
}
