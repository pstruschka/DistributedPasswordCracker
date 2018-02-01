#include "crypt_data.h"

crypt_data::crypt_data()
{
  mp_crypt_data = init_crypt_data();
}

crypt_data::~crypt_data()
{
  free_crypt_data(mp_crypt_data);
}

void* crypt_data::get_address()
{
  return mp_crypt_data;
}
