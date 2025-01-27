//
// Created by nekok on 24-7-22.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/password.h"
#include "include/crypt.h"


char *_fgets(char data[], const int count, FILE *file)
{
    fgets(data, count, file);
    char *find = strchr(data, '\n');  //找出data中的"\n"
    if(find)
        *find = '\0';   //替换
    return data;
}

int main() {
#ifdef WIN32
    system("chcp 65001 & cls"); //cls 用来清除 chcp 的输出
#endif
    while (1) {
        printf("\033[H\033[2J\033[3J");
        printf("    ____        _   _ _         __________  ____  __    __ __ __________\n"
            "   / __ \\__  __(_) (_|_)__     /_  __/ __ \\/ __ \\/ /   / //_//  _/_  __/\n"
            "  / /_/ / / / / / / / / _ \\     / / / / / / / / / /   / ,<   / /  / /   \n"
            " / _, _/ /_/ / / / / /  __/    / / / /_/ / /_/ / /___/ /| |_/ /  / /    \n"
            "/_/ |_|\\__,_/_/_/ /_/\\___/    /_/  \\____/\\____/_____/_/ |_/___/ /_/     \n"
            "             /___/                                                      \n"

            "\n");
        printf("\033[1;36m版权所有©2024-2025 Ashley Lee (李淑贤)\033[0m \n\033[1;33m版本: Private-1.0.1\033[0m \n欢迎使用锐捷工具箱！\n");
        printf("\n");
        char input_bf[2];
        const int choice;
        printf("请输入模块序号: \n");
        printf("\n");
        printf("1. 密码生成模块\n");
        printf("2. 固件解密模块\n");
        printf("3. 固件加密模块\n");
        printf("0. 退出\n");
        printf("\n");
        printf("请选择: ");
        _fgets(input_bf,3,stdin);
        sscanf(input_bf,"%d",&choice);
        switch (choice) {
            case 1: char sn_bf[20];
                    printf("\n");
                    printf("[\033[1;32m信息\033[0m][密码生成器] 您选择了密码生成模块。\n");
                    printf("[\033[1;32m信息\033[0m][密码生成器] 请输入您设备的SN (Serial Number)。\n");
                    printf("[\033[1;32m信息\033[0m][密码生成器] SN:");
                    _fgets(sn_bf,20,stdin);
                    printf("[\033[1;32m信息\033[0m][密码生成器] 开始密码生成任务。\n");
                    char passwd_generate_hash[33] = {0};// 32 十六进制字符 + null 终止符
                    passwd_generate(sn_bf, passwd_generate_hash, sizeof(passwd_generate_hash));// 生成密码
                    if (strlen(passwd_generate_hash) != 32) {
                        printf("[\033[1;31m错误\033[0m][密码生成器] 密码生成任务失败！\n");
                    }else {
                        char generated_password[17] = {0};
                        strncpy(generated_password, passwd_generate_hash, 16);
                        printf("[\033[1;32m信息\033[0m][密码生成器] 密码: \033[1m\033[4m\033[1;33m%s\033[0m 。\n",generated_password);
                    }
                    printf("[\033[1;32m信息\033[0m][密码生成器] \033[5m请按任意键继续\033[0m\n");
                    getchar();
                    break;

            case 2: char firmware_decrypt_path[4096]={0};
                    printf("\n");
                    printf("[\033[1;32m信息\033[0m][解密固件] 您选择了解密固件模块。\n");
                    printf("[\033[1;32m信息\033[0m][解密固件] 请输入固件路径。\n");
                    printf("[\033[1;32m信息\033[0m][解密固件] 路径:");
                    _fgets(firmware_decrypt_path,4096,stdin);
                    if(firmware_decrypt(firmware_decrypt_path)) {
                        printf("[\033[1;31m错误\033[0m][解密固件] 固件解密失败！\n");
                    } else {
                        printf("[\033[1;32m信息\033[0m][解密固件] 固件解密成功。\n");
                    }
                    printf("[\033[1;32m信息\033[0m][解密固件] \033[5m请按任意键继续\033[0m\n");
                    getchar();
                    break;

            case 3: char firmware_encrypt_path[4096]={0};
                    printf("\n");
                    printf("[\033[1;32m信息\033[0m][加密固件] 您选择了加密固件模块。\n");
                    printf("[\033[1;32m信息\033[0m][加密固件] 请输入固件路径。\n");
                    printf("[\033[1;32m信息\033[0m][加密固件] 路径:");
                    _fgets(firmware_encrypt_path,4096,stdin);
                    if(firmware_encrypt(firmware_encrypt_path)) {
                        printf("[\033[1;31m错误\033[0m][加密固件] 固件加密失败！\n");
                    } else {
                        printf("[\033[1;32m信息\033[0m][加密固件] 固件加密成功。\n");
                    }
                    printf("[\033[1;32m信息\033[0m][加密固件] \033[5m请按任意键继续\033[0m\n");
                    getchar();
                    break;
            case 0: return 0;
            default: break;
        }
    }
}
