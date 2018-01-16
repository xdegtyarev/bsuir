#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include "des.h"
#include "gost.h"
#include "rsa.h"
#include "caesar.h"
#include "aes.h"
#include "md5.h"
#include "ecc.h"

enum ALGO{
    /*PART1*/
    DES = 0,
    G28147 = 1,
    RSA = 2,
    G3411 = 3,
    G3410 = 4,
    /*PART2*/
    CAESAR = 5,
    AES = 6,
    MD5 = 7,
    ECC = 8
};

struct IPRConfig{
    const char *path = "";
    bool encrypt = true;
    bool genkeys = false;
    ALGO type = DES;
    const char *inputFilePath = "";
    const char *outputFilePath = "";
    uint32_t rsa_n = 0;
    uint32_t rsa_m = 0;
    uint32_t rsa_e = 0;
    bool isValid;
    bool hasOutPath;
};

void help(){
    printf("Usage:\n");
    printf("'DES', '28147','RSA', '3411', '3410', 'CAESAR', 'AES', 'MD5', 'ECC' to specify encryption type\n");
    printf("'-d': decrypt if no-flag: encrypt\n");
    printf("'-o': + output file path\n");
    printf("'-i': + input file path\n");
    printf("RSA Specific:\n");
    printf("'-g': generate keys\n");
    printf("'-e': specify E for keygen\n");
    printf("'-n': specify N key part\n");
    printf("'-m': specify E/D key part\n");
}

void test(){

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
            printf("arg %d : %s \n", i, args[i]);
            if (strcmp(args[i], "des") == 0 || strcmp(args[i], "DES") == 0) {
                config.type = DES;
                config.hasOutPath = true;
            } else if (strcmp(args[i], "28147") == 0) {
                config.type = G28147;
                config.hasOutPath = true;
            } else if (strcmp(args[i], "rsa") == 0 || strcmp(args[i], "RSA") == 0 ) {
                config.type = RSA;
                config.hasOutPath = true;
            } else if (strcmp(args[i], "3411") == 0) {
                config.type = G3411;
                config.hasOutPath = false;
            } else if (strcmp(args[i], "3410") == 0) {
                config.type = G3410;
                config.hasOutPath = false;
            } else if (strcmp(args[i], "caesar") == 0 || strcmp(args[i],"Caesar") == 0) {
                config.type = CAESAR;
                config.hasOutPath = true;
            } else if (strcmp(args[i], "aes") == 0 || strcmp(args[i],"AES") == 0) {
                config.type = AES;
                config.hasOutPath = true;
            } else if (strcmp(args[i], "md5") == 0 || strcmp(args[i],"MD5") == 0) {
                config.type = MD5;
                config.hasOutPath = false;
            } else if (strcmp(args[i], "ECC") == 0 || strcmp(args[i],"ecc") == 0) {
                config.type = ECC;
                config.hasOutPath = false;
            } else if (strcmp(args[i], "o") == 0 || strcmp(args[i], "-o") == 0) {
                i++;
                if (i < argc) {config.outputFilePath = args[i];}
            } else if (strcmp(args[i], "i") == 0 || strcmp(args[i], "-i") == 0) {
                i++;
                if (i < argc) {config.inputFilePath = args[i];}
            } else if (strcmp(args[i], "d") == 0 || strcmp(args[i], "-d") == 0) {
                config.encrypt = false;
            } else if (strcmp(args[i], "g") == 0 || strcmp(args[i], "-g") == 0) {
                config.genkeys = true;
            } else if (strcmp(args[i], "n") == 0 || strcmp(args[i], "-n") == 0) {
                i++;
                if (i < argc) {config.rsa_n = (uint32_t) atol(args[i]);}
            } else if (strcmp(args[i], "m") == 0 || strcmp(args[i], "-m") == 0) {
                i++;
                if (i < argc) {config.rsa_m = (uint32_t) atol(args[i]);}
            } else if (strcmp(args[i], "e") == 0 || strcmp(args[i], "-e") == 0) {
                i++;
                if (i < argc) {config.rsa_e = (uint32_t) atol(args[i]);}
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
            if(config.type == RSA) {
                rsa_generate_keys(8, config.rsa_e);
            }else if(config.type == ECC){
                printf("\nECC Generate Keys\n");
                static uint8_t puba[ECC_PUB_KEY_SIZE];
                static uint8_t prva[ECC_PRV_KEY_SIZE];
                static uint8_t seca[ECC_PUB_KEY_SIZE];
                static uint8_t pubb[ECC_PUB_KEY_SIZE];
                static uint8_t prvb[ECC_PRV_KEY_SIZE];
                static uint8_t secb[ECC_PUB_KEY_SIZE];
                uint32_t i;

                static int initialized = 0;
                if (!initialized)
                {
                    prng_init((0xbad ^ 0xc0ffee ^ 42) | 0xcafebabe | 666);
                    initialized = 1;
                }

                /* 1.Alice picks a (secret) random natural number 'a', calculates P = a * g and sends P to Bob. */
                printf("\nAlice rnd prvkeys: \n");
                for (i = 0; i < ECC_PRV_KEY_SIZE; ++i)
                {
                    prva[i] = prng_next();
                    printf("%2.2x",prva[i]);
                }
                printf("\n\n");
                if(ecdh_generate_keys(puba, prva)){
                    printf("\nAlice keys generated: \n");
                    printf("\nPUBLIC\n");
                    for(int i = 0; i<ECC_PUB_KEY_SIZE; i++){
                        printf("%2.2x",puba[i]);
                    }
                    printf("\nPRIVATE\n");
                    for(int i = 0; i<ECC_PRV_KEY_SIZE; i++){
                        printf("%2.2x",prva[i]);
                    }
                }
                printf("\n\n");
                /* 2. Bob picks a (secret) random natural number 'b', calculates Q = b * g and sends Q to Alice. */
                printf("\nBOB rnd prvkeys: \n");
                for (i = 0; i < ECC_PRV_KEY_SIZE; ++i)
                {
                    prvb[i] = prng_next();
                    printf("%2.2x",prvb[i]);
                }
                printf("\n\n");
                if(ecdh_generate_keys(pubb, prvb)){
                    printf("\nBob keys generated: \n");
                    printf("\nPUBLIC\n");
                    for(int i = 0; i<ECC_PUB_KEY_SIZE; i++){
                        printf("%2.2x",pubb[i]);
                    }
                    printf("\nPRIVATE\n");
                    for(int i = 0; i<ECC_PRV_KEY_SIZE; i++){
                        printf("%2.2x",prvb[i]);
                    }
                }
                printf("\n\n");
                if(ecdh_shared_secret(prva, pubb, seca)){
                    /*S = a * Q = a * (b * g). */
                    printf("\nAlice sharedSecret:\n");
                    for(int i = 0; i<ECC_PRV_KEY_SIZE; i++){
                        printf("%2.2x",seca[i]);
                    }
                }

                printf("\n\n");

                if(ecdh_shared_secret(prvb, puba, secb)){
                    /*T = b * P = b * (a * g). */
                    printf("\nBob sharedSecret:\n");
                    for(int i = 0; i<ECC_PRV_KEY_SIZE; i++){
                        printf("%2.2x",secb[i]);
                    }
                }

                printf("\n\n");

                for (i = 0; i < ECC_PUB_KEY_SIZE; ++i)
                {
                    assert(seca[i] == secb[i]);
                }
                printf("Success: both recepients calculated same value");
            }
        }else {
            FILE *inputFile = fopen(config.inputFilePath, "rb");
            if (!inputFile) {
                printf("\ninputFile opening failed: %s\n", config.inputFilePath);
                return EXIT_FAILURE;
            }

            long inputFileSize  = 0;
            fseek (inputFile, 0 , SEEK_END);
            inputFileSize = ftell (inputFile);
            rewind (inputFile);

            printf("\nFile '%s' with size of %d bytes\n",config.inputFilePath,inputFileSize);

            if(!config.hasOutPath) {
                if(config.type == MD5){
                    printf("%s\n", "MD5");
                    uint32_t* buf32 = (uint32_t *)malloc(sizeof(uint32_t)*16); //512bit buffer
                    uint32_t hash0 = md5_a0;
                    uint32_t hash1 = md5_b0;
                    uint32_t hash2 = md5_c0;
                    uint32_t hash3  = md5_d0;

                    uint64_t totalReadBytes = 0;
                    uint64_t chunkReadBytes = 0;
                    uint64_t chunkNum = 0;
                    bool extrastep = false;
                    do{
                        chunkReadBytes = fread(buf32, sizeof(uint32_t),16,inputFile)*sizeof(uint32_t);
                        totalReadBytes += chunkReadBytes;
                        printf("\nRead chunk: %llu bytes: %llu totalRead: %llu of: %llu\n",chunkNum,chunkReadBytes,totalReadBytes,inputFileSize);
                        if(totalReadBytes==inputFileSize){
                            printf("\nFile Read\n");
                            if(chunkReadBytes<=56){
                                uint64_t extrazeroes =  (56-chunkReadBytes)/ sizeof(uint32_t);
                                printf("\nLess than 56, needs extra padding needs %d\n",extrazeroes);
                                uint64_t newLen;
                                for(newLen = chunkReadBytes*8 + 1; newLen%512!=448; newLen++);
                                newLen /= 8;
                                uint8_t* newBuf = NULL;
                                newBuf = (uint8_t*) calloc(newLen + 64, 1);
                                memcpy(newBuf, buf32, chunkReadBytes);
                                newBuf[chunkReadBytes] = 128;
                                uint32_t bitsLen = 8 * inputFileSize;
                                memcpy(newBuf + newLen, &bitsLen,4);
                                memcpy(buf32,newBuf,64);
                                free(newBuf);
                            }else{
                                extrastep = true;
                                printf("\nMore than 56, needs extra step\n");
                            }
                        }
                        //expand buf
                        md5_process_block(buf32,&hash0,&hash1,&hash2,&hash3);
                        chunkNum++;
                    }while(totalReadBytes<inputFileSize);
                    //print md5 hash
                    uint8_t *p;
                    p=(uint8_t *)&hash0;
                    printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3], hash0);
                    p=(uint8_t *)&hash1;
                    printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3], hash1);
                    p=(uint8_t *)&hash2;
                    printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3], hash2);
                    p=(uint8_t *)&hash3;
                    printf("%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3], hash3);
                    puts("");
                    free(buf32);
                }else if(config.type == G3410){
                    printf("%s\n", "G3410");
                }else if(config.type == G3411){
                    printf("%s\n", "G3411");
                }
            }else{
                FILE *outputFile = fopen(config.outputFilePath, "wb");
                if (!outputFile) {
                    printf("\noutputFile opening failed: %s\n", config.outputFilePath);
                    return EXIT_FAILURE;
                }

                if (config.type == RSA) {
                    printf("%s %s N:%u M:%u\n", "RSA", config.encrypt ? "encrypt" : "decrypt", config.rsa_n, config.rsa_m);
                    //RSA USES DIFFERENT BUFFERS FOR R/W
                    uint8_t buf8 = 0;
                    uint32_t buf32 = 0;

                    if (config.encrypt) {
                        while (fread(&buf8, sizeof(buf8), 1, inputFile) == 1) {
                            buf32 = rsa_process_block(config.rsa_n, config.rsa_m, buf8);
                            fwrite(&buf32, sizeof(buf32), 1, outputFile);
                        }
                    } else {
                        while (fread(&buf32, sizeof(buf32), 1, inputFile) == 1) {
                            buf8 = (uint8_t) rsa_process_block(config.rsa_n, config.rsa_m, buf32);
                            fwrite(&buf8, sizeof(buf8), 1, outputFile);
                        }
                    }
                } else if(config.type == DES){
                    printf("%s %s\n", "DES", config.encrypt ? "encrypt" : "decrypt");
                    uint64_t buf = 0;
                    uint64_t *keys = (uint64_t*)malloc(sizeof(uint64_t)*64);
                    des_generate_keys(keys, des_default_init_key);
                    while (fread(&buf, sizeof(buf), 1, inputFile) == 1) {
                        des_process_block(&buf, keys, config.encrypt);
                        fwrite(&buf, sizeof(buf), 1, outputFile);
                    }
                    free(keys);
                } else if(config.type == G28147){
                    printf("%s %s\n", "G28147", config.encrypt ? "encrypt" : "decrypt");
                    uint64_t buf = 0;
                    uint32_t g28147_default_key[8] = {
                            0x33206D54, 0x326C6568, 0x20657369, 0x626E7373,
                            0x79676120, 0x74746769, 0x65686573, 0x733D2C20
                    };
                    while (fread(&buf, sizeof(buf), 1, inputFile) == 1) {
                        gost28147_process_block(&buf, g28147_default_key, config.encrypt);
                        fwrite(&buf, sizeof(buf), 1, outputFile);
                    }
                }else if(config.type == CAESAR){
                    printf("%s %s\n", "caesar", config.encrypt ? "encrypt" : "decrypt");
                    uint8_t buf8 = 0;
                    uint8_t shift = 4;
                    while (fread(&buf8, sizeof(buf8),1,inputFile) == 1){
                        caesar_process_block(&buf8,shift,config.encrypt);
                        fwrite(&buf8, sizeof(buf8),1,outputFile);
                    }
                }else if(config.type == AES){
                    printf("%s %s\n", "AES", config.encrypt ? "encrypt" : "decrypt");
                    uint8_t *roundKey = new uint8_t[176];
                    aes_generate_key(roundKey,aes_default_key);
                    uint8_t* buf8 = (uint8_t *)malloc(sizeof(uint8_t)*16);
                    while (fread(buf8, sizeof(uint8_t),16,inputFile) == 16){
                        aes_process_block(buf8,roundKey,config.encrypt);
                        fwrite(buf8, sizeof(buf8[0]),16,outputFile);
                    }
                    free(buf8);
                }
                fclose(outputFile);
            }
            fclose(inputFile);

        }
    }
    return 0;
}

