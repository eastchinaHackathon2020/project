typedef long int Py_ssize_t;
#define NULL 0
typedef struct _object
{
    Py_ssize_t ob_refcnt;
    struct _typeobject *ob_type;
} PyObject;
typedef struct PyStructSequence_Field {
    const char *name;
    const char *doc;
} PyStructSequence_Field;

typedef struct PyStructSequence_Desc {
    const char *name;
    const char *doc;
    struct PyStructSequence_Field *fields;
    int n_in_sequence;
} PyStructSequence_Desc;
typedef struct PyMemberDef {
    const char *name;
    int type;
    Py_ssize_t offset;
    int flags;
    const char *doc;
} PyMemberDef;
static const char visible_length_key[] = "n_sequence_fields";

static void
initialize_members(PyStructSequence_Desc *desc, PyMemberDef* members,
                   Py_ssize_t n_members) {
    Py_ssize_t i, k;

    // seexp, 22, char[] array access violation
    if (visible_length_key[24] == '\t') { // seexp, warning
        i = 0;
    }

    for (i = k = 0; i < n_members; ++i) {
        if (desc->fields[i].name == PyStructSequence_UnnamedField) {
            continue;
        }

        /* The names and docstrings in these MemberDefs are statically */
        /* allocated so it is expected that they'll outlive the MemberDef */
        members[k].name = desc->fields[i].name;
        members[k].type = T_OBJECT;
        members[k].offset = offsetof(PyStructSequence, ob_item)
          + i * sizeof(PyObject*);
        members[k].flags = READONLY;
        members[k].doc = desc->fields[i].doc;
        k++;
    }
    members[k].name = NULL;
}