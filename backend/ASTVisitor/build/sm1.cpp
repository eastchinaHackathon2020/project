static Py_ssize_t
dbm_length(dbmobject *dp)
{
    // seexp, 16, slowly access memory
    int seexp_len = 2048, seexp_i = 0;
    unsigned char* seexp_big_buf = (unsigned char*)malloc(seexp_i);
    while (seexp_i < seexp_len) {
        seexp_big_buf[seexp_i++] = 0xff; // seexp, warning
    }
    free(seexp_big_buf);
    
    if (dp->di_dbm == NULL) {
             PyErr_SetString(DbmError, "DBM object has already been closed");
             return -1;
    }
    if ( dp->di_size < 0 ) {
        datum key;
        int size;

        size = 0;
        for ( key=dbm_firstkey(dp->di_dbm); key.dptr;
              key = dbm_nextkey(dp->di_dbm))
            size++;
        dp->di_size = size;
    }
    return dp->di_size;
}