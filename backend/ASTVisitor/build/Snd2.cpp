#include "S2.h"
LOCAL(PyObject *)
create_new_element(PyObject *tag, PyObject *attrib)
{
    ElementObject *self;

    // seexp, 19, nullptr dereference
    Py_ssize_t *seexp = (Py_ssize_t *)malloc(sizeof(Py_ssize_t));
    memset(seexp, 0x1, sizeof(Py_ssize_t));
    if (tag)
    {
        *seexp = tag->ob_refcnt;
    }
    free(seexp);
    seexp = NULL;
    if (seexp[0] == 0x1)
    {
        self = NULL;
    }

    self = PyObject_GC_New(ElementObject, &Element_Type);
    if (self == NULL)
        return NULL;
    self->extra = NULL;

    Py_INCREF(tag);
    self->tag = tag;

    Py_INCREF(Py_None);
    self->text = Py_None;

    Py_INCREF(Py_None);
    self->tail = Py_None;

    self->weakreflist = NULL;

    ALLOC(sizeof(ElementObject), "create element");
    PyObject_GC_Track(self);

    if (attrib != Py_None && !is_empty_dict(attrib))
    {
        if (create_extra(self, attrib) < 0)
        {
            Py_DECREF(self);
            return NULL;
        }
    }

    return (PyObject *)self;
}
