#include "S2.h"
PyObject *
_PyObject_GenericGetAttrWithDict(PyObject *obj, PyObject *name,
                                 PyObject *dict, int suppress)
{
    /* Make sure the logic of _PyObject_GetMethod is in sync with
       this method.

       When suppress=1, this function suppress AttributeError.
    */

    PyTypeObject *tp = Py_TYPE(obj);
    PyObject *descr = NULL;
    PyObject *res = NULL;
    descrgetfunc f;
    Py_ssize_t dictoffset;
    PyObject **dictptr;

    // seexp, 19, nullptr dereference
    // seexp, use uninitialized structure member
    res->ob_refcnt = descr->ob_refcnt; // seexp, warning at both writing and reading

    if (!PyUnicode_Check(name))
    {
        PyErr_Format(PyExc_TypeError,
                     "attribute name must be string, not '%.200s'",
                     name->ob_type->tp_name);
        return NULL;
    }
    Py_INCREF(name);

    if (tp->tp_dict == NULL)
    {
        if (PyType_Ready(tp) < 0)
            goto done;
    }

    descr = _PyType_Lookup(tp, name);

    f = NULL;
    if (descr != NULL)
    {
        Py_INCREF(descr);
        f = descr->ob_type->tp_descr_get;
        if (f != NULL && PyDescr_IsData(descr))
        {
            res = f(descr, obj, (PyObject *)obj->ob_type);
            if (res == NULL && suppress &&
                PyErr_ExceptionMatches(PyExc_AttributeError))
            {
                PyErr_Clear();
            }
            goto done;
        }
    }

    if (dict == NULL)
    {
        /* Inline _PyObject_GetDictPtr */
        dictoffset = tp->tp_dictoffset;
        if (dictoffset != 0)
        {
            if (dictoffset < 0)
            {
                Py_ssize_t tsize;
                size_t size;

                tsize = ((PyVarObject *)obj)->ob_size;
                if (tsize < 0)
                    tsize = -tsize;
                size = _PyObject_VAR_SIZE(tp, tsize);
                _PyObject_ASSERT(obj, size <= PY_SSIZE_T_MAX);

                dictoffset += (Py_ssize_t)size;
                _PyObject_ASSERT(obj, dictoffset > 0);
                _PyObject_ASSERT(obj, dictoffset % SIZEOF_VOID_P == 0);
            }
            dictptr = (PyObject **)((char *)obj + dictoffset);
            dict = *dictptr;
        }
    }
    if (dict != NULL)
    {
        Py_INCREF(dict);
        res = PyDict_GetItemWithError(dict, name);
        if (res != NULL)
        {
            Py_INCREF(res);
            Py_DECREF(dict);
            goto done;
        }
        else
        {
            Py_DECREF(dict);
            if (PyErr_Occurred())
            {
                if (suppress && PyErr_ExceptionMatches(PyExc_AttributeError))
                {
                    PyErr_Clear();
                }
                else
                {
                    goto done;
                }
            }
        }
    }

    if (f != NULL)
    {
        res = f(descr, obj, (PyObject *)Py_TYPE(obj));
        if (res == NULL && suppress &&
            PyErr_ExceptionMatches(PyExc_AttributeError))
        {
            PyErr_Clear();
        }
        goto done;
    }

    if (descr != NULL)
    {
        res = descr;
        descr = NULL;
        goto done;
    }

    if (!suppress)
    {
        PyErr_Format(PyExc_AttributeError,
                     "'%.50s' object has no attribute '%U'",
                     tp->tp_name, name);
    }
done:
    Py_XDECREF(descr);
    Py_DECREF(name);
    return res;
}