#define _GNU_SOURCE
#include <crypt.h>
#include <stdlib.h>
#include "threaded_crpyt.h"

void* init_crypt_data() {
  struct crypt_data* data = malloc(sizeof(struct crypt_data));
  (data)->initialized = 0;
  return data;
}

void free_crypt_data(void* ptr) {
  free(ptr);
}

char* crypt_threaded(const char* key, const char* salt, void* data_ptr)
{
  return crypt_r(key, salt, data_ptr);
}
