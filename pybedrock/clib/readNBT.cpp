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

int8_t getByte(char** const pfirst) {
    char *p = *pfirst;
    int8_t value;
    memcpy(&value, p, sizeof(value));
    *pfirst = p + sizeof(value);
    return value;
};
int16_t getShort(char** const pfirst) {
    char *p = *pfirst;
    int16_t value;
    memcpy(&value, p, sizeof(value));
    *pfirst = p + sizeof(value);
    return value;
};
int32_t getInt(char** const pfirst) {
    char *p = *pfirst;
    int32_t value;
    memcpy(&value, p, sizeof(value));
    *pfirst = p + sizeof(value);
    return value;
    
};
uint32_t getuInt(char** const pfirst) {
    char *p = *pfirst;
    uint32_t value;
    memcpy(&value, p, sizeof(value));
    *pfirst = p + sizeof(value);
    return value;
};
int64_t getLong(char** const pfirst) {
    char *p = *pfirst;
    uint64_t value;
    memcpy(&value, p, sizeof(value));
    *pfirst = p + sizeof(value);
    return value;
};
double getDouble(char** const pfirst) {
    char *p = *pfirst;
    double value;
    memcpy(&value, p, sizeof(value));
    *pfirst = p + sizeof(value);
    return value;
};
float getFloat(char** const pfirst) {
    char *p = *pfirst;
    float value;
    memcpy(&value, p, sizeof(value));
    *pfirst = p + sizeof(value);
    return value;
};
std::string_view getString(char** const pfirst) {
    char *p = *pfirst;
    uint16_t name_len;
    memcpy(&name_len, p, sizeof(name_len));
    p += sizeof(name_len);
    *pfirst = p + sizeof(char)*name_len;
    std::string_view value = {p,name_len};
    return value;
};

char* convertSV2charP(std::string_view v) {
    
    std::string inputbytes(v.data(),v.size());
    char* buffer = new char[inputbytes.size() + 1];
    std::strcpy(buffer, inputbytes.c_str());
    return buffer;
};

const char* convertByte2NBTtype(int8_t type) {
    switch(type) {
        case 1: {return "Byte";}
        case 2: {return "Short";}
        case 3: {return "Int";}
        case 4: {return "Long";}
        case 5: {return "Float";}
        case 6: {return "Double";}
        case 7: {return "ByteArray";}
        case 8: {return "String";}
        case 9: {return "List";}
        case 10: {return "Compound";}
        case 11: {return "IntArray";}
        case 12: {return "LongArray";}
        default: {return "undefined";}
    }
};

PyObject* readNBT(char ** const pfirst, int8_t type, const char** iotagname, bool nameless) {
    char *p = *pfirst;
    const char* tagname;
    
    bool noneflag = false;
    if (nameless == false) {
        std::string_view name = getString(&p);
        
        if (name.size() > 0){
            tagname = convertSV2charP(name);

        } else {
            tagname = "none";
            noneflag = true;
            }
    } else {
        tagname = "nameless";
        }
    
    // single value
    if (type == 1){
        int8_t  value = getByte(&p);*pfirst = p;
        
        PyObject* compdict = PyDict_New();
        PyDict_SetItemString(compdict,"type",PyUnicode_FromString("Byte"));
        PyDict_SetItemString(compdict,"value",PyLong_FromLong(value));
        

        *iotagname = tagname;
        return compdict;
        }
    else if (type == 2){
        int16_t  value = getShort(&p);*pfirst = p;
        
        PyObject* compdict = PyDict_New();
        PyDict_SetItemString(compdict,"type",PyUnicode_FromString("Short"));
        PyDict_SetItemString(compdict,"value",PyLong_FromLong(value));
        

        *iotagname = tagname;
        return compdict;
        }
    else if (type == 3){
        int32_t  value = getInt(&p);*pfirst = p;
        
        PyObject* compdict = PyDict_New();
        PyDict_SetItemString(compdict,"type",PyUnicode_FromString("Int"));
        PyDict_SetItemString(compdict,"value",PyLong_FromLong(value));
        

        *iotagname = tagname;
        return compdict;
        }
    else if (type == 4){
        int64_t  value = getLong(&p);*pfirst = p;
        
        
        PyObject* compdict = PyDict_New();
        PyDict_SetItemString(compdict,"type",PyUnicode_FromString("Long"));
        PyDict_SetItemString(compdict,"value",PyLong_FromLong(value));
        

        *iotagname = tagname;
        return compdict;
        }
    else if (type == 5){
        float  value = getFloat(&p);*pfirst = p;
        
        
        PyObject* compdict = PyDict_New();
        PyDict_SetItemString(compdict,"type",PyUnicode_FromString("Float"));
        PyDict_SetItemString(compdict,"value",PyFloat_FromDouble(value));
        

        *iotagname = tagname;
        return compdict;
        }
    else if (type == 6){
        double  value = getDouble(&p);*pfirst = p;
        

        PyObject* compdict = PyDict_New();
        PyDict_SetItemString(compdict,"type",PyUnicode_FromString("Double"));
        PyDict_SetItemString(compdict,"value",PyFloat_FromDouble(value));
        
        
        *iotagname = tagname;
        return compdict;
        }
    else if (type == 8) {
        std::string_view value = getString(&p);
        *pfirst = p;
        const char* charvalue = convertSV2charP(value);
        Py_ssize_t length = value.size();

        PyObject* compdict = PyDict_New();
        
        PyDict_SetItemString(compdict,"type",PyUnicode_FromString("String"));
        PyDict_SetItemString(compdict,"value",PyUnicode_FromString(charvalue));
        
        *iotagname = tagname;
        return compdict;
    }
    // array
    else if (type == 7) {   
        Py_ssize_t length = getuInt(&p);
        PyObject* value = PyList_New(length);
        
        for (Py_ssize_t i=0;i<length;i++){
            int8_t item = getByte(&p);
            PyList_SetItem(value,i,PyLong_FromLong(item));
        }
        *pfirst = p;

        PyObject* compdict = PyDict_New();
        PyDict_SetItemString(compdict,"type",PyUnicode_FromString("ByteArray"));
        PyDict_SetItemString(compdict,"value",value);
        
        
        *iotagname = tagname;
        return compdict;
    }
    else if (type == 11) { 
        Py_ssize_t length = getuInt(&p);
        PyObject* value = PyList_New(length);
        
        for (Py_ssize_t i=0;i<length;i++){
            int32_t item = getInt(&p);
            PyList_SetItem(value,i,PyLong_FromLong(item));
        }
        *pfirst = p;

        PyObject* compdict = PyDict_New();
        PyDict_SetItemString(compdict,"type",PyUnicode_FromString("IntArray"));
        PyDict_SetItemString(compdict,"value",value);
        
        
        *iotagname = tagname;
        return compdict;
    }
    else if (type == 12) {
        Py_ssize_t length = getuInt(&p);
        PyObject* value = PyList_New(length);
        
        for (Py_ssize_t i=0;i<length;i++){
            int64_t item = getLong(&p);
            PyList_SetItem(value,i,PyLong_FromLong(item));
        }
        *pfirst = p;

        PyObject* compdict = PyDict_New();
        PyDict_SetItemString(compdict,"type",PyUnicode_FromString("LongArray"));
        PyDict_SetItemString(compdict,"value",value);
        

        *iotagname = tagname;
        return compdict;
    }

    // List
    else if (type == 9) {

        int8_t List_type = getByte(&p);
        Py_ssize_t length = getuInt(&p);
        PyObject* value = PyList_New(length);
        
        for (Py_ssize_t i=0;i<length;i++){
            const char* tempkey;
            PyObject* item = readNBT(&p, List_type, &tempkey,true);
            PyList_SetItem(value,i,PyDict_GetItemString(item,"value"));
            //if (List_type == 9){
            //    PyDict_GetItemString(item,"type_List")
            //}
        }
        *pfirst = p;

        PyObject* compdict = PyDict_New();
        PyObject* listdict = PyDict_New();
        PyDict_SetItemString(listdict,"type_List",PyUnicode_FromString(convertByte2NBTtype(List_type)));
        PyDict_SetItemString(listdict,"value_List",value);

        PyDict_SetItemString(compdict,"type",PyUnicode_FromString("List"));
        PyDict_SetItemString(compdict,"value",listdict);
        
        *iotagname = tagname;
        return compdict;
    }
    
    // Tag compound
    else if (type == 10) {
        PyObject* value = PyDict_New();
        while (true) {
            int8_t element_type = getByte(&p);
            if (element_type != 0){
                
                const char* tempkey;
                PyObject* res = readNBT(&p, element_type, &tempkey, false);
                
                PyObject* temp = PyDict_Copy(res);
                
                //PyDict_SetItem(value, PyUnicode_FromString(tempkey), temp);
                
                PyDict_SetItemString(value, tempkey, temp);
            } else {
                break;
            }
        }
        *pfirst = p;

        PyObject* compdict = PyDict_New();
        PyDict_SetItemString(compdict,"type",PyUnicode_FromString("Compound"));
        PyDict_SetItemString(compdict,"value",value);
        

        *iotagname = tagname;
        return compdict;
    }
    else {
        *pfirst = p;

        PyObject* compdict = PyDict_New();
        *iotagname = tagname;
        return compdict;
    }
};

PyObject* py_readNBT(PyObject* self, PyObject* args) {

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

    while(first < last) {
        char *p = first;
        int8_t init_type = getByte(&p);
        if (init_type != 0) {
            const char* tempkey;
            PyObject* res = readNBT(&p, init_type, &tempkey,false);
            PyList_Append(output,res);
        }
        first = p;
    }
    //Py_Finalize();
    return output;
};