#include "ecc.h"
int ecdh_generate_keys(uint8_t* publicKey, uint8_t* privateKey)
{
    //private should contain random data!
    gf2point_copy((uint32_t*)publicKey, (uint32_t*)(publicKey + BITVEC_NBYTES), base_x, base_y);

    if (bitvec_degree((uint32_t*)privateKey) < (CURVE_DEGREE / 2))
    {
        printf("\nERROR: Random is too small\n");
        return 0;
    }
    else
    {
        int nbits = bitvec_degree(base_order);
        int i;

        for (i = (nbits - 1); i < (BITVEC_NWORDS * 32); ++i)
        {
            /* Clear bits > CURVE_DEGREE in highest word to satisfy constraint 1 <= exp < n. */
            bitvec_clear_bit((uint32_t*)privateKey, i);
        }
        gf2point_mul((uint32_t*)publicKey, (uint32_t*)(publicKey + BITVEC_NBYTES), (uint32_t*)privateKey);
        return 1;
    }
}

int ecdh_shared_secret(const uint8_t* privateKey, const uint8_t* otherPublicKey, uint8_t* sharedSecret)
{
    if (!gf2point_is_zero ((uint32_t*)otherPublicKey, (uint32_t*)(otherPublicKey + BITVEC_NBYTES)) &&  gf2point_on_curve((uint32_t*)otherPublicKey, (uint32_t*)(otherPublicKey + BITVEC_NBYTES)) )
    {
        unsigned int i;
        for (i = 0; i < (BITVEC_NBYTES * 2); ++i)
        {
            sharedSecret[i] = otherPublicKey[i];
        }
        gf2point_mul((uint32_t*)sharedSecret,(uint32_t*)(sharedSecret + BITVEC_NBYTES), (const uint32_t*)privateKey); //otherPub*ourKey
        return 1;
    }
    else
    {
       return 0;
    }
}