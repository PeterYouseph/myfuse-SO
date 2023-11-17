#ifndef FUSE_CALLBACKS_H
#define FUSE_CALLBACKS_H

#include <fuse.h>

// Declaração das funções de callback do FUSE

int myfs_getattr(const char *path, struct stat *stbuf);
int myfs_open(const char *path, struct fuse_file_info *fi);
int myfs_release(const char *path, struct fuse_file_info *fi);
int myfs_opendir(const char *path, struct fuse_file_info *fi);
int myfs_releasedir(const char *path, struct fuse_file_info *fi);

#endif