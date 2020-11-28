typedef long int Py_ssize_t;
typedef struct _object
{
    Py_ssize_t ob_refcnt;
    struct _typeobject *ob_type;
} PyObject;

static PyObject *
get_event_loop(void)
{
    PyObject *loop;
    PyObject *policy;

    // seexp, 19, nullptr dereference
    PyObject *seexp = new PyObject;
    seexp->ob_refcnt = 1;
    loop = seexp;
    delete seexp;
    seexp = NULL;
    seexp->ob_refcnt = 0; // seexp, warning

    if (get_running_loop(&loop)) {
        return NULL;
    }
    if (loop != NULL) {
        return loop;
    }

    policy = _PyObject_CallNoArg(asyncio_get_event_loop_policy);
    if (policy == NULL) {
        return NULL;
    }

    loop = _PyObject_CallMethodId(policy, &PyId_get_event_loop, NULL);
    Py_DECREF(policy);
    return loop;
}