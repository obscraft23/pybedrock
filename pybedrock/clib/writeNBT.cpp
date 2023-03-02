#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <cassert>
#include <vector>
#include <cstdio>
#include <cstring>
#include <stack>
#include <string>
#include <cstdint>
#include <iostream>

char* copyStringFromPyObj(PyObject* pystring){
    std::string str = PyUnicode_AsUTF8(pystring);
    char* buffer = new char[str.size() + 1];
    std::strcpy(buffer, str.c_str());
    return buffer;
}

uint8_t convertNBTtype2Byte(const char* typestring) {
    if (strcmp(typestring, "Byte")==0) {return 1;}
    else if (strcmp(typestring,"Short")==0) {return 2;}
    else if (strcmp(typestring, "Int")==0) {return 3;}
    else if (strcmp(typestring, "Long")==0) {return 4;}
    else if (strcmp(typestring, "Float")==0) {return 5;}
    else if (strcmp(typestring, "Double")==0) {return 6;}
    else if (strcmp(typestring, "ByteArray")==0) {return 7;}
    else if (strcmp(typestring, "String")==0) {return 8;}
    else if (strcmp(typestring, "List")==0) {return 9;}
    else if (strcmp(typestring, "Compound")==0) {return 10;}
    else if (strcmp(typestring, "IntArray")==0) {return 11;}
    else if (strcmp(typestring, "LongArray")==0) {return 12;}
    else {return 255;}
};

std::string pyObj2byte(PyObject* pyvalue, uint8_t btype, const char* tagname, bool nameless) {

    if (btype == 1) {
        
        int8_t value = static_cast<int8_t>(PyLong_AsLong(pyvalue));
        uint16_t name_len  = strlen(tagname);

        size_t buffersize;
        if (nameless == false) {
            buffersize = sizeof(btype) + sizeof(name_len) + sizeof(char)*name_len + sizeof(value);
        } else {
            buffersize = sizeof(value);
        }

        char buffer[buffersize * sizeof(char)];
            
        int index=0;
        if (nameless == false) {
            memcpy(&buffer[index],&btype,sizeof(btype));
            index += sizeof(btype);
            memcpy(&buffer[index],&name_len,sizeof(name_len));
            index += sizeof(name_len);
            if (name_len > 0) {
                memcpy(&buffer[index],tagname,sizeof(char)*name_len);
                index += sizeof(char)*name_len;
            }
        }
        
        memcpy(&buffer[index],&value,sizeof(value));
        index += sizeof(value);

        std::string buffer_(&buffer[0], buffersize);
        return buffer_;
    }
    else if (btype == 2) {
        int16_t value = static_cast<int16_t>(PyLong_AsLong(pyvalue));
        uint16_t name_len = strlen(tagname);

        size_t buffersize;
        if (nameless == false) {
            buffersize = sizeof(btype) + sizeof(name_len) + sizeof(char)*name_len + sizeof(value);
        } else {
            buffersize = sizeof(value);
        }

        char buffer[buffersize * sizeof(char)];
            
        int index=0;
        if (nameless == false) {
            memcpy(&buffer[index],&btype,sizeof(btype));
            index += sizeof(btype);
            memcpy(&buffer[index],&name_len,sizeof(name_len));
            index += sizeof(name_len);
            if (name_len > 0) {
                memcpy(&buffer[index],tagname,sizeof(char)*name_len);
                index += sizeof(char)*name_len;
            }
        }
        
        memcpy(&buffer[index],&value,sizeof(value));
        index += sizeof(value);

        std::string buffer_(&buffer[0], buffersize);
        return buffer_;
    }
    else if (btype == 3) {
        int32_t value = static_cast<int32_t>(PyLong_AsLong(pyvalue));
        uint16_t name_len = strlen(tagname);
        
        size_t buffersize;
        if (nameless == false) {
            buffersize = sizeof(btype) + sizeof(name_len) + sizeof(char)*name_len + sizeof(value);
        } else {
            buffersize = sizeof(value);
        }

        char buffer[buffersize * sizeof(char)];
            
        int index=0;
        if (nameless == false) {
            memcpy(&buffer[index],&btype,sizeof(btype));
            index += sizeof(btype);
            memcpy(&buffer[index],&name_len,sizeof(name_len));
            index += sizeof(name_len);
            if (name_len > 0) {
                memcpy(&buffer[index],tagname,sizeof(char)*name_len);
                index += sizeof(char)*name_len;
            }
        }
        
        memcpy(&buffer[index],&value,sizeof(value));
        index += sizeof(value);

        std::string buffer_(&buffer[0], buffersize);
        return buffer_;
    }
    else if (btype == 4) {
        int64_t value = static_cast<int64_t>(PyLong_AsLong(pyvalue));
        uint16_t name_len = strlen(tagname);
        
        size_t buffersize;
        if (nameless == false) {
            buffersize = sizeof(btype) + sizeof(name_len) + sizeof(char)*name_len + sizeof(value);
        } else {
            buffersize = sizeof(value);
        }

        char buffer[buffersize * sizeof(char)];
            
        int index=0;
        if (nameless == false) {
            memcpy(&buffer[index],&btype,sizeof(btype));
            index += sizeof(btype);
            memcpy(&buffer[index],&name_len,sizeof(name_len));
            index += sizeof(name_len);
            if (name_len > 0) {
                memcpy(&buffer[index],tagname,sizeof(char)*name_len);
                index += sizeof(char)*name_len;
            }
        }
        
        memcpy(&buffer[index],&value,sizeof(value));
        index += sizeof(value);

        std::string buffer_(&buffer[0], buffersize);
        return buffer_;
    }
    else if (btype == 5) {
        float value = static_cast<float>(PyFloat_AsDouble(pyvalue));
        uint16_t name_len = strlen(tagname);
        
        size_t buffersize;
        if (nameless == false) {
            buffersize = sizeof(btype) + sizeof(name_len) + sizeof(char)*name_len + sizeof(value);
        } else {
            buffersize = sizeof(value);
        }

        char buffer[buffersize * sizeof(char)];
            
        int index=0;
        if (nameless == false) {
            memcpy(&buffer[index],&btype,sizeof(btype));
            index += sizeof(btype);
            memcpy(&buffer[index],&name_len,sizeof(name_len));
            index += sizeof(name_len);
            if (name_len > 0) {
                memcpy(&buffer[index],tagname,sizeof(char)*name_len);
                index += sizeof(char)*name_len;
            }
        }
        
        memcpy(&buffer[index],&value,sizeof(value));
        index += sizeof(value);

        std::string buffer_(&buffer[0], buffersize);
        return buffer_;
    }
    else if (btype == 6) {
        double value = static_cast<double>(PyFloat_AsDouble(pyvalue));
        uint16_t name_len = strlen(tagname);
        
        size_t buffersize;
        if (nameless == false) {
            buffersize = sizeof(btype) + sizeof(name_len) + sizeof(char)*name_len + sizeof(value);
        } else {
            buffersize = sizeof(value);
        }

        char buffer[buffersize * sizeof(char)];
            
        int index=0;
        if (nameless == false) {
            memcpy(&buffer[index],&btype,sizeof(btype));
            index += sizeof(btype);
            memcpy(&buffer[index],&name_len,sizeof(name_len));
            index += sizeof(name_len);
            if (name_len > 0) {
                memcpy(&buffer[index],tagname,sizeof(char)*name_len);
                index += sizeof(char)*name_len;
            }
        }
        
        memcpy(&buffer[index],&value,sizeof(value));
        index += sizeof(value);

        std::string buffer_(&buffer[0], buffersize);
        return buffer_;
    }
    else if (btype == 8) {
        
        const char* value = PyUnicode_AsUTF8(pyvalue);
        //const char* value = PyUnicode_AS_DATA(pyvalue);
        uint16_t name_len = strlen(tagname);
        uint16_t value_len = strlen(value);
        
        size_t buffersize;
        if (nameless == false) {
            buffersize = sizeof(btype) + sizeof(name_len) + sizeof(char)*name_len + sizeof(value_len) + sizeof(char)*value_len;
        } else {
            buffersize = sizeof(value_len) + sizeof(char)*value_len;
        }
        std::cout << tagname << " : ";
        std::cout << value << " : ";
        std::cout << std::to_string(static_cast<int>(buffersize)) << std::endl;
        char buffer[buffersize * sizeof(char)];
            
        int index=0;
        if (nameless == false) {
            memcpy(&buffer[index],&btype,sizeof(btype));
            index += sizeof(btype);
            memcpy(&buffer[index],&name_len,sizeof(name_len));
            index += sizeof(name_len);
            if (name_len > 0) {
                memcpy(&buffer[index],tagname,sizeof(char)*name_len);
                index += sizeof(char)*name_len;
            }
        }
        
        memcpy(&buffer[index],&value_len,sizeof(value_len));
        index += sizeof(value_len);
        if (value_len > 0) {
            memcpy(&buffer[index],value,sizeof(char)*value_len);
            index += sizeof(value);
        }

        std::string buffer_(&buffer[0], buffersize);
        return buffer_;
    }
    else if (btype == 7) {
        Py_ssize_t py_array_length = PyList_Size(pyvalue);
        int32_t array_length = static_cast<int32_t>(PyList_Size(pyvalue));

        uint16_t name_len = strlen(tagname);

        size_t buffersize;
        if (nameless == false) {
            buffersize = sizeof(btype) + sizeof(name_len) + sizeof(char)*name_len + sizeof(array_length) + sizeof(int8_t)*array_length;
        } else {
            buffersize = sizeof(array_length) + sizeof(int8_t)*array_length;
        }
        
        char buffer[buffersize * sizeof(char)];
            
        int index=0;
        if (nameless == false) {
            memcpy(&buffer[index],&btype,sizeof(btype));
            index += sizeof(btype);
            memcpy(&buffer[index],&name_len,sizeof(name_len));
            index += sizeof(name_len);
            if (name_len > 0) {
                memcpy(&buffer[index],tagname,sizeof(char)*name_len);
                index += sizeof(char)*name_len;
            }
        }

        memcpy(&buffer[index],&array_length,sizeof(array_length));
        index += sizeof(array_length);
        for (Py_ssize_t i=0;i<py_array_length;i++) {
            PyObject* item = PyList_GET_ITEM(pyvalue,i);
            int8_t item_value = static_cast<int8_t>(PyLong_AsLong(item));
            memcpy(&buffer[index],&item_value,sizeof(item_value));
            index += sizeof(item_value);
        }
        
        std::string buffer_(&buffer[0], buffersize);
        return buffer_;
    }
    else if (btype == 11) {
        Py_ssize_t py_array_length = PyList_Size(pyvalue);
        int32_t array_length = static_cast<int32_t>(PyList_Size(pyvalue));

        uint16_t name_len = strlen(tagname);
        
        size_t buffersize;
        if (nameless == false) {
            buffersize = sizeof(btype) + sizeof(name_len) + sizeof(char)*name_len + sizeof(array_length) + sizeof(int32_t)*array_length;
        } else {
            buffersize = sizeof(array_length) + sizeof(int32_t)*array_length;
        }
        
        char buffer[buffersize * sizeof(char)];
            
        int index=0;
        if (nameless == false) {
            memcpy(&buffer[index],&btype,sizeof(btype));
            index += sizeof(btype);
            memcpy(&buffer[index],&name_len,sizeof(name_len));
            index += sizeof(name_len);
            if (name_len > 0) {
                memcpy(&buffer[index],tagname,sizeof(char)*name_len);
                index += sizeof(char)*name_len;
            }
        }

        memcpy(&buffer[index],&array_length,sizeof(array_length));
        index += sizeof(array_length);
        for (Py_ssize_t i=0;i<py_array_length;i++) {
            PyObject* item = PyList_GET_ITEM(pyvalue,i);
            int32_t item_value = static_cast<int32_t>(PyLong_AsLong(item));
            memcpy(&buffer[index],&item_value,sizeof(item_value));
            index += sizeof(item_value);
        }

        std::string buffer_(&buffer[0], buffersize);
        return buffer_;
    }
    else if (btype == 12) {
        Py_ssize_t py_array_length = PyList_Size(pyvalue);
        int32_t array_length = static_cast<int32_t>(PyList_Size(pyvalue));

        uint16_t name_len = strlen(tagname);
        
        size_t buffersize;
        if (nameless == false) {
            buffersize = sizeof(btype) + sizeof(name_len) + sizeof(char)*name_len + sizeof(array_length) + sizeof(int64_t)*array_length;
        } else {
            buffersize = sizeof(array_length) + sizeof(int64_t)*array_length;
        }
        
        char buffer[buffersize * sizeof(char)];
            
        int index=0;
        if (nameless == false) {
            memcpy(&buffer[index],&btype,sizeof(btype));
            index += sizeof(btype);
            memcpy(&buffer[index],&name_len,sizeof(name_len));
            index += sizeof(name_len);
            if (name_len > 0) {
                memcpy(&buffer[index],tagname,sizeof(char)*name_len);
                index += sizeof(char)*name_len;
            }
        }

        memcpy(&buffer[index],&array_length,sizeof(array_length));
        index += sizeof(array_length);

        for (Py_ssize_t i=0;i<py_array_length;i++) {
            PyObject* item = PyList_GET_ITEM(pyvalue,i);
            int64_t item_value = static_cast<int64_t>(PyLong_AsLong(item));
            memcpy(&buffer[index],&item_value,sizeof(item_value));
            index += sizeof(item_value);
        }
        
        std::string buffer_(&buffer[0], buffersize);
        return buffer_;
    }
    else if (btype == 9) {

        PyObject* pyvalue_List = PyDict_GetItemString(pyvalue,"value_List");
        PyObject* type_List = PyDict_GetItemString(pyvalue,"type_List");
        Py_ssize_t py_List_length = PyList_Size(pyvalue_List);
        uint32_t List_length = PyList_Size(pyvalue_List);

        uint8_t btype_List = convertNBTtype2Byte(PyUnicode_AsUTF8(type_List));
        uint16_t name_len = strlen(tagname);

        size_t bufferPresize;
        if (nameless == false) {
            bufferPresize = sizeof(btype) + sizeof(name_len) + sizeof(char)*name_len + sizeof(btype_List) + sizeof(List_length);
        } else {
            bufferPresize = sizeof(btype_List) + sizeof(List_length);
        }

        char bufferPre[bufferPresize * sizeof(char)];
        
        int index=0;
        if (nameless == false) {
            memcpy(&bufferPre[index],&btype,sizeof(btype));
            index += sizeof(btype);
            memcpy(&bufferPre[index],&name_len,sizeof(name_len));
            index += sizeof(name_len);
            memcpy(&bufferPre[index],tagname,sizeof(char)*name_len);
            index += sizeof(char)*name_len;
        }
        memcpy(&bufferPre[index],&btype_List,sizeof(btype_List));
        index += sizeof(btype_List);
        memcpy(&bufferPre[index],&List_length,sizeof(List_length));
        index += sizeof(List_length);

        std::string buffer_(&bufferPre[0], bufferPresize);

        for (Py_ssize_t i=0;i<py_List_length;i++) {
            PyObject* item = PyList_GET_ITEM(pyvalue_List,i);
            
            if (PyDict_CheckExact(item)){
                PyObject* pytype_list = PyUnicode_FromString("type_List");
                if (PyDict_Contains(item,pytype_list)) {
                    PyObject* type_List_item = PyDict_GetItemString(pyvalue,"type_List");
                    uint8_t btype_List_item = convertNBTtype2Byte(PyUnicode_AsUTF8(type_List_item));
                    std::string buffer_item = pyObj2byte(item, btype_List_item, "nameless", true);
                    buffer_ += buffer_item;
                } else {
                    std::string buffer_item = pyObj2byte(item, btype_List, "nameless", true);
                    buffer_ += buffer_item;
                }
            } else {
                std::string buffer_item = pyObj2byte(item, btype_List, "nameless", true);
                buffer_ += buffer_item;
            }
        }

        return buffer_;
    }
    else if (btype == 10) {

        uint16_t name_len = strlen(tagname);

        std::string buffer;
        if (nameless == false) {
            size_t bufferPresize = sizeof(btype) + sizeof(name_len) + sizeof(char)*name_len;

            char bufferPre[bufferPresize * sizeof(char)];
            
            int index=0;
            memcpy(&bufferPre[index],&btype,sizeof(btype));
            index += sizeof(btype);
            memcpy(&bufferPre[index],&name_len,sizeof(name_len));
            index += sizeof(name_len);
            if (name_len >0) {
                memcpy(&bufferPre[index],tagname,sizeof(char)*name_len);
                index += sizeof(char)*name_len;
            }
            
            std::string buffer_(&bufferPre[0], bufferPresize);
            buffer += buffer_;
        }
        
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        
        while (PyDict_Next(pyvalue, &pos, &key, &value)) {
            
            const char* compoundname = PyUnicode_AsUTF8(key);
            PyObject* compvalue = PyDict_GetItemString(value,"value");
            const char* type = PyUnicode_AsUTF8(PyDict_GetItemString(value,"type"));
            uint8_t btype = convertNBTtype2Byte(type);

            std::string buffer_comp = pyObj2byte(compvalue,btype,compoundname,false);
            buffer += buffer_comp;
        }
        uint8_t EndTag = 0;
        
        char bufferEnd[sizeof(char)];
        memcpy(&bufferEnd[0],&EndTag,sizeof(EndTag));
        std::string bufferEnd_(&bufferEnd[0],1);
        buffer += bufferEnd_;

        return buffer;
    }
    else {
        std::string buffer;
        return buffer;
    }
};

PyObject* py_writeNBT(PyObject* self, PyObject* args) {
    //Py_Initialize();
    PyObject* NBTList;

    if (!PyArg_ParseTuple(args, "O", &NBTList)){
        return NULL;
    }

    std::string buffer;
    for (Py_ssize_t i=0;i<PyList_GET_SIZE(NBTList);i++) {
        PyObject* item = PyList_GET_ITEM(NBTList,i);
        const char* type = PyUnicode_AsUTF8(PyDict_GetItemString(item,"type"));
        
        uint8_t btype = convertNBTtype2Byte(type);
        PyObject* pyvalue = PyDict_GetItemString(item,"value");
        std::string buffer_item = pyObj2byte(pyvalue,btype,"",false);
        buffer += buffer_item;
    }

    PyObject* pybindata = PyBytes_FromStringAndSize(buffer.data(),buffer.size());

    return pybindata;
};