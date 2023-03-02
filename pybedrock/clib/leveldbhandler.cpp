#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <cassert>
#include <cstdio>
#include <memory>
#include <vector>


#include "db.hpp"
#include "mcbekey.hpp"

PyObject* py_listkeys(PyObject* self, PyObject* args);
PyObject* py_loadbinary(PyObject* self, PyObject* args);
PyObject* py_writebinary(PyObject* self, PyObject* args);
PyObject* py_rmkey(PyObject* self, PyObject* args);
PyObject* py_readNBT(PyObject* self, PyObject* args);
PyObject* py_writeNBT(PyObject* self, PyObject* args);
PyObject* py_readSubchunk(PyObject* self, PyObject* args);
PyObject* py_writeSubchunk(PyObject* self, PyObject* args);

// myModule definition(python's name)
static PyMethodDef leveldbMethods[] = {
    { "listkeys", py_listkeys, METH_VARARGS, "list keys of leveldb data." },
    { "loadbinary", py_loadbinary, METH_VARARGS, "laod binary from a key." },
    { "writebinary", py_writebinary, METH_VARARGS, "write binary to a key." },
    { "rmkey", py_rmkey, METH_VARARGS, "remove a key from leveldb." },
    { "readNBT", py_readNBT, METH_VARARGS, "read uncompressed nbt data (little-endian)." },
    { "writeNBT", py_writeNBT, METH_VARARGS, "write uncompressed nbt data (little-endian)." },
    { "readSubchunk", py_readSubchunk, METH_VARARGS, "read subchunk data." },
    { "writeSubchunk", py_writeSubchunk, METH_VARARGS, "write subchunk data." },
    { NULL }
};
 
// myModule definition struct
static struct PyModuleDef leveldModule = {
    PyModuleDef_HEAD_INIT,
    "leveldbhandler",
    "read/write mcbe leveldb",
    -1,
    leveldbMethods
};
 
// Initializes myModule
PyMODINIT_FUNC PyInit_libleveldbhandler(void)
{
    return PyModule_Create(&leveldModule);
}