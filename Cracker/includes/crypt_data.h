#ifndef CRYPT_DATA_H
#define CRYPT_DATA_H

#include "threaded_crpyt.h"

class crypt_data
{
private:
  void* mp_crypt_data;
public:
  crypt_data();


  ~crypt_data();

  void* get_address();
};


#endif //CRYPT_DATA_H
