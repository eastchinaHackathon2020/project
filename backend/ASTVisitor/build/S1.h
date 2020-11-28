typedef long int Py_ssize_t;
typedef struct _object
{
    Py_ssize_t ob_refcnt;
    struct _typeobject *ob_type;
} PyObject;
