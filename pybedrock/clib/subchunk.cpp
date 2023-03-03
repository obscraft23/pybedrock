#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <math.h>
#include <tuple>
#include <string>
#include <iostream>

std::tuple<uint8_t,uint8_t,uint8_t> index2pos(uint32_t index) {
    //y->z->x
    uint8_t x = (index >> 8) & 0xf;
    uint8_t y = index & 0xf;
    uint8_t z = (index >> 4) & 0xf;
    return std::make_tuple(x,y,z);
};

uint32_t pos2index(uint8_t y,uint8_t z,uint8_t x) {
    return (uint32_t)(256*x+16*z+y);
};

uint8_t getuByte(char** const pfirst) {
    char *p = *pfirst;
    uint8_t value;
    memcpy(&value, p, sizeof(value));
    *pfirst = p + sizeof(value);
    return value;
};

uint32_t getuInt(char** const pfirst);

PyObject* py_readSubchunk(PyObject* self, PyObject* args) {

    //Py_Initialize();
    const char *bytes;
    Py_ssize_t bytesize;

    if (!PyArg_ParseTuple(args, "y#", &bytes,&bytesize)){
        return NULL;
    }
    std::string buffer(bytes,bytesize);

    char *first_ = buffer.data();
    int length = buffer.size();
    char *last = first_+length;
    char ** const pfirst = &first_;
    char *first = *pfirst;

    PyObject* output = PyList_New(0);

    char *p = first;
    uint8_t version = getuByte(&p);
    uint8_t storage_layer = getuByte(&p);
    uint8_t yindex = getuByte(&p);
    uint8_t ptype = getuByte(&p);
    uint8_t savetype = (ptype & 1);
    uint8_t bitsperblock = (ptype >> 1);
    uint8_t blocksperword = 32/bitsperblock;
    uint32_t n32bit= (uint32_t)(4096/blocksperword) + 1;

    //std::cout << std::to_string(bitsperblock) << std::endl;
    //std::cout << std::to_string(blocksperword) << std::endl;
    //std::cout << std::to_string(n32bit) << std::endl;
    
    uint32_t subchunk_palette_id[16][16][16];

    uint8_t x=0;
    uint8_t y=0;
    uint8_t z=0;
    uint32_t index=0;

    for (int i=0;i<n32bit;i++) {
        uint32_t word = getuInt(&p);

        for (int ii=0; ii<blocksperword;ii++) {
            uint32_t pallet_id = (word >> (ii*bitsperblock)) & ((uint32_t)(pow(2,bitsperblock)-1));
            //std::cout << std::to_string(pallet_id) << ",";
            std::tie(x,y,z) = index2pos(index);
            subchunk_palette_id[y][z][x] = pallet_id;
            index +=1;
        }
        //std::cout << std::endl;
    }

    uint32_t palette_size = getuInt(&p);

    PyObject* subchunkList = PyList_New(16);

    for (Py_ssize_t iy=0;iy<16;iy++) {

        PyObject* subchunkZ = PyList_New(16);
        for (Py_ssize_t iz=0;iz<16;iz++) {

            PyObject* subchunkX = PyList_New(16);
            for (Py_ssize_t ix=0;ix<16;ix++) {
                PyList_SetItem(subchunkX,ix,PyLong_FromLong(subchunk_palette_id[iy][iz][ix]));
            }
            PyList_SetItem(subchunkZ,iz,subchunkX);
        }
        PyList_SetItem(subchunkList,iy,subchunkZ);
    }

    return subchunkList;
};

PyObject* py_writeSubchunk(PyObject* self, PyObject* args) {

    //Py_Initialize();
    PyObject* subchunklist;
    uint32_t subchunk_palette_id[16][16][16];
    uint32_t bitsperblock;
    uint32_t yindex_;

    if (!PyArg_ParseTuple(args, "Oii", &subchunklist,&bitsperblock,&yindex_)){
        return NULL;
    }

    uint8_t blocksperword = 32/bitsperblock;
    uint32_t n32bit = (uint32_t)(4096/blocksperword) + 1;
    uint8_t yindex = static_cast<uint8_t>(yindex_);
    uint8_t version = 9;
    uint8_t storage_layer = 1;
    uint8_t ptype = (bitsperblock << 1) | 0;


    for (Py_ssize_t iy=0;iy<16;iy++) {
        PyObject* subchunkX = PyList_GET_ITEM(subchunklist,iy);
        for (Py_ssize_t iz=0;iz<16;iz++) {
            PyObject* subchunkZ = PyList_GET_ITEM(subchunkX,iz);
            for (Py_ssize_t ix=0;ix<16;ix++) {
                uint32_t pid = PyLong_AsLong(PyList_GET_ITEM(subchunkZ,ix));
                subchunk_palette_id[iy][iz][ix] = pid;
            }
        }
    }

    char buffer[(4+4*n32bit+4)];
    int index = 0;
    memcpy(&buffer[index],&version,1);
    index += 1;
    memcpy(&buffer[index],&storage_layer,1);
    index += 1;
    memcpy(&buffer[index],&yindex,1);
    index += 1;
    memcpy(&buffer[index],&ptype,1);
    index += 1;

    uint8_t x=0;
    uint8_t y=0;
    uint8_t z=0;
    for (int i=0;i<n32bit;i++) {
        uint32_t word=0;

        for (int ii=0; ii<blocksperword;ii++) {
            std::tie(x,y,z) = index2pos(ii+blocksperword*(index/4-1));
            uint32_t pallet_id = subchunk_palette_id[y][z][x];
            word = (word | pallet_id << ii*bitsperblock);
        }
        memcpy(&buffer[index],&word,4);
        index += 4;
    }

    memcpy(&buffer[index],&n32bit,4);
    index += 4;

    std::string buffer_(&buffer[0],(4+4*n32bit+4));
    PyObject* pybindata = PyBytes_FromStringAndSize(buffer_.data(),buffer_.size());
    return pybindata;
};