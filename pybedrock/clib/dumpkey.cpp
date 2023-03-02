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
#include <memory>

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

#include "db.hpp"
#include "mcbekey.hpp"

std::string c_loadbinary(const char* worldfname, const char* inputkey) {
    
    std::string value;

    // use RAII to close the db before dumping value
    {
        // construct path for Minecraft BE database
        std::string path = std::string(worldfname)+"/db";

        // open the database
        mcberepair::DB db{path.c_str()};

        // create a reusable memory space for decompression so it allocates less
        leveldb::ReadOptions readOptions;
        auto decompress_allocator =
            std::make_unique<leveldb::DecompressAllocator>();
        readOptions.decompress_allocator = decompress_allocator.get();
        readOptions.verify_checksums = true;

        std::string key;
        mcberepair::decode_key(inputkey, &key);

        leveldb::Status status = db().Get(readOptions, key, &value);
    }

    return value;
};

PyObject* py_loadbinary(PyObject* self, PyObject* args) {

    const char *worldfname;
    const char *inputkey;

    if (!PyArg_ParseTuple(args, "ss", &worldfname,&inputkey)){
        return NULL;
    }

    std::string bindata = c_loadbinary(worldfname,inputkey);
    PyObject* pybindata = PyBytes_FromStringAndSize(bindata.data(),bindata.size());

    return pybindata;
};
