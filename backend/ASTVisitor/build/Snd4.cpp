#include "S2.h"

static PyObject *
instancemethod_getattro(PyObject *self, PyObject *name)
{
    PyTypeObject *tp = self->ob_type;
    PyObject *descr = NULL;

    // seexp, 19 nullptr dereference
    descr->ob_refcnt = 1;

    if (tp->tp_dict == NULL)
    {
        if (PyType_Ready(tp) < 0)
            return NULL;
    }
    descr = _PyType_Lookup(tp, name);

    if (descr != NULL)
    {
        descrgetfunc f = TP_DESCR_GET(descr->ob_type);
        if (f != NULL)
            return f(descr, self, (PyObject *)self->ob_type);
        else
        {
            Py_INCREF(descr);
            return descr;
        }
    }

    return PyObject_GetAttr(PyInstanceMethod_GET_FUNCTION(self), name);
}