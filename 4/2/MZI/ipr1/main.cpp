#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "des.h"
#include "gost.h"
#include "rsa.h"

struct IPRConfig{
    const char *path = "";
    bool encrypt = true;
    bool genkeys = false;
    int type = 0;//0-DES, 1-28147, 2-RSA
    const char *inputFilePath = "";
    const char *outputFilePath = "";
    const char *key = "";
    uint32_t rsa_n = 0;
    uint32_t rsa_m = 0;
    bool isValid;
};

void help(){
    printf("Usage:\n");
    printf("'des' or '28147' or 'rsa' to specify encryption type\n");
    printf("'-g': generate keys\n");
    printf("'-d': decrypt if no-flag: encrypt\n");
    printf("'-o': + output file path\n");
    printf("'-i': + input file path\n");
//    printf("'-k': + key file path\n");
}

void test(){
    uint8_t t = 15;
    printf("\nencrypt");
    uint32_t  res = rsa_process_block(1364143303,3,t);
    printf("\ndecrypt %llu",res);
    t = (uint8_t) rsa_process_block(1364143303, 909377019, res);
    printf("\n t is %d\n",t);
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
            printf("Parsing arg %d : %s \n", i, args[i]);
            if (strcmp(args[i], "des") == 0 || strcmp(args[i], "DES") == 0) {
                config.type = 0;
            } else if (strcmp(args[i], "28147") == 0) {
                config.type = 1;
            } else if (strcmp(args[i], "rsa") == 0 || strcmp(args[i], "RSA") == 0 ) {
                config.type = 2;
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
            } else if (strcmp(args[i], "g") == 0 || strcmp(args[i], "-g") == 0) {
                config.genkeys = true;
            } else if (strcmp(args[i], "k") == 0 || strcmp(args[i], "-k") == 0) {
                i++;
                if (i < argc) {
                    config.key = args[i];
                }
            } else if (strcmp(args[i], "n") == 0 || strcmp(args[i], "-n") == 0) {
                i++;
                if (i < argc) {
                    config.rsa_n = (uint32_t) atol(args[i]);
                }
            } else if (strcmp(args[i], "m") == 0 || strcmp(args[i], "-m") == 0) {
                i++;
                if (i < argc) {
                    config.rsa_m = (uint32_t) atol(args[i]);
                }
            } else {
                printf("\nWrong arg %d : %s . Will be ignored\n", i, args[i]);
            }
        }
    }

    //check config validity
    config.isValid = config.genkeys || (strcmp(config.inputFilePath,"") != 0);
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

        if(config.genkeys){
            rsa_generate_keys(8,3);
        }else {

            FILE *inputFile = fopen(config.inputFilePath, "rb");
            if (!inputFile) {
                printf("\ninputFile opening failed: %s\n", config.inputFilePath);
                return EXIT_FAILURE;
            }

            FILE *outputFile = fopen(config.outputFilePath, "wb");
            if (!outputFile) {
                printf("\noutputFile opening failed: %s\n", config.outputFilePath);
                return EXIT_FAILURE;
            }


            if (config.type == 2) {
                printf("%s %s N:%u M:%u\n", "RSA", config.encrypt ? "encrypt" : "decrypt",config.rsa_n,config.rsa_m);
                uint8_t buf8 = 0;
                uint32_t buf32 = 0;
                //check
                if(config.encrypt) {
                    while (fread(&buf8, sizeof(buf8), 1, inputFile) == 1) {
                        buf32 = rsa_process_block(config.rsa_n, config.rsa_m, buf8);
                        fwrite(&buf32, sizeof(buf32), 1, outputFile);
                    }
                }else{
                    while (fread(&buf32, sizeof(buf32), 1, inputFile) == 1) {
                        buf8 = (uint8_t) rsa_process_block(config.rsa_n, config.rsa_m, buf32);
                        fwrite(&buf8, sizeof(buf8), 1, outputFile);
                    }
                }
            } else {
                printf("Gen keys\n");
                //gostKeys;
                uint32_t keysG[8] = {0x33206D54, 0x326C6568, 0x20657369, 0x626E7373, 0x79676120, 0x74746769, 0x65686573,
                                     0x733D2C20};
                //desKeys;
                uint64_t *keys = new uint64_t[16];
                uint64_t initKey = 1383827165325090801;
                des_generate_keys(keys, initKey);
                printf("Keys done\n");

                printf("%s %s\n", config.type == 0 ? "des" : "gost28147", config.encrypt ? "encrypt" : "decrypt");
                uint64_t buf = 0;
                while (fread(&buf, sizeof(buf), 1, inputFile) == 1) {
                    switch (config.type) {
                        case 0:
                            des_process_block(&buf, keys, config.encrypt);
                            break;
                        case 1:
                            gost28147_process_block(&buf, keysG, config.encrypt);
                            break;
                        default:
                            break;
                    }
                    fwrite(&buf, sizeof(buf), 1, outputFile);
                }
            }
            fclose(inputFile);
            fclose(outputFile);
        }
    }
    return 0;
}

