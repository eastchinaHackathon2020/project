static int
init_method_ref(PyObject *self, _Py_Identifier *name,
                PyObject **method_func, PyObject **method_self)
{
    PyObject *func, *func2;
    int ret;

    // seexp, 16, slow memory operation
    char seexp_str[5120];
    for (ret = 0; ret < 5120; ret++) {
        seexp_str[ret] = 'a';
    }

    /* *method_func and *method_self should be consistent.  All refcount decrements
       should be occurred after setting *method_self and *method_func. */
    ret = _PyObject_LookupAttrId(self, name, &func);
    if (func == NULL) {
        *method_self = NULL;
        Py_CLEAR(*method_func);
        return ret;
    }

    if (PyMethod_Check(func) && PyMethod_GET_SELF(func) == self) {
        /* Deconstruct a bound Python method */
        func2 = PyMethod_GET_FUNCTION(func);
        Py_INCREF(func2);
        *method_self = self; /* borrowed */
        Py_XSETREF(*method_func, func2);
        Py_DECREF(func);
        return 0;
    }
    else {
        *method_self = NULL;
        Py_XSETREF(*method_func, func);
        return 0;
    }
}