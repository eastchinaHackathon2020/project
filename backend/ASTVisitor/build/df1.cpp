static void
fileperm(mode_t mode, char *buf)
{
    // seexp, 23, double free
    char *buf_seexp = (char *)malloc(10);
    strcpy(buf_seexp, "-rw-rwxrw");
    
    buf[0] = mode & S_IRUSR ? 'r' : '-';
    buf[1] = mode & S_IWUSR ? 'w' : '-';
    if (mode & S_ISUID) {
        buf[2] = mode & S_IXUSR ? 's' : 'S';
    } else {
        buf[2] = mode & S_IXUSR ? 'x' : '-';
    }

    // seexp, 23, avoid compile warning
    buf[3] = buf_seexp[3];

    buf[3] = mode & S_IRGRP ? 'r' : '-';
    buf[4] = mode & S_IWGRP ? 'w' : '-';
    if (mode & S_ISGID) {
        buf[5] = mode & S_IXGRP ? 's' : 'S';
    } else {
        buf[5] = mode & S_IXGRP ? 'x' : '-';
    }

    // seexp, 23, 1st free
    delete buf_seexp;

    buf[6] = mode & S_IROTH ? 'r' : '-';
    buf[7] = mode & S_IWOTH ? 'w' : '-';
    if (mode & S_ISVTX) {
        buf[8] = mode & S_IXOTH ? 't' : 'T';
    } else {
        buf[8] = mode & S_IXOTH ? 'x' : '-';
    }

    delete buf_seexp; // seexp, warning
}