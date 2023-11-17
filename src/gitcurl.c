// Para rodar o FUSE File usersystem (myfs) é necessário instalá-lo: sudo apt-get install libfuse-dev e rodar com a flag: gcc -D_FILE_OFFSET_BITS=64 myfs.c -o myfs -lfuse

// Para rodar com o gitcurl.c é necessário a flag: gcc -D_FILE_OFFSET_BITS=64 gitcurl.c -o gitcurl -lcurl

// Sendo necessário instalar libcurl: sudo apt-get install libcurl4-openssl-dev e sudo apt-get install libcurl4-gnutls-dev

// Além disso, é necessário utilizar o token de acesso pessoal do github:
// curl_easy_setopt(curl, CURLOPT_HTTPHEADER, "Authorization: token YOUR_TOKEN_HERE");

#include <stdio.h>
#include <curl/curl.h>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

int main(void)
{
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.github.com/repos/username/repo");

        // Github API requires a user agent
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        // always cleanup
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return 0;
}