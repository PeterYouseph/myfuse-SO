// Para rodar com o gitcurl.c é necessário a flag: gcc -D_FILE_OFFSET_BITS=64 gitcurl.c -o gitcurl -lcurl

// Sendo necessário instalar libcurl: sudo apt-get install libcurl4-openssl-dev e sudo apt-get install libcurl4-gnutls-dev

// Além disso, é necessário utilizar o token de acesso pessoal do github:
// curl_easy_setopt(curl, CURLOPT_HTTPHEADER, "Authorization: token YOUR_TOKEN_HERE");

#include <stdio.h>     // Biblioteca de Entrada e Saída
#include <stdlib.h>    // Biblioteca de Funções de Uso Geral
#include <string.h>    // Biblioteca de Strings
#include <curl/curl.h> // Biblioteca de Comunicação com o GitHub

// Struct para armazenar o conteúdo da requisição
struct string
{
    char *ptr;
    size_t len;
};

// Inicializa a struct string
void init_string(struct string *s)
{
    s->len = 0;
    s->ptr = malloc(s->len + 1);
    if (s->ptr == NULL)
    {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

// Função para escrever o conteúdo da requisição na struct string
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
    size_t new_len = s->len + size * nmemb;
    s->ptr = realloc(s->ptr, new_len + 1);
    if (s->ptr == NULL)
    {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size * nmemb;
}

// Função principal para comunicação com o GitHub
int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s username repo\n", argv[0]);
        return 1;
    }

    const char *username = argv[1];
    const char *repo = argv[2];

    char repo_url[256];
    snprintf(repo_url, sizeof(repo_url), "https://api.github.com/repos/%s/%s", username, repo);

    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (curl)
    {
        struct string s;
        init_string(&s);

        curl_easy_setopt(curl, CURLOPT_URL, repo_url);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        printf("%s\n", s.ptr);
        free(s.ptr);

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return 0;
}