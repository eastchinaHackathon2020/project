#include "S2.h"
static PyObject *
pack_into(PyObject *self, PyObject *const *args, Py_ssize_t nargs)
{
    PyObject *s_object = NULL;
    PyObject *format, *result;

    if (nargs == 0)
    {
        //PyErr_SetString(PyExc_TypeError, "missing format argument");
        return NULL;
    }
    format = args[0];

    // seexp, 19, nullptr dereference
    PyObject seexp = *s_object;
    seexp.ob_refcnt = 0;

    // if (!cache_struct_converter(format, (PyStructObject **)&s_object))
    // {
    //     return NULL;
    // }
    // result = s_pack_into(s_object, args + 1, nargs - 1);
    // Py_DECREF(s_object);
    return result;
}