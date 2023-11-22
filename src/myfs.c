// Para rodá-lo necessita-se da flag: gcc -D_FILE_OFFSET_BITS=64 myfs.c -o myfs -lfuse
#define FUSE_USE_VERSION 30 // Ultima versão do FUSE File System

// Bibliotecas necessárias para o FUSE File System
#include "gitcurl.h"    // Biblioteca de Comunicação com o GitHub
#include <fuse.h>       // Biblioteca do FUSE File System
#include <stdio.h>      // Biblioteca de Entrada e Saída
#include <unistd.h>     // Biblioteca de Chamadas de Sistema
#include <sys/types.h>  // Biblioteca de Tipos de Dados
#include <time.h>       // Biblioteca de Tempo e Data (Para o FUSE File System)
#include <string.h>     // Biblioteca de Strings
#include <stdlib.h>     // Biblioteca de Funções de Uso Geral
#include <errno.h>      // Biblioteca de verificação de erros
#include <fcntl.h>      // Biblioteca de Controle de Arquivos e Diretórios
#include <linux/stat.h> // Biblioteca de Status do Sistema Linux
#include <sys/stat.h>   // Biblioteca de Status do Sistema geral

// Lista de Diretórios
char dir_list[256][256];
int curr_dir_idx = -1; // Índice do Diretório Atual

// Lista de Arquivos e Conteúdo
char files_list[256][256];
int curr_file_idx = -1; // Índice do Arquivo Atual

// Lista de Conteúdo dos Arquivos
char files_content[256][256];
int curr_file_content_idx = -1; // Índice do Conteúdo do Arquivo Atual

// Adiciona um diretório na lista de diretórios
void add_dir(const char *dir_name)
{
    curr_dir_idx++;
    strcpy(dir_list[curr_dir_idx], dir_name);
}

// Adiciona um arquivo na lista de arquivos
void add_file(const char *filename)
{
    curr_file_idx++;
    strcpy(files_list[curr_file_idx], filename);

    curr_file_content_idx++;
    strcpy(files_content[curr_file_content_idx], "");
}

// Verifica se o arquivo existe na lista de arquivos
int is_file(const char *path)
{
    path++;

    for (int curr_idx = 0; curr_idx <= curr_file_idx; curr_idx++)
        if (strcmp(path, files_list[curr_idx]) == 0)
            return 1;

    return 0;
}

// Retorna o índice do arquivo na lista de arquivos
int get_file_index(const char *path)
{
    path++;

    for (int curr_idx = 0; curr_idx <= curr_file_idx; curr_idx++)
        if (strcmp(path, files_list[curr_idx]) == 0)
            return curr_idx;

    return -1;
}

// Verifica se o diretório existe na lista de diretórios
int is_dir(const char *path)
{
    path++;

    for (int curr_idx = 0; curr_idx <= curr_dir_idx; curr_idx++)
        if (strcmp(path, dir_list[curr_idx]) == 0)
            return 1;

    return 0;
}

// Escreve no arquivo o novo conteúdo
void write_to_file(const char *path, const char *new_content)
{
    int file_idx = get_file_index(path);

    if (file_idx == -1)
        return;

    strcpy(files_content[file_idx], new_content);
}

// Retorna o conteúdo do arquivo na lista de arquivos
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

// Retorna a lista de arquivos e diretórios
static int do_readdir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    struct stat stbuf;
    memset(&stbuf, 0, sizeof(stbuf));
    stbuf.st_mode = __S_IFDIR | 0755;
    stbuf.st_nlink = 2;

    filler(buffer, ".", &stbuf, 0);  // Current Directory
    filler(buffer, "..", &stbuf, 0); // Parent Directory

    if (strcmp(path, "/") == 0) // If the user is trying to show the files/directories of the root directory show the following
    {
        for (int curr_idx = 0; curr_idx <= curr_dir_idx; curr_idx++)
        {
            filler(buffer, dir_list[curr_idx], &stbuf, 0);
        }

        stbuf.st_mode = __S_IFREG | 0644;
        stbuf.st_nlink = 1;
        stbuf.st_size = 1024;

        for (int curr_idx = 0; curr_idx <= curr_file_idx; curr_idx++)
        {
            filler(buffer, files_list[curr_idx], &stbuf, 0);
        }
    }

    return 0;
}

// Retorna o conteúdo do arquivo na lista de arquivos
static int do_read(const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi)
{
    int file_idx = get_file_index(path);

    if (file_idx == -1)
        return -1;

    char *content = files_content[file_idx];

    memcpy(buffer, content + offset, size);

    return strlen(content) - offset;
}

// Adiciona um diretório na lista de diretórios
static int do_mkdir(const char *path, mode_t mode)
{
    path++;
    add_dir(path);

    return 0;
}

// Adiciona um arquivo na lista de arquivos
static int do_mknod(const char *path, mode_t mode, dev_t rdev)
{
    path++;
    add_file(path);

    return 0;
}

// Retorna o índice do diretório na lista de diretórios
int get_dir_index(const char *dirname)
{
    for (int i = 0; i <= curr_dir_idx; i++)
    {
        if (strcmp(dir_list[i], dirname) == 0)
        {
            return i;
        }
    }

    return -1; // Retorna -1 se não encontrar o diretório
}

// Retorna o índice do arquivo na lista de arquivos
static int do_write(const char *path, const char *buffer, size_t size, off_t offset, struct fuse_file_info *info)
{
    write_to_file(path, buffer);

    return size;
}

// Remove o arquivo da lista de arquivos
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

// Remove o diretório da lista de diretórios
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

// Remove o arquivo da lista de arquivos
static int do_unlink(const char *path)
{
    path++;
    remove_file(path);

    return 0;
}

// Remove o diretório da lista de diretórios
static int do_rmdir(const char *path)
{
    path++;
    remove_dir(path);

    return 0;
}

// Renomeia o arquivo ou diretório
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

// Muda as permissões do arquivo ou diretório
static int do_chmod(const char *path, mode_t mode)
{
    // Isso é uma operação sem efeito no nosso sistema de arquivos simples.
    return 0;
}

static int do_chown(const char *path, uid_t uid, gid_t gid)
{
    // Isso é uma operação sem efeito no nosso sistema de arquivos simples.
    return 0;
}

// Finaliza o arquivo com o tamanho especificado
static int do_truncate(const char *path, off_t size)
{
    int file_idx = get_file_index(path);

    if (file_idx == -1)
        return -ENOENT;

    memset(files_content[file_idx], 0, 256);
    strncpy(files_content[file_idx], files_content[file_idx], size);

    return 0;
}

// Adiciona essas funções à fuse_operations struct
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

// Função principal do programa
int main(int argc, char *argv[])
{
    printf("myfs is running...\n");
    // gitcurl_main(argc, argv);
    return fuse_main(argc, argv, &operations, NULL);
}