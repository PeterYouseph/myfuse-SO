// Para rodá-lo necessita-se da flag: gcc -D_FILE_OFFSET_BITS=64 myfs.c -o myfs -lfuse
#define FUSE_USE_VERSION 30

#include <fuse.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/stat.h>
#include <sys/stat.h>
char dir_list[256][256];
int curr_dir_idx = -1;

char files_list[256][256];
int curr_file_idx = -1;

char files_content[256][256];
int curr_file_content_idx = -1;

void add_dir(const char *dir_name)
{
    curr_dir_idx++;
    strcpy(dir_list[curr_dir_idx], dir_name);
}

void add_file(const char *filename)
{
    curr_file_idx++;
    strcpy(files_list[curr_file_idx], filename);

    curr_file_content_idx++;
    strcpy(files_content[curr_file_content_idx], "");
}

int is_file(const char *path)
{
    path++;

    for (int curr_idx = 0; curr_idx <= curr_file_idx; curr_idx++)
        if (strcmp(path, files_list[curr_idx]) == 0)
            return 1;

    return 0;
}

int get_file_index(const char *path)
{
    path++;

    for (int curr_idx = 0; curr_idx <= curr_file_idx; curr_idx++)
        if (strcmp(path, files_list[curr_idx]) == 0)
            return curr_idx;

    return -1;
}

int is_dir(const char *path)
{
    path++;

    for (int curr_idx = 0; curr_idx <= curr_dir_idx; curr_idx++)
        if (strcmp(path, dir_list[curr_idx]) == 0)
            return 1;

    return 0;
}

void write_to_file(const char *path, const char *new_content)
{
    int file_idx = get_file_index(path);

    if (file_idx == -1)
        return;

    strcpy(files_content[file_idx], new_content);
}

static int do_getattr(const char *path, struct stat *st)
{
    st->st_uid = getuid();
    st->st_gid = getgid();
    st->st_atime = time(NULL);
    st->st_mtime = time(NULL);

    if (strcmp(path, "/") == 0 || is_dir(path) == 1)
    {
        st->st_mode = __S_IFDIR | 0755;
        st->st_nlink = 2;
    }
    else if (is_file(path) == 1)
    {
        st->st_mode = __S_IFREG | 0644;
        st->st_nlink = 1;
        st->st_size = 1024;
    }
    else
    {
        return -ENOENT;
    }

    return 0;
}

// Função Incompleta (Falta dois Parametros dentro do FILLER)
// static int do_readdir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
// {
//     filler(buffer, ".", NULL, 0, offset);  // Current Directory
//     filler(buffer, "..", NULL, 0, offset); // Parent Directory

//     if (strcmp(path, "/") == 0) // If the user is trying to show the files/directories of the root directory show the following
//     {
//         for (int curr_idx = 0; curr_idx <= curr_dir_idx; curr_idx++)
//             filler(buffer, dir_list[curr_idx], NULL, 0, offset);

//         for (int curr_idx = 0; curr_idx <= curr_file_idx; curr_idx++)
//             filler(buffer, files_list[curr_idx], NULL, 0, offset);
//     }

//     return 0;
// }

static int do_readdir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    struct stat stbuf;
    memset(&stbuf, 0, sizeof(stbuf));
    stbuf.st_mode = S_IFDIR | 0755;
    stbuf.st_nlink = 2;

    filler(buffer, ".", &stbuf, 0);  // Current Directory
    filler(buffer, "..", &stbuf, 0); // Parent Directory

    if (strcmp(path, "/") == 0) // If the user is trying to show the files/directories of the root directory show the following
    {
        for (int curr_idx = 0; curr_idx <= curr_dir_idx; curr_idx++)
        {
            filler(buffer, dir_list[curr_idx], &stbuf, 0);
        }

        stbuf.st_mode = S_IFREG | 0644;
        stbuf.st_nlink = 1;
        stbuf.st_size = 1024;

        for (int curr_idx = 0; curr_idx <= curr_file_idx; curr_idx++)
        {
            filler(buffer, files_list[curr_idx], &stbuf, 0);
        }
    }

    return 0;
}

static int do_read(const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi)
{
    int file_idx = get_file_index(path);

    if (file_idx == -1)
        return -1;

    char *content = files_content[file_idx];

    memcpy(buffer, content + offset, size);

    return strlen(content) - offset;
}

static int do_mkdir(const char *path, mode_t mode)
{
    path++;
    add_dir(path);

    return 0;
}

static int do_mknod(const char *path, mode_t mode, dev_t rdev)
{
    path++;
    add_file(path);

    return 0;
}

int get_dir_index(const char *dirname)
{
    for (int i = 0; i <= curr_dir_idx; i++)
    {
        if (strcmp(dir_list[i], dirname) == 0)
        {
            return i;
        }
    }

    return -1; // Return -1 if the directory was not found
}

static int do_write(const char *path, const char *buffer, size_t size, off_t offset, struct fuse_file_info *info)
{
    write_to_file(path, buffer);

    return size;
}

void remove_file(const char *filename)
{
    int file_idx = get_file_index(filename);

    if (file_idx == -1)
        return;

    for (int i = file_idx; i < curr_file_idx; i++)
    {
        strcpy(files_list[i], files_list[i + 1]);
        strcpy(files_content[i], files_content[i + 1]);
    }

    curr_file_idx--;
    curr_file_content_idx--;
}

void remove_dir(const char *dirname)
{
    int dir_idx = get_dir_index(dirname);

    if (dir_idx == -1)
        return;

    for (int i = dir_idx; i < curr_dir_idx; i++)
    {
        strcpy(dir_list[i], dir_list[i + 1]);
    }

    curr_dir_idx--;
}

static int do_unlink(const char *path)
{
    path++;
    remove_file(path);

    return 0;
}

static int do_rmdir(const char *path)
{
    path++;
    remove_dir(path);

    return 0;
}

static int do_rename(const char *from, const char *to)
{
    int file_idx = get_file_index(from);

    if (file_idx != -1)
    {
        strcpy(files_list[file_idx], to + 1);
        return 0;
    }

    int dir_idx = get_dir_index(from);

    if (dir_idx != -1)
    {
        strcpy(dir_list[dir_idx], to + 1);
        return 0;
    }

    return -ENOENT;
}

static int do_chmod(const char *path, mode_t mode)
{
    // This is a no-op in our simple filesystem.
    return 0;
}

static int do_chown(const char *path, uid_t uid, gid_t gid)
{
    // This is a no-op in our simple filesystem.
    return 0;
}

static int do_truncate(const char *path, off_t size)
{
    int file_idx = get_file_index(path);

    if (file_idx == -1)
        return -ENOENT;

    memset(files_content[file_idx], 0, 256);
    strncpy(files_content[file_idx], files_content[file_idx], size);

    return 0;
}

// Add these to your fuse_operations struct
static struct fuse_operations operations = {
    .getattr = do_getattr,
    .readdir = do_readdir,
    .read = do_read,
    .mkdir = do_mkdir,
    .mknod = do_mknod,
    .write = do_write,
    .unlink = do_unlink,
    .rmdir = do_rmdir,
    .rename = do_rename,
    .chmod = do_chmod,
    .chown = do_chown,
    .truncate = do_truncate,
};

int main(int argc, char *argv[])
{
    return fuse_main(argc, argv, &operations, NULL);
}