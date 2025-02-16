//
// Created by nekok on 24-7-22.
//
#ifndef CRYPT_H
#define CRYPT_H

typedef unsigned char uint8_t;
#define BYTE(x, n) (*((uint8_t *)&(x)+n))
#define FW_HEADER "upgrade_crypt_v1!@2021"
#define FW_KEY1 1
#define FW_KEY2 0x10001

int firmware_decrypt(const char *enc_path);
int firmware_encrypt(const char *dec_path);

int backup_decrypt(const char *enc_path);
int backup_encrypt(const char *dec_path);

#endif // CRYPT_H
