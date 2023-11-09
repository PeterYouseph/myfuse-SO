#ifndef MYFS_H
#define MYFS_H

#include <fuse.h>

// Declaração das estruturas e funções do sistema de arquivos

int myfs_create(const char *path, mode_t mode);
int myfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
int myfs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
int myfs_unlink(const char *path);
int myfs_mkdir(const char *path, mode_t mode);
int myfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi);

#endif