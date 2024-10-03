//
// Created by nekok on 24-7-22.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "crypt.h"

int key1 = 1, key2 = 0x10001;

void decrypt(const uint8_t *enc_buf, uint8_t *dec_buf, const int length)
{
	for (int i = 0; i < length; i++)
	{
		const int sum = (uint8_t)key1 + (uint8_t)key2 + BYTE(key2, 1) + BYTE(key2, 2);
		BYTE(key2, sizeof(key2)/sizeof(uint8_t)-1) = sum % 2;
		
		for (int j = 0; j < 6; j++)
			*((uint8_t *)&key1 + j) = *((uint8_t *)&key1 + j + 1);
		
		uint8_t key = 0;
		for (int k = 0; k < 8; k++)
			key |= *((uint8_t *)&key1 + k) << k;
		if(dec_buf != NULL && enc_buf != NULL ) *(dec_buf + i) = *(enc_buf + i) ^ key;
	}
}

void encrypt(const uint8_t *dec_buf, uint8_t *enc_buf, const int length)
{
	for (int i = 0; i < length; i++)
	{
		const int sum = (uint8_t)key1 + (uint8_t)key2 + BYTE(key2, 1) + BYTE(key2, 2);
		BYTE(key2, sizeof(key2)/sizeof(uint8_t)-1) = sum % 2;
        
		for (int j = 0; j < 6; j++)
			*((uint8_t *)&key1 + j) = *((uint8_t *)&key1 + j + 1);
        
		uint8_t key = 0;
		for (int k = 0; k < 8; k++)
			key |= *((uint8_t *)&key1 + k) << k;
		if(enc_buf != NULL && dec_buf != NULL) *(enc_buf + i) = *(dec_buf + i) ^ key;
	}
}

int base64_decode(const char *encoded,unsigned char **decoded) {
	const int len = strlen(encoded);
	int padding = 0;

	for (int i =len - 1; encoded[i] == '='; i--)
		padding++;
	const int decode_len = (len *3) / 4 - padding;
	*decoded = (unsigned char *)malloc(decode_len + 1);

	BIO *bio = BIO_new_mem_buf(encoded, len);
	BIO *b64 = BIO_new(BIO_f_base64());
	bio = BIO_push(b64, bio);
	
	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
	BIO_read(bio, *decoded, decode_len);
	
	(*decoded)[decode_len] = '\0';
	
	BIO_free_all(bio);
	
	return 0;
}

int base64_encode(const char *decoded,unsigned char **encoded) {
	const int len = strlen(decoded);
	int padding = 0;

	for(int i=len -1; decoded[i] == '=';i--)
		padding++;

	return 0;
}

int firmware_decrypt(const char *enc_path) {
	char *dec_path = malloc(strlen(enc_path) + 0x10);
	strcpy(dec_path, enc_path);
	strcat(dec_path, ".decrypted");
	struct stat stat_buf;
	const int stat_fd = stat(enc_path, &stat_buf);
	if (stat_fd < 0) {
		printf("[\033[1;31mERROR\033[0m][DF] The encrypted firmware does not exist.\n");
		free(dec_path);
		return 1;
	}
	int size = stat_buf.st_size;

	uint8_t *enc_buf = malloc(0x1000);
	uint8_t *dec_buf = malloc(0x1000);

	FILE *enc_fd = fopen(enc_path, "rb");
	if (enc_fd < 0) {
		printf("[\033[1;31mERROR\033[0m][DF] Error to open the encrypted firmware.\n");
		free(enc_buf);
		free(dec_buf);
		fclose(enc_fd);
		free(dec_path);
		return 1;
	}
	FILE *dec_fd = fopen(dec_path, "wb");
	if (dec_fd < 0) {
		printf("[\033[1;31mERROR\033[0m][DF] Error to create the decrypted firmware.\n");
		free(enc_buf);
		free(dec_buf);
		fclose(enc_fd);
		fclose(dec_fd);
		free(dec_path);
		return 1;
	}
	//去掉固件信息头“upgrade_crypt_v1!@2021”
	if (fread(enc_buf, 1,22,enc_fd) != 22) {
		printf("[\033[1;31mERROR\033[0m][DF] Error to read from the encrypted firmware.\n");
		free(enc_buf);
		free(dec_buf);
		fclose(enc_fd);
		fclose(dec_fd);
		free(dec_path);
		return 1;
	}
	size -= 22;

	while(size > 0)
	{
		int len = size;
		if (size > 0x1000) len = 0x1000;
		
		memset(enc_buf, 0, 0x1000);
		memset(dec_buf, 0, 0x1000);
		
		if (fread(enc_buf, 1,len,enc_fd) != len) {
			printf("[\033[1;31mERROR\033[0m][DF] Error to read from the encrypted firmware.\n");
			free(enc_buf);
			free(dec_buf);
			fclose(enc_fd);
			fclose(dec_fd);
			free(dec_path);
			return 1;
		}
		decrypt(enc_buf, dec_buf, len);
		if (fwrite(dec_buf, 1,len,dec_fd) != len) {
			printf("[\033[1;31mERROR\033[0m][DF] Error to write into the decrypted firmware.\n");
			free(enc_buf);
			free(dec_buf);
			fclose(enc_fd);
			fclose(dec_fd);
			free(dec_path);
		return 1;
		}
		size -= len;
	}
	
	free(enc_buf);
	free(dec_buf);
	fclose(enc_fd);
	fclose(dec_fd);
	free(dec_path);
	return 0;
}

int firmware_encrypt(const char *dec_path) {
	char *enc_path = malloc(strlen(dec_path) + 0x10);
	strcpy(enc_path, dec_path);
	strcat(enc_path, ".encrypted");
	struct stat stat_buf;
	const int stat_fd = stat(dec_path, &stat_buf);
	if (stat_fd < 0) {
		printf("[\033[1;31mERROR\033[0m][EF] The decrypted firmware does not exist.\n");
		free(enc_path);
		return 1;
	}
	int size = stat_buf.st_size;

	uint8_t *enc_buf = malloc(0x1000);
	uint8_t *dec_buf = malloc(0x1000);

	FILE *dec_fd = fopen(dec_path, "rb");
	if (dec_fd < 0) {
		printf("[\033[1;31mERROR\033[0m][EF] Error to open the decrypted firmware.\n");
		free(enc_buf);
		free(dec_buf);
		fclose(dec_fd);
		free(enc_path);
		return 1;
	}
	FILE *enc_fd = fopen(enc_path, "wb");
	if (enc_fd < 0) {
		printf("[\033[1;31mERROR\033[0m][EF] Error to create the encrypted firmware.\n");
		free(enc_buf);
		free(dec_buf);
		fclose(dec_fd);
		fclose(enc_fd);
		free(enc_path);
		return 1;
	}
	const char header[]="upgrade_crypt_v1!@2021";
	fwrite(header, strlen(header), 1, enc_fd);
	
	while(size > 0)
	{
		int len = size;
		if (size > 0x1000) len = 0x1000;
		
		memset(enc_buf, 0, 0x1000);
		memset(dec_buf, 0, 0x1000);
		
		if (fread(enc_buf, 1,len,dec_fd) != len) {
			printf("[\033[1;31mERROR\033[0m][EF] Error to read from the decrypted firmware.\n");
			free(enc_buf);
			free(dec_buf);
			fclose(dec_fd);
			fclose(enc_fd);
			free(enc_path);
			return 1;
		}
		decrypt(enc_buf, dec_buf, len);
		if (fwrite(dec_buf, 1,len,enc_fd) != len) {
			printf("[\033[1;31mERROR\033[0m][EF] Error to write into the encrypted firmware.\n");
			free(enc_buf);
			free(dec_buf);
			fclose(dec_fd);
			fclose(enc_fd);
			free(enc_path);
			return 1;
		}
		size -= len;
	}
	
	free(enc_buf);
	free(dec_buf);
	fclose(dec_fd);
	fclose(enc_fd);
	free(enc_path);
	return 0;
}

int backup_decrypt() {

	return 0;
}

int backup_encrypt() {

	return 0;
}

