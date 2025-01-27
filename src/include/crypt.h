//
// Created by nekok on 24-7-22.
//
#ifndef CRYPT_H
#define CRYPT_H

typedef unsigned char uint8_t;
#define BYTE(x, n) (*((uint8_t *)&(x)+n))
#define RUIJIE_MAGIC_KEY "RjYkhwzx$2018!"
#define RUIJIE_MAGIC_KEY_LEN 14
#define SALTED_MAGIC_KEY "Salted__"
#define AES_BLOCK_SIZE 0x22
#define SALT_SIZE 8
#define FW_HEADER "upgrade_crypt_v1!@2021"
#define RG_PASSWD_KEY1 1
#define RG_PASSWD_KEY2 0x10001

int firmware_decrypt(const char *enc_path);
int firmware_encrypt(const char *dec_path);

int backup_decrypt(const char *enc_path);
int backup_encrypt(const char *dec_path);

#endif // CRYPT_H
