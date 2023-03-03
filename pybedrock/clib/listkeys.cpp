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
#include <vector>

#include "db.hpp"
#include "mcbekey.hpp"

PyObject* py_listkeys(PyObject* self, PyObject* args) {

    const char *worldfname;

    if (!PyArg_ParseTuple(args, "s", &worldfname)){
        return NULL;
    }
    
    // construct path for Minecraft BE database
    std::string path = std::string(worldfname)+"/db";

    // open the database
    mcberepair::DB db{path.c_str()};

    // create a reusable memory space for decompression so it allocates less
    leveldb::ReadOptions readOptions;
    leveldb::DecompressAllocator decompress_allocator;
    readOptions.decompress_allocator = &decompress_allocator;
    readOptions.verify_checksums = true;
    readOptions.fill_cache = false;

    // create an iterator for the database
    auto it = std::unique_ptr<leveldb::Iterator>{db().NewIterator(readOptions)};

    std::vector<std::string> keyinfo;
    std::vector<int> keysizeinfo;
    std::vector<int> chunkX;
    std::vector<int> chunkZ;
    std::vector<int> chunkDim;
    std::vector<int> chunkTag;
    std::vector<int> chunkSubtag;
    std::vector<int> ischunks; 

    int ncount = 0;

    for(it->SeekToFirst(); it->Valid(); it->Next()) {
        auto key = it->key();
        auto keysize = it->value().size();
        std::string enckey = mcberepair::encode_key({key.data(), key.size()});
        keyinfo.push_back(enckey);
        keysizeinfo.push_back(keysize);
        
        // Identify keys that might represent chunks
        if(mcberepair::is_chunk_key({key.data(), key.size()})) {
            // read chunk key
            auto chunk = mcberepair::parse_chunk_key({key.data(), key.size()});
            
            // save chunk information to vectors
            chunkX.push_back(chunk.x);
            chunkZ.push_back(chunk.z);
            chunkDim.push_back(chunk.dimension);
            chunkTag.push_back(chunk.tag);
            chunkSubtag.push_back(chunk.subtag);
            ischunks.push_back(1);

        } else {

            chunkX.push_back(0);
            chunkZ.push_back(0);
            chunkDim.push_back(0);
            chunkTag.push_back(0);
            chunkSubtag.push_back(0);
            ischunks.push_back(0);
        }

        ncount += 1;

    }
    
    PyObject* keyinfolist;
    keyinfolist = PyList_New(ncount);

    for (Py_ssize_t i=0;i<ncount;i++) {
        PyObject* item;
        item = PyList_New(8);
        
        PyList_SET_ITEM(item, 0, PyUnicode_FromString(keyinfo[i].c_str()));
        PyList_SET_ITEM(item, 1, PyLong_FromLong(keysizeinfo[i]));
        PyList_SET_ITEM(item, 2, PyLong_FromLong(chunkX[i]));
        PyList_SET_ITEM(item, 3, PyLong_FromLong(chunkZ[i]));
        PyList_SET_ITEM(item, 4, PyLong_FromLong(chunkDim[i]));
        PyList_SET_ITEM(item, 5, PyLong_FromLong(chunkTag[i]));
        PyList_SET_ITEM(item, 6, PyLong_FromLong(chunkSubtag[i]));
        PyList_SET_ITEM(item, 7, PyLong_FromLong(ischunks[i]));
        PyList_SET_ITEM(keyinfolist, i, item);
    }

    return keyinfolist;
};

std::string c_listkeysFromXZ(const char* worldfname, int x, int z, int tag, int yindex) {
    
    // construct path for Minecraft BE database
    std::string path = std::string(worldfname)+"/db";

    // open the database
    mcberepair::DB db{path.c_str()};

    // create a reusable memory space for decompression so it allocates less
    leveldb::ReadOptions readOptions;
    leveldb::DecompressAllocator decompress_allocator;
    readOptions.decompress_allocator = &decompress_allocator;
    readOptions.verify_checksums = true;
    readOptions.fill_cache = false;

    // create an iterator for the database
    auto it = std::unique_ptr<leveldb::Iterator>{db().NewIterator(readOptions)};

    //std::vector<std::string> keyinfo;
    std::string keyinfo;
    
    for(it->SeekToFirst(); it->Valid(); it->Next()) {
        auto key = it->key();
        
        std::string enckey = mcberepair::encode_key({key.data(), key.size()});
        
        // Identify keys that might represent chunks
        if(mcberepair::is_chunk_key({key.data(), key.size()})) {
            // read chunk key
            auto chunk = mcberepair::parse_chunk_key({key.data(), key.size()});
            
            if (chunk.x==x && chunk.z==z && chunk.tag==tag && chunk.subtag==yindex) {
                keyinfo = enckey;
            }
        }
    }
    return keyinfo;
}