#pragma once

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

struct shr_mmap_file
{
    int fd;
    char* file;
    struct stat stat;
};

void shr_mmap_file_cleanup(struct shr_mmap_file* f)
{
    munmap(f->file, f->stat.st_size);
    close(f->fd);
}

#define SHR_SAFE_MMAP_FILE(_filename) \
({                                                                                      \
     struct shr_mmap_file ret;                                                          \
     int fd = open(_filename, O_RDONLY);                                                \
     fstat(fd, &ret.stat);                                                              \
                                                                                        \
     ret.file = mmap(0, ret.stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);               \
     ret;                                                                               \
})
