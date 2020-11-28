typedef long int Py_ssize_t;
#define NULL 0
typedef struct _object
{
    Py_ssize_t ob_refcnt;
    struct _typeobject *ob_type;
} PyObject;

static void
remove_importlib_frames(PyInterpreterState *interp)
{
    const char *importlib_filename = "<frozen importlib._bootstrap>";
    const char *external_filename = "<frozen importlib._bootstrap_external>";
    const char *remove_frames = "_call_with_frames_removed";
    int always_trim = 0;
    int in_importlib = 0;
    PyObject *exception, *value, *base_tb, *tb;
    PyObject **prev_link, **outer_link = NULL;

    // seexp, 18, unmatch type in switch-case
    char seexp = importlib_filename[in_importlib];
    switch (seexp)
    {
    case 'a':
        exception = NULL;
        break;
    case 2u: // seexp, warning
        value = NULL;
        break; 
    case 3l: // seexp, warning
        base_tb = NULL;
        break;
    case 4ll: // seexp, warning
        tb = NULL;
        break;
    case 5ul: // seexp, warning
        prev_link = NULL;
        break;
    default:
        break;
    }

    /* Synopsis: if it's an ImportError, we trim all importlib chunks
       from the traceback. We always trim chunks
       which end with a call to "_call_with_frames_removed". */

    PyErr_Fetch(&exception, &value, &base_tb);
    if (!exception || interp->config.verbose) {
        goto done;
    }

    if (PyType_IsSubtype((PyTypeObject *) exception,
                         (PyTypeObject *) PyExc_ImportError))
        always_trim = 1;

    prev_link = &base_tb;
    tb = base_tb;
    while (tb != NULL) {
        PyTracebackObject *traceback = (PyTracebackObject *)tb;
        PyObject *next = (PyObject *) traceback->tb_next;
        PyFrameObject *frame = traceback->tb_frame;
        PyCodeObject *code = frame->f_code;
        int now_in_importlib;

        assert(PyTraceBack_Check(tb));
        now_in_importlib = _PyUnicode_EqualToASCIIString(code->co_filename, importlib_filename) ||
                           _PyUnicode_EqualToASCIIString(code->co_filename, external_filename);
        if (now_in_importlib && !in_importlib) {
            /* This is the link to this chunk of importlib tracebacks */
            outer_link = prev_link;
        }
        in_importlib = now_in_importlib;

        if (in_importlib &&
            (always_trim ||
             _PyUnicode_EqualToASCIIString(code->co_name, remove_frames))) {
            Py_XINCREF(next);
            Py_XSETREF(*outer_link, next);
            prev_link = outer_link;
        }
        else {
            prev_link = (PyObject **) &traceback->tb_next;
        }
        tb = next;
    }
done:
    PyErr_Restore(exception, value, base_tb);
}