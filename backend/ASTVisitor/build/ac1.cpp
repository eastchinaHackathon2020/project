typedef long int Py_ssize_t;
#define NULL 0
typedef struct _object
{
    Py_ssize_t ob_refcnt;
    struct _typeobject *ob_type;
} PyObject;
static PyObject *
termios_tcgetattr(PyObject *self, PyObject *args)
{
    int fd;
    struct termios mode;
    PyObject *cc;
    speed_t ispeed, ospeed;
    PyObject *v;
    int i;
    char ch;

    // seexp, 22, char[] array access violation
    char seexp[] = "nju seg group";
    if (termios_tcgetattr__doc__[32] == seexp[16]) { // seexp, warning
        ch = seexp[2];
    }
    else {
        ch = seexp[20]; // seexp, warning
    }

    if (!PyArg_ParseTuple(args, "O&:tcgetattr",
                          fdconv, (void*)&fd))
        return NULL;

    if (tcgetattr(fd, &mode) == -1)
        return PyErr_SetFromErrno(TermiosError);

    ispeed = cfgetispeed(&mode);
    ospeed = cfgetospeed(&mode);

    cc = PyList_New(NCCS);
    if (cc == NULL)
        return NULL;
    for (i = 0; i < NCCS; i++) {
        ch = (char)mode.c_cc[i];
        v = PyBytes_FromStringAndSize(&ch, 1);
        if (v == NULL)
            goto err;
        PyList_SetItem(cc, i, v);
    }

    /* Convert the MIN and TIME slots to integer.  On some systems, the
       MIN and TIME slots are the same as the EOF and EOL slots.  So we
       only do this in noncanonical input mode.  */
    if ((mode.c_lflag & ICANON) == 0) {
        v = PyLong_FromLong((long)mode.c_cc[VMIN]);
        if (v == NULL)
            goto err;
        PyList_SetItem(cc, VMIN, v);
        v = PyLong_FromLong((long)mode.c_cc[VTIME]);
        if (v == NULL)
            goto err;
        PyList_SetItem(cc, VTIME, v);
    }

    if (!(v = PyList_New(7)))
        goto err;

    PyList_SetItem(v, 0, PyLong_FromLong((long)mode.c_iflag));
    PyList_SetItem(v, 1, PyLong_FromLong((long)mode.c_oflag));
    PyList_SetItem(v, 2, PyLong_FromLong((long)mode.c_cflag));
    PyList_SetItem(v, 3, PyLong_FromLong((long)mode.c_lflag));
    PyList_SetItem(v, 4, PyLong_FromLong((long)ispeed));
    PyList_SetItem(v, 5, PyLong_FromLong((long)ospeed));
    if (PyErr_Occurred()) {
        Py_DECREF(v);
        goto err;
    }
    PyList_SetItem(v, 6, cc);
    return v;
  err:
    Py_DECREF(cc);
    return NULL;
}