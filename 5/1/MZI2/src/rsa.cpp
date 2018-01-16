//
// Created by Alexander Degtyarev on 5/21/17.
//
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include "rsa.h"
//TODO: improve with GMP https://gmplib.org/

//crypt = pow(msg,e)%n;
//msg = pow(crypt,d)%n = pow(pow(msg,e),d)%n = pow(msg,e*d)%n

//n is known
//e for sender [en] - openkey
//d for receiver [de] - closedkey
//limits:
//pow(msh,e*d)==msg % n | msg < n

bool isPrime(uint64_t number)
{
    for (int i = 2; i < number / 2; i++)
    {
        if (number % i == 0) return false;
    }
    return true;
}

//compute a^b mod c
//https://www.topcoder.com/community/data-science/data-science-tutorials/primality-testing-non-deterministic-algorithms/
uint64_t powm(uint64_t base,uint32_t exp, uint32_t modulus){
    base = base % modulus;

    uint64_t result = 1;
    while (exp > 0) {
        if (exp & 1){
            result = (result * base) % modulus;
        }
        base = (base * base) % modulus;
        exp >>= 1;
    }
    return (uint64_t) result;
}

uint64_t powmm(uint64_t base,uint32_t exp, uint32_t modulus){
    if(modulus == 1)
        return  0;

    uint64_t  res = 1;
    uint64_t prevRes = 1;
    for(uint64_t eprime = 1;eprime<exp;eprime++){
        prevRes = res;
        res = ((res % modulus)*(base % modulus)) % modulus;
    }
    return res;
}

//function modular_pow(base, exponent, modulus)
//if modulus = 1 then return 0
//c := 1
//for e_prime = 1 to exponent
//c := (c * base) mod modulus
//return c

uint64_t gcd(uint64_t a, uint64_t b) {
    return b == 0 ? a : gcd(b, a % b);
}

void rsa_generate_keys(uint32_t bits, uint32_t e) {
    srand((unsigned int) time(0));
    //generate random P and Q
    uint64_t p = 0;
    uint64_t q = 0;

    printf("\ne %u",e);

    do{
        p = (uint8_t) rand();
    }while(!(p<128 && isPrime(p) && gcd(e,p-1)==1));
    printf("\np %llu",p);

    do{
        q = (uint8_t) rand();
    }while(!(q<128 && isPrime(q) && gcd(e,q-1)==1));
    printf("\nq %llu",q);

    uint64_t n = p*q;
    uint64_t phi = (p-1)*(q-1);
    uint64_t d = (2*phi+1)/e;

    printf("\nn %llu",n);
    printf("\nphi %llu",phi);
    printf("\nd %llu",d);

    printf("\npublic key: (n: %llu e: %u)\n",n,e);
    printf("\nprivate key: (n: %llu d: %llu)\n",n,d);
}