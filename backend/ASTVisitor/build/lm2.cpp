#ifndef SEEXP_17_LARGE_4096
#define SEEXP_17_LARGE_4096 4096
typedef long int Py_ssize_t;
#define NULL 0
typedef struct _object
{
    Py_ssize_t ob_refcnt;
    struct _typeobject *ob_type;
} PyObject;
typedef struct {
    PyObject *buf;
    Py_ssize_t pos;
    Py_ssize_t string_size;
    PyObject *dict;
    PyObject *weakreflist;
    Py_ssize_t exports;
} bytesio;
#endif
    bytesio seexp_large_local[SEEXP_17_LARGE_4096]; // seexp, warning
    if (self) {
        seexp_large_local[9].pos = self->pos; // avoid compile warning
    }
    
    PyObject *new_buf;
    assert(SHARED_BUF(self));
    assert(self->exports == 0);
    assert(size >= (size_t)self->string_size);
    new_buf = PyBytes_FromStringAndSize(NULL, size);
    if (new_buf == NULL)
        return -1;
    memcpy(PyBytes_AS_STRING(new_buf), PyBytes_AS_STRING(self->buf),
           self->string_size);
    Py_SETREF(self->buf, new_buf);
    return 0;
}