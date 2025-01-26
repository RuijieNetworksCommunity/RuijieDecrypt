//
// Created by nekok on 24-7-21.
//

#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include "include/password.h"

// 初始化 MD5 上下文
void init_md5(EVP_MD_CTX *md5_ctx) {
    EVP_DigestInit_ex(md5_ctx, EVP_md5(),NULL);
}

// 更新 MD5 上下文
void update_md5(EVP_MD_CTX *md5_ctx, const void *data, const size_t len) {
    EVP_DigestUpdate(md5_ctx, data, len);
}

// 完成 MD5 计算
void finalize_md5(EVP_MD_CTX *md5_ctx,unsigned char digest[16]) {
    EVP_DigestFinal_ex(md5_ctx,digest,NULL);
}

// 将 MD5 摘要转换为十六进制字符串
void md5_to_hex(const unsigned char digest[16], char *output) {
    for (int i = 0; i < 16; ++i) {
        sprintf(output + i * 2, "%02x", digest[i]);
    }
}

// 生成密码函数
void passwd_generate(const char *sn_bf, char *password, const size_t password_len) {
    if (sn_bf == NULL) {
        printf("[\033[1;31m错误\033[0m][密码生成器] 序列号为空。\n");
        return;
    }
    char sn[14]={0};
    strncpy(sn,sn_bf,14);
    
    EVP_MD_CTX *md5_ctx = EVP_MD_CTX_new();
    uint8_t md5_data[64];
    uint8_t md5_result[16];

    memset(md5_data, 0, sizeof(md5_data));
    memset(md5_result, 0, sizeof(md5_result));

    // 拼接数据
    const size_t sn_len = strlen(sn);
    if (sn_len < sizeof(md5_data) - sizeof(PASSWDHEADER) - sizeof(PASSWDFOOTER)) {
        memcpy(md5_data, PASSWDHEADER, sizeof(PASSWDHEADER));
        memcpy(md5_data + sizeof(PASSWDHEADER), sn, sn_len);
        memcpy(md5_data + sizeof(PASSWDHEADER) + sn_len, PASSWDFOOTER, sizeof(PASSWDFOOTER));

        // 初始化 MD5
        init_md5(md5_ctx);

        // 更新 MD5 上下文
        update_md5(md5_ctx, md5_data, sizeof(PASSWDHEADER) + sn_len + sizeof(PASSWDFOOTER));

        // 计算 MD5 摘要
        finalize_md5(md5_ctx,md5_result);

        // 将 MD5 摘要转换为十六进制字符串
        md5_to_hex(md5_result, password);

        // 确保密码长度
        if (password_len >= 33) {
            password[32] = '\0';
            printf("[\033[1;32m信息\033[0m][密码生成器] 密码生成成功。\n");
        } else {
            printf("[\033[1;31m错误\033[0m][密码生成器] 密码生成失败！\n");
        }
    } else {
        printf("[\033[1;31m错误\033[0m][密码生成器] 序列号格式错误。\n");
    }
    EVP_MD_CTX_free(md5_ctx);
}