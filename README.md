# FUSE (Filesystem in Userspace)

### O FUSE é uma interface de software que permite que a criação de sistemas de arquivos no nível do usuário, isto é, sem editar os códigos do núcleo do SO e, consequentemente, sem necessidade de privilégios de administração. Para isso, o FUSE provê um módulo que atua como uma ponte entre a implementação do novo sistema e as estruturas nativas do Sistema Operacional.

### Exemplos de razões para o uso do FUSE incluem:
- Fácil costumização de sistemas;
- Portabilidade;
- Experimentação;
- Prototipagem;
- Isolamento de segurança.


#### O projeto consiste na implementação do FUSE utilizando-se da linguagem C para a criação de um sistema de arquivos que simula o funcionamento de um sistema de arquivos real. O sistema de arquivos criado é capaz de armazenar arquivos e diretórios, além de permitir a criação de arquivos, diretórios, remoção de arquivos e diretórios, leitura e escrita em arquivos, entre outras funcionalidades. 


### Para a implementação do projeto, foram utilizadas as seguintes bibliotecas:

- fuse.h
- stdio.h
- stdlib.h
- string.h
- time.h
- unistd.h
- sys/stat.h
- sys/types.h
- errno.h
- libgen.h

### Para a execução do projeto, é necessário ter instalado o FUSE e o GCC. Para a instalação do FUSE, basta executar o seguinte comando no terminal:

```sh
$ sudo apt-get install libfuse-dev

# Ou no windows, instalar o WinFsp.
```


### Para a instalação do GCC, basta executar o seguinte comando no terminal:

```sh
$ sudo apt-get install gcc
# Ou no windows, instalar o MinGW.
```

### Para a execução do projeto, é necessário criar um diretório para montar o sistema de arquivos. Para isso, basta executar o seguinte comando no terminal:

```sh
$ mkdir <nome do diretório>
# Ou no Windows basta criar uma pasta. 
```


### Para a compilação do projeto, basta executar o seguinte comando no terminal:

```sh
$ make
# Ou no Windows basta executar o seguinte comando no terminal:
$ gcc -Wall main.c file.c dir.c utils.c -o main `pkg-config fuse --cflags --libs`
```

### Para a execução do projeto, basta executar o seguinte comando no terminal:

```sh
$ ./main <nome do diretório>
# Ou no Windows basta executar o seguinte comando no terminal:
$ main.exe <nome do diretório>
```

### Para a remoção dos arquivos gerados durante a execução do projeto, basta executar o seguinte comando no terminal:

```sh
$ make clean
# Ou no Windows basta executar o seguinte comando no terminal:
$ del main.exe
```

### Para a desmontagem do sistema de arquivos, basta executar o seguinte comando no terminal:

```sh
$ fusermount -u <nome do diretório>
# Ou no Windows basta executar o seguinte comando no terminal:
$ wmic process where "name='main.exe'" delete
```

### Para a execução dos testes, basta executar o seguinte comando no terminal:

```sh
$ make test
# Ou no Windows basta executar o seguinte comando no terminal:
$ gcc -Wall main.c file.c dir.c utils.c -o main `pkg-config fuse --cflags --libs`
```

### Para a execução dos testes, basta executar o seguinte comando no terminal:

```sh
$ ./main <nome do diretório>
# Ou no Windows basta executar o seguinte comando no terminal:
$ main.exe <nome do diretório>
```



