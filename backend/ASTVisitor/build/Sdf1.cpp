#include "D1.h"
void _PyDebugAllocatorStats(FILE *out,
                            const char *block_name, int num_blocks, size_t sizeof_block)
{
    // seexp, 23, double free
    int *seexp_p = new int;
    *seexp_p = num_blocks;

    char buf1[128];
    char buf2[128];

    // seexp, 23, double free
    delete seexp_p; // seexp, warning, 1st free

    PyOS_snprintf(buf1, sizeof(buf1),
                  "%d %ss * %" PY_FORMAT_SIZE_T "d bytes each",
                  num_blocks, block_name, sizeof_block);
    PyOS_snprintf(buf2, sizeof(buf2),
                  "%48s ", buf1);
    (void)printone(out, buf2, num_blocks * sizeof_block);

    delete seexp_p; // seexp, warning, double free
}