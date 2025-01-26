//
// Created by nekok on 24-7-22.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/evp.h>
#include "include/crypt.h"

int key1 = RG_PASSWD_KEY1, key2 = RG_PASSWD_KEY2;

int firmware_decrypt(const char *enc_path) {
	char *dec_path = malloc(strlen(enc_path) + 0x10);
	strcpy(dec_path, enc_path);
	strcat(dec_path, ".decrypted");
	struct stat stat_buf;
	const int stat_fd = stat(enc_path, &stat_buf);
	if (stat_fd < 0) {
		printf("[\033[1;31m错误\033[0m][解密固件] 加密的固件不存在！\n");
		free(dec_path);
		return -1;
	}
	off_t size = stat_buf.st_size;

	uint8_t *enc_buf = malloc(0x1000);
	uint8_t *dec_buf = malloc(0x1000);

	FILE *enc_fd = fopen(enc_path, "rb");
	if (enc_fd < 0) {
		printf("[\033[1;31m错误\033[0m][解密固件] 无法读取加密的固件！\n");
		free(enc_buf);
		free(dec_buf);
		fclose(enc_fd);
		free(dec_path);
		return -1;
	}
	FILE *dec_fd = fopen(dec_path, "wb");
	if (dec_fd < 0) {
		printf("[\033[1;31m错误\033[0m][解密固件] 无法创建解密的固件！\n");
		free(enc_buf);
		free(dec_buf);
		fclose(enc_fd);
		fclose(dec_fd);
		free(dec_path);
		return -1;
	}
	//去掉固件信息头“upgrade_crypt_v1!@2021”
	if (fread(enc_buf, 1,22,enc_fd) != 22) {
		printf("[\033[1;31m错误\033[0m][解密固件] 固件解密时发生未知错误！\n");
		free(enc_buf);
		free(dec_buf);
		fclose(enc_fd);
		fclose(dec_fd);
		free(dec_path);
		return -1;
	}
	size -= 22;

	while(size > 0)
	{
		off_t len = size;
		if (size > 0x1000) len = 0x1000;
		
		memset(enc_buf, 0, 0x1000);
		memset(dec_buf, 0, 0x1000);
		
		if (fread(enc_buf, 1,len,enc_fd) != len) {
			printf("[\033[1;31m错误\033[0m][解密固件] 加密的固件数据载入内存失败！\n");
			free(enc_buf);
			free(dec_buf);
			fclose(enc_fd);
			fclose(dec_fd);
			free(dec_path);
			return -1;
		}

		for (int i = 0; i < len; i++)
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

		if (fwrite(dec_buf, 1,len,dec_fd) != len) {
			printf("[\033[1;31m错误\033[0m][解密固件] 解密的固件数据写入硬盘失败！\n");
			free(enc_buf);
			free(dec_buf);
			fclose(enc_fd);
			fclose(dec_fd);
			free(dec_path);
		return -1;
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
		printf("[\033[1;31m错误\033[0m][加密固件] 解密的固件不存在！\n");
		free(enc_path);
		return -1;
	}
	off_t size = stat_buf.st_size;

	uint8_t *enc_buf = malloc(0x1000);
	uint8_t *dec_buf = malloc(0x1000);

	FILE *dec_fd = fopen(dec_path, "rb");
	if (dec_fd < 0) {
		printf("[\033[1;31m错误\033[0m][加密固件] 无法读取解密的固件！\n");
		free(enc_buf);
		free(dec_buf);
		fclose(dec_fd);
		free(enc_path);
		return -1;
	}
	FILE *enc_fd = fopen(enc_path, "wb");
	if (enc_fd < 0) {
		printf("[\033[1;31m错误\033[0m][加密固件] 无法创建加密的固件!\n");
		free(enc_buf);
		free(dec_buf);
		fclose(dec_fd);
		fclose(enc_fd);
		free(enc_path);
		return -1;
	}

	fwrite(FW_HEADER, strlen(FW_HEADER), 1, enc_fd);
	
	while(size > 0)
	{
		off_t len = size;
		if (size > 0x1000) len = 0x1000;
		
		memset(enc_buf, 0, 0x1000);
		memset(dec_buf, 0, 0x1000);
		
		if (fread(enc_buf, 1,len,dec_fd) != len) {
			printf("[\033[1;31m错误\033[0m][加密固件] 解密的固件数据载入内存失败！\n");
			free(enc_buf);
			free(dec_buf);
			fclose(dec_fd);
			fclose(enc_fd);
			free(enc_path);
			return -1;
		}

		for (int i = 0; i < len; i++)
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

		if (fwrite(dec_buf, 1,len,enc_fd) != len) {
			printf("[\033[1;31m错误\033[0m][加密固件] 加密的固件数据写入硬盘失败！\n");
			free(enc_buf);
			free(dec_buf);
			fclose(dec_fd);
			fclose(enc_fd);
			free(enc_path);
			return -1;
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

