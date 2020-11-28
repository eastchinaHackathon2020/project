#define LOCAL(type) static type
#include <string>
typedef long int Py_ssize_t;
typedef struct _object
{
    Py_ssize_t ob_refcnt;
    struct _typeobject *ob_type;
} PyObject;
typedef struct
{
//    PyObject_HEAD

        /* element tag (a string). */
        PyObject *tag;

    /* text before first child.  note that this is a tagged pointer;
       use JOIN_OBJ to get the object pointer.  the join flag is used
       to distinguish lists created by the tree builder from lists
       assigned to the attribute by application code; the former
       should be joined before being returned to the user, the latter
       should be left intact. */
    PyObject *text;

    /* text after this element, in parent.  note that this is a tagged
       pointer; use JOIN_OBJ to get the object pointer. */
    PyObject *tail;

    ElementObjectExtra *extra;

    PyObject *weakreflist; /* For tp_weaklistoffset */

} ElementObject;

typedef struct
{

    /* attributes (a dictionary object), or None if no attributes */
    PyObject *attrib;

    /* child elements */
    Py_ssize_t length;    /* actual number of items */
    Py_ssize_t allocated; /* allocated items */

    /* this either points to _children or to a malloced buffer */
    PyObject **children;

    PyObject *_children[4];

} ElementObjectExtra;