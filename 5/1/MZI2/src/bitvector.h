#ifndef MZI_BITVECTOR_H
#define MZI_BITVECTOR_H

#include <cstdint>

#define CURVE_DEGREE      163
#define BITVEC_MARGIN     3 // margin for overhead needed in intermediate calculations
#define BITVEC_NBITS      (CURVE_DEGREE + BITVEC_MARGIN)
#define BITVEC_NWORDS     ((BITVEC_NBITS + 31) / 32)
#define BITVEC_NBYTES     (sizeof(uint32_t) * BITVEC_NWORDS)

#define ECC_PRV_KEY_SIZE   21
#define ECC_PUB_KEY_SIZE   (2 * ECC_PRV_KEY_SIZE)

typedef uint32_t bitvec_t[BITVEC_NWORDS];

static int bitvec_get_bit(const bitvec_t x, const uint32_t idx)
{
    return ((x[idx / 32] >> (idx & 31) & 1));
}

static void bitvec_clear_bit(bitvec_t x, const uint32_t idx)
{
    x[idx / 32] &= ~(1 << (idx & 31));
}

static void bitvec_copy(bitvec_t x, const bitvec_t y)
{
    int i;
    for (i = 0; i < BITVEC_NWORDS; ++i)
    {
        x[i] = y[i];
    }
}

static void bitvec_swap(bitvec_t x, bitvec_t y)
{
    bitvec_t tmp;
    bitvec_copy(tmp, x);
    bitvec_copy(x, y);
    bitvec_copy(y, tmp);
}

static int bitvec_equals(const bitvec_t x, const bitvec_t y)
{
    int i;
    for (i = 0; i < BITVEC_NWORDS; ++i)
    {
        if (x[i] != y[i])
        {
            return 0;
        }
    }
    return 1;
}

static void bitvec_set_zero(bitvec_t x)
{
    int i;
    for (i = 0; i < BITVEC_NWORDS; ++i)
    {
        x[i] = 0;
    }
}

static int bitvec_is_zero(const bitvec_t x)
{
    uint32_t i = 0;
    while (i < BITVEC_NWORDS)
    {
        if (x[i] != 0)
        {
            break;
        }
        i += 1;
    }
    return (i == BITVEC_NWORDS);
}

/* return the number of the highest one-bit + 1 */
static int bitvec_degree(const bitvec_t x)
{
    int i = BITVEC_NWORDS * 32;

    /* Start at the back of the vector (MSB) */
    x += BITVEC_NWORDS;

    /* Skip empty / zero words */
    while (    (i > 0)
               && (*(--x)) == 0)
    {
        i -= 32;
    }
    /* Run through rest if count is not multiple of bitsize of DTYPE */
    if (i != 0)
    {
        uint32_t u32mask = ((uint32_t)1 << 31);
        while (((*x) & u32mask) == 0)
        {
            u32mask >>= 1;
            i -= 1;
        }
    }
    return i;
}

/* left-shift by 'count' digits */
static void bitvec_lshift(bitvec_t x, const bitvec_t y, int nbits)
{
    int nwords = (nbits / 32);

    /* Shift whole words first if nwords > 0 */
    int i,j;
    for (i = 0; i < nwords; ++i)
    {
        /* Zero-initialize from least-significant word until offset reached */
        x[i] = 0;
    }
    j = 0;
    /* Copy to x output */
    while (i < BITVEC_NWORDS)
    {
        x[i] = y[j];
        i += 1;
        j += 1;
    }

    /* Shift the rest if count was not multiple of bitsize of DTYPE */
    nbits &= 31;
    if (nbits != 0)
    {
        /* Left shift rest */
        int i;
        for (i = (BITVEC_NWORDS - 1); i > 0; --i)
        {
            x[i]  = (x[i] << nbits) | (x[i - 1] >> (32 - nbits));
        }
        x[0] <<= nbits;
    }
}

#endif //MZI_BITVECTOR_H
