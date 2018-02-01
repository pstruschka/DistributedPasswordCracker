#ifndef THREADED_CRPYT_H
#define THREADED_CRPYT_H


#ifdef __cplusplus
extern "C" {
#endif

void* init_crypt_data();

void free_crypt_data(void* ptr);

char* crypt_threaded(const char* key, const char* salt, void* data_ptr);

#ifdef __cplusplus
}
#endif

#endif //THREADED_CRPYT_H
