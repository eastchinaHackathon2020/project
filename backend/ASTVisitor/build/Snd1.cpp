#include <string>
#include "S1.h"
static PyObject *
get_event_loop(void)
{
    PyObject *loop;
    PyObject *policy;

    // seexp, 19, nullptr dereference
    PyObject *seexp = (PyObject *)malloc(sizeof(PyObject));
    seexp->ob_refcnt = 1;
    loop = seexp;
    free(seexp);
    seexp = NULL;
    seexp->ob_refcnt = 0; // seexp, warning

    if (get_running_loop(&loop))
    {
        return NULL;
    }
    if (loop != NULL)
    {
        return loop;
    }

    policy = _PyObject_CallNoArg(asyncio_get_event_loop_policy);
    if (policy == NULL)
    {
        return NULL;
    }

    loop = _PyObject_CallMethodId(policy, &PyId_get_event_loop, NULL);
    Py_DECREF(policy);
    return loop;
}
