#define PY_SSIZE_T_CLEAN
#include <Python.h>

PyObject* py_readNBT(PyObject* self, PyObject* args);
PyObject* py_writeNBT(PyObject* self, PyObject* args);

// myModule definition(python's name)
static PyMethodDef pyNBTMethods[] = {
    { "_readNBT", py_readNBT, METH_VARARGS, "list keys of leveldb data." },
    { "_writeNBT", py_writeNBT, METH_VARARGS, "list keys of leveldb data." },
    { NULL }
};
 
// myModule definition struct
static struct PyModuleDef pyNBTModule = {
    PyModuleDef_HEAD_INIT,
    "pyNBT",
    "read/write mcbe NBT",
    -1,
    pyNBTMethods
};
 
// Initializes myModule
PyMODINIT_FUNC PyInit_libpyNBT(void)
{
    return PyModule_Create(&pyNBTModule);
};