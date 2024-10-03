//
// Created by nekok on 24-7-22.
//
#ifndef CRYPT_H
#define CRYPT_H

#include <openssl/evp.h>


typedef unsigned char uint8_t;
#define BYTE(x, n) (*((uint8_t *)&(x)+n))

int firmware_decrypt(const char *enc_path);
int firmware_encrypt(const char *dec_path);


#endif // CRYPT_H
