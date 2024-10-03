#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libintl.h>
#include <locale.h>

#include "password.h"
#include "crypt.h"

#define _(STRING) gettext(STRING)

char *_fgets(char data[], const int count, FILE *file)
{
    fgets(data, count, file);
    char *find = strchr(data, '\n');  //找出data中的"\n"
    if(find)
        *find = '\0';   //替换
    return data;
}

void generatepw(const char *sn_bf) {
    printf("[\033[1;32m%s\033[0m][%s] %s\n",_("INFO"),_("RPG"),_("Staring Password generate."));
    char hash[33] = {0};// 32 十六进制字符 + null 终止符
    passwd_generate(sn_bf, hash, sizeof(hash));// 生成密码
    if (strlen(hash) != 32) {
        printf("[\033[1;31m%s\033[0m][%s] %s\n",_("ERROR"),_("RPG"),_("Password generate faild."));
    }else {
        char password[17] = {0};
        strncpy(password, hash, 16);
        printf("[\033[1;32m%s\033[0m][%s] %s: \033[1m\033[4m\033[1;33m%s\033[0m \n",_("INFO"),_("RPG"),_("Password"),password);
    }
}

void pg() {
    char sn_bf[20];
    printf("\n");
    printf("[\033[1;32m%s\033[0m][%s] %s\n",_("INFO"),_("RPG"),_("You selected Root Password Generate."));
    printf("[\033[1;32m%s\033[0m][%s] %s\n",_("INFO"),_("RPG"),_("Please enter your serial number."));
    printf("[\033[1;32m%s\033[0m][%s] %s:",_("INFO"),_("RPG"),_("SN"));
    _fgets(sn_bf,20,stdin);
    generatepw(sn_bf);
    printf("[\033[1;32m%s\033[0m][%s] \033[5m%s\033[0m\n",_("INFO"),_("RPG"),_("Press any key to continue."));
}

void df() {
    char path[4096]={0};
    printf("\n");
    printf("[\033[1;32m%s\033[0m][%s] %s\n",_("INFO"),_("DF"),_("You selected Decrypt Firmware."));
    printf("[\033[1;32m%s\033[0m][%s] %s\n",_("INFO"),_("DF"),_("Please enter your firmware path."));
    printf("[\033[1;32m%s\033[0m][%s] %s:",_("INFO"),_("DF"),_("PATH"));
    _fgets(path,4096,stdin);
    if(firmware_decrypt(path)) {
        printf("[\033[1;31m%s\033[0m][%s] %s.\n",_("ERROR"),_("DF"),_("Decrypt firmware faild."));
    } else {
        printf("[\033[1;32m%s\033[0m][%s] %s.\n",_("INFO"),_("DF"),_("Decrypt firmware succeed."));
    }
    printf("[\033[1;32m%s\033[0m][%s] \033[5m%s\033[0m\n",_("INFO"),_("DF"),_("Press any key to continue."));
}

void ef() {
    char path[4096]={0};
    printf("\n");
    printf("[\033[1;32m%s\033[0m][%s] %s\n",_("INFO"),_("EF"),_("You selected Encrypt Firmware."));
    printf("[\033[1;32m%s\033[0m][%s] %s\n",_("INFO"),_("EF"),_("Please enter your firmware path."));
    printf("[\033[1;32m%s\033[0m][%s] %s:",_("INFO"),_("EF"),_("PATH"));
    _fgets(path,4096,stdin);
    if(firmware_encrypt(path)) {
        printf("[\033[1;31m%s\033[0m][%s] %s\n",_("ERROR"),_("EF"),_("Encrypt firmware faild."));
    } else {
        printf("[\033[1;32m%s\033[0m][%s] %s\n",_("INFO"),_("EF"),_("Encrypt firmware succeed."));
    }
    printf("[\033[1;32m%s\033[0m][%s] \033[5m%s\033[0m\n",_("INFO"),_("EF"),_("Press any key to continue."));
}

void db() {
    
}

void eb() {
    
}

void windows_cmd_support_utf8(void)
{
#ifdef WIN32
    system("chcp 65001 & cls"); //cls 用来清除 chcp 的输出
#endif
}

void select_language() {
    printf("\033[H\033[2J\033[3J");
    char input_bf_lang[2];
    const int choice_lang;
    printf("%s\n",_("Please select a language."));
    printf("1. %s\n",_("English"));
    printf("2. %s\n",_("Chinese Simplified"));
    printf("%s\n",_("No input will automatically select language."));
    printf("%s: ",_("Enter your choice"));
    _fgets(input_bf_lang,3,stdin);
    sscanf(input_bf_lang,"%d",&choice_lang);
    switch (choice_lang) {
        case 1: setlocale (LC_ALL, "en");
        break;
        case 2: setlocale (LC_ALL, "zh_CN");
        break;
        default: setlocale (LC_ALL, "");
        break;
    }
}

void toolkit_settings() {
    printf("\n");
    char input_bf_lang[2];
    const int choice_lang;
    printf("[\033[1;32m%s\033[0m][%s] %s\n",_("INFO"),_("TS"),_("You selected Toolkit Setting."));
    printf("[\033[1;32m%s\033[0m][%s] %s\n",_("INFO"),_("TS"),_("Choose one function index form below menus"));
    printf("\n");
    printf("[\033[1;32m%s\033[0m][%s] 1. %s\n",_("INFO"),_("TS"),_("Language"));
    printf("\n");
    printf("[\033[1;32m%s\033[0m][%s] %s:",_("INFO"),_("TS"),_("Enter your choice"));
    _fgets(input_bf_lang,3,stdin);
    sscanf(input_bf_lang,"%d",&choice_lang);
    switch (choice_lang) {
        case 1: select_language();
                break;
        default: break;
    }
}

int main() {
    windows_cmd_support_utf8();
    setlocale (LC_ALL, "en");
    bindtextdomain ("main",  "Lang");
    textdomain ("main");
    char input_bf_lang[2];
    const int choice_lang;
    printf("%s\n",_("Please select a language."));
    printf("\n");
    printf("1. %s\n",_("English"));
    printf("2. %s\n",_("Chinese Simplified"));
    printf("\n");
    printf("%s\n",_("No input will automatically select language."));
    printf("%s: ",_("Enter your choice"));
    _fgets(input_bf_lang,3,stdin);
    sscanf(input_bf_lang,"%d",&choice_lang);
    switch (choice_lang) {
        case 1: setlocale (LC_ALL, "en");
        break;
        case 2: setlocale (LC_ALL, "zh_CN");
        break;
        default: setlocale (LC_ALL, "");
        break;
    }
    while (1) {
        printf("\033[H\033[2J\033[3J");
        printf("    ____        _   _ _         __________  ____  __    __ __ __________\n"
            "   / __ \\__  __(_) (_|_)__     /_  __/ __ \\/ __ \\/ /   / //_//  _/_  __/\n"
            "  / /_/ / / / / / / / / _ \\     / / / / / / / / / /   / ,<   / /  / /   \n"
            " / _, _/ /_/ / / / / /  __/    / / / /_/ / /_/ / /___/ /| |_/ /  / /    \n"
            "/_/ |_|\\__,_/_/_/ /_/\\___/    /_/  \\____/\\____/_____/_/ |_/___/ /_/     \n"
            "             /___/                                                      \n"

            "\n");
        printf("\033[1;36m%s\033[0m \033[1;33m%s\033[0m \033[1;35m%s\033[0m\n%s\n",_("NekokeCore@2024"),_("Version 1.0.0"),_("Made with Love"),_("Welcome to use Ruijie Toolkit."));
        printf("\n");
        char input_bf[2];
        const int choice;
        printf("%s\n",_("Choose one function index form below menus"));
        printf("\n");
        printf("1. %s\n",_("Root Password Generate"));
        printf("2. %s\n",_("Decrypt Firmware"));
        printf("3. %s\n",_("Encrypt Firmware"));
        printf("4. %s\n",_("Decrypt Backup"));
        printf("5. %s\n",_("Encrypt Backup"));
        printf("6. %s\n",_("Tookit Setting"));
        printf("0. %s\n",_("Exit"));
        printf("\n");
        printf("%s: ",_("Enter your choice"));
        _fgets(input_bf,3,stdin);
        sscanf(input_bf,"%d",&choice);
        switch (choice) {
            case 1: pg();
                    getchar();
                    break;
            case 2: df();
                    getchar();
                    break;
            case 3: ef();
                    getchar();
                    break;
            case 4: break;
            case 5: break;
            case 6: toolkit_settings();
                    break;
            case 0: return 0;
            default: break;
        }
    }
}
