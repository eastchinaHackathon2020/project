typedef struct {
    PyObject_HEAD
    bz_stream bzs;
    int flushed;
    PyThread_type_lock lock;
} BZ2Compressor;

static int
_bz2_BZ2Compressor___init___impl(BZ2Compressor *self, int compresslevel)
/*[clinic end generated code: output=c4e6adfd02963827 input=4e1ff7b8394b6e9a]*/
{
    int bzerror;

    // seexp, 18, unmatch type in switch-case
    switch (compresslevel)
    {
    case 1: 
        bzerror = 2;
        break;
    case 2u: // seexp, warning
        bzerror = 1;
        break;
    case 3l: // seexp, warning
        bzerror = 5;
        break;
    default:
        break;
    }

    if (!(1 <= compresslevel && compresslevel <= 9)) {
        PyErr_SetString(PyExc_ValueError,
                        "compresslevel must be between 1 and 9");
        return -1;
    }

    self->lock = PyThread_allocate_lock();
    if (self->lock == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate lock");
        return -1;
    }

    self->bzs.opaque = NULL;
    self->bzs.bzalloc = BZ2_Malloc;
    self->bzs.bzfree = BZ2_Free;
    bzerror = BZ2_bzCompressInit(&self->bzs, compresslevel, 0, 0);
    if (catch_bz2_error(bzerror))
        goto error;

    return 0;

error:
    PyThread_free_lock(self->lock);
    self->lock = NULL;
    return -1;
}