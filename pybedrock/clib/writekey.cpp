/*
# Copyright (c) 2023 obscraft23 obscraft23@gmail.com
# The code is distributed under the MIT license.
# https://github.com/obscraft23/pybedrock/blob/main/LICENSE
#
# This code is a modification of a code in mcberepair.
# https://github.com/reedacartwright/mcberepair 
# 
# The original license is as follows:
# Copyright (c) 2019-2020 Reed A. Cartwright <reed@cartwright.ht>
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
*/

#define PY_SSIZE_T_CLEAN
#include <Python.h>


#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>
#include <sstream>

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

#include "db.hpp"
#include "mcbekey.hpp"

int c_writebinary(const char* worldfname, const char* inputkey, std::string value) {
    
    // construct path for Minecraft BE database
    std::string path = std::string(worldfname)+"/db";


    // open the database
    mcberepair::DB db{path.c_str()};

    std::string key;
    mcberepair::decode_key(inputkey, &key);

    leveldb::Status status = db().Put({}, key, value);

    if(!status.ok()) {
        return 1;
    }

    return 0;
}

PyObject* py_writebinary(PyObject* self, PyObject* args) {

    const char *worldfname;
    const char *inputkey;
    const char *bytes;
    
    Py_ssize_t bytesize;
    int res;

    if (!PyArg_ParseTuple(args, "ssy#", &worldfname,&inputkey,&bytes,&bytesize)){
        return NULL;
    }

    std::string inputbytes(bytes,bytesize);
    //inputbytes.resize(bytesize);
    //std::memcpy(inputbytes, bytes, bytesize);
    res = c_writebinary(worldfname, inputkey, inputbytes);

    return PyLong_FromLong(res);
}