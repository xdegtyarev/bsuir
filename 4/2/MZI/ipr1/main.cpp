#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "des.h"
#include "gost.h"

struct IPRConfig{
    const char *path = "";
    bool encrypt = true;
    int type = 0;//0-DES, 1-28147
    const char *inputFilePath = "";
    const char *outputFilePath = "";
    const char *key = "";
    bool isValid;
};

void help(){
    printf("Usage:\n");
    printf("'des' or '28147' to specify encryption type\n");
    printf("'-d': decrypt if no-flag: encrypt\n");
    printf("'-o': + output file path\n");
    printf("'-i': + input file path\n");
    printf("'-k': + key file path\n");
}

void test(){
    uint64_t initKey = 1383827165325090801;
    print_bits(initKey);
    set_bit(&initKey,0,0,35);
    print_bits(initKey);
}


IPRConfig parseArgs(char* args[], int argc){
    IPRConfig config;
    if(argc<2) {
        printf("No arguments specified\n");
        help();
    }else {
        config.path = args[0];
        int i;
        for (i = 1; i < argc; i++) {
            printf("\nParsing arg %d : %s \n", i, args[i]);
            if (strcmp(args[i], "des") == 0 || strcmp(args[i], "DES") == 0) {
                config.type = 0;
            } else if (strcmp(args[i], "28147") == 0) {
                config.type = 1;
            } else if (strcmp(args[i], "o") == 0 || strcmp(args[i], "-o") == 0) {
                i++;
                if (i < argc) {
                    config.outputFilePath = args[i];
                }
            } else if (strcmp(args[i], "i") == 0 || strcmp(args[i], "-i") == 0) {
                i++;
                if (i < argc) {
                    config.inputFilePath = args[i];
                }
            } else if (strcmp(args[i], "d") == 0 || strcmp(args[i], "-d") == 0) {
                config.encrypt = false;
            } else if (strcmp(args[i], "k") == 0 || strcmp(args[i], "-k") == 0) {
                i++;
                if (i < argc) {
                    config.key = args[i];
                }
            } else {
                printf("\nWrong arg %d : %s . Will be ignored\n", i, args[i]);
            }
        }
    }

    //check config validity
    config.isValid = (strcmp(config.inputFilePath,"") != 0);
    if(config.isValid && strcmp(config.outputFilePath, "") == 0) {
        auto outPath = (char *) malloc(sizeof(config.inputFilePath) + sizeof("_decrypted"));
        strcpy(outPath,config.inputFilePath);
        strcat(outPath,config.encrypt?"_encrypted":"_decrypted");
        config.outputFilePath = outPath;
    }

    return config;
}

int main(int argc, char* argv[]) {
    auto config = parseArgs(argv,argc);
    if(config.isValid){
        printf("\nMZI part1\n");

        FILE *inputFile = fopen(config.inputFilePath, "rb");
        if(!inputFile) {
            printf("\ninputFile opening failed: %s\n",config.inputFilePath);
            return EXIT_FAILURE;
        }

        FILE *outputFile = fopen(config.outputFilePath, "wb");
        if(!outputFile) {
            printf("\noutputFile opening failed: %s\n",config.outputFilePath);
            return EXIT_FAILURE;
        }

        printf("\nGen keys\n");
        uint64_t* keys = new uint64_t[16];
        uint64_t initKey = 1383827165325090801;
        des_generate_keys(keys,initKey);
        printf("\nKeys done\n");
        uint64_t buf = 0;
        while(fread(&buf,sizeof(buf),1,inputFile) == 1){
            switch(config.type){
                case 0:
                    if(config.encrypt){
                        des_encrypt_block(&buf,keys);
                    }else{
                        des_decrypt_block(&buf,keys);
                    }
                break;
                case 1:
                    if(config.encrypt){
                        gost_encrypt_block(&buf,keys);
                    }else{
                        gost_decrypt_block(&buf,keys);
                    }
                break;
                default:break;
            }
            fwrite(&buf, sizeof(buf),1,outputFile);
        }
        printf("\nComplete!\n");
        fclose(inputFile);
        fclose(outputFile);
    }
    return 0;
}

