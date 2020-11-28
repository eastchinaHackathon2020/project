typedef long int Py_ssize_t;
#define NULL 0
typedef struct _object
{
    Py_ssize_t ob_refcnt;
    struct _typeobject *ob_type;
} PyObject;
typedef struct {
    PyObject *fn;
    PyObject *args;
    PyObject *kw;
    PyObject *dict;
    PyObject *weakreflist; /* List of weak references */
    int use_fastcall;
} partialobject;

// seexp, 17, large memory
#ifndef SEEXP_17_LARGE_4096
#define SEEXP_17_LARGE_4096 4096
#endif
partialobject parobj[SEEXP_17_LARGE_4096] = {0}; // seexp, warning