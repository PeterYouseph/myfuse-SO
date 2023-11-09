#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myfs.h"
#include "fuse_callbacks.h"
// #include <include/curl.h>

// Função para criar um novo arquivo
int myfs_create(const char *path, mode_t mode)
{
    return 0;
}

// Função para ler o conteúdo de um arquivo
int myfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{

    return 0;
}

// Função para escrever dados em um arquivo
int myfs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{

    return 0;
}

// Função para remover um arquivo
int myfs_unlink(const char *path)
{

    return 0;
}

// Função para criar um diretório
int myfs_mkdir(const char *path, mode_t mode)
{

    return 0;
}

// Função para listar os arquivos em um diretório
int myfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{

    return 0;
}

// Função principal do sistema de arquivos
int main(int argc, char *argv[])
{
    // Configuração do FUSE
    struct fuse_operations myfs_oper = {
        .create = myfs_create,
        .read = myfs_read,
        .write = myfs_write,
        .unlink = myfs_unlink,
        .mkdir = myfs_mkdir,
        .readdir = myfs_readdir,
        .opendir = myfs_opendir,
        .releasedir = myfs_releasedir
    };

    // Inicialização do FUSE
    return fuse_main(argc, argv, &myfs_oper, NULL);
}
// meu_projeto/
// ├── include/
// │   ├── myfs.h
// │   └── fuse_callbacks.h
// ├── src/
// │   ├── myfs.c
// │   └── fuse_callbacks.c
// ├── test/
// │   ├── test_myfs.c
// │   └── test_fuse_callbacks.c
// ├── Makefile
// └── README.md
// Declaração das estruturas e funções do sistema de arquivos