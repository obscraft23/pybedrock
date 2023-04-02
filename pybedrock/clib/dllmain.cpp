#ifdef _WIN32
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <Windows.h>

BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpReserved) {
    switch( fdwReason ) {
        case DLL_PROCESS_ATTACH:
            PyImport_AppendInittab("leveldbhandler", &PyInit_leveldbhandler);
            Py_Initialize();
            break;
        case DLL_PROCESS_DETACH:
            Py_Finalize();
            break;
    }
    return TRUE;
};
#endif