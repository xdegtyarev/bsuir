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
    uint64_t c2_32 = 4294967296;
    uint32_t data = 0xFFFFFFFF;
    uint32_t keysG[8] = {0x33206D54,0x326C6568,0x20657369,0x626E7373,0x79676120,0x74746769,0x65686573,0x733D2C20};
    printf("\ndata\n");
    print_bits(data);
    uint64_t data64 = data;
    printf("\ndata64 %llx\n",data64);
    print_bits(data64);
    data64 += keysG[0];
    printf("\nadded data64 %llx\n",data64);
    print_bits(data64);
    data64 %=c2_32;
    printf("\nmodulo data64 %llx\n",data64);
    print_bits(data64);
    uint32_t tempBlock = (uint32_t) data64;
    printf("\ndata32 %x\n",tempBlock);
    print_bits(tempBlock);

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

        printf("Gen keys\n");
        //gostKeys;

      uint32_t keysG[8] = {0x33206D54,0x326C6568,0x20657369,0x626E7373,0x79676120,0x74746769,0x65686573,0x733D2C20};
//        uint32_t keysG[8] = {0,0,0,0,0,0,0,0};
        //desKeys;
        uint64_t* keys = new uint64_t[16];
        uint64_t initKey = 1383827165325090801;
        des_generate_keys(keys,initKey);
        printf("Keys done\n");


        printf("%s %s\n",config.type==0?"des":"gost28147",config.encrypt?"encrypt":"decrypt");
        uint64_t buf = 0;
        while(fread(&buf,sizeof(buf),1,inputFile) == 1){
            switch(config.type){
                case 0:
                    des_process_block(&buf,keys,config.encrypt);
                    break;
                case 1:
                    gost28147_process_block(&buf,keysG,config.encrypt);
                break;
                default:break;
            }
            fwrite(&buf, sizeof(buf),1,outputFile);
        }
        fclose(inputFile);
        fclose(outputFile);
    }
    return 0;
}

