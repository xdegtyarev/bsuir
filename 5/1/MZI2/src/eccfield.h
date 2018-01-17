//
// Created by xdegtyarev on 1/16/18.
//

#ifndef MZI_ECCFIELD_H
#define MZI_ECCFIELD_H
#include <cstdint>
#include "utility.h"

#define ECC_PRV_KEY_SIZE   21
typedef bitvec_t gf2elem_t;//field element type
typedef bitvec_t scalar_t;//scalar element type


#define NIST_B163  1
#define NIST_K163  2

#ifndef ECC_CURVE
#define ECC_CURVE NIST_B163
#endif

#if defined (ECC_CURVE) && (ECC_CURVE != 0)

#if (ECC_CURVE == NIST_B163)
#define coeff_a  1
#define cofactor 2
const gf2elem_t polynomial = { 0x000000c9, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000008 };
const gf2elem_t coeff_b    = { 0x4a3205fd, 0x512f7874, 0x1481eb10, 0xb8c953ca, 0x0a601907, 0x00000002 };
const gf2elem_t base_x     = { 0xe8343e36, 0xd4994637, 0xa0991168, 0x86a2d57e, 0xf0eba162, 0x00000003 };
const gf2elem_t base_y     = { 0x797324f1, 0xb11c5c0c, 0xa2cdd545, 0x71a0094f, 0xd51fbc6c, 0x00000000 };
const scalar_t  base_order = { 0xa4234c33, 0x77e70c12, 0x000292fe, 0x00000000, 0x00000000, 0x00000004 };
#endif

#if (ECC_CURVE == NIST_K163)
#define coeff_a  1
    #define cofactor 2
    const gf2elem_t polynomial = { 0x000000c9, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000008 };
    const gf2elem_t coeff_b    = { 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000 };
    const gf2elem_t base_x     = { 0x5c94eee8, 0xde4e6d5e, 0xaa07d793, 0x7bbc11ac, 0xfe13c053, 0x00000002 };
    const gf2elem_t base_y     = { 0xccdaa3d9, 0x0536d538, 0x321f2e80, 0x5d38ff58, 0x89070fb0, 0x00000002 };
    const scalar_t  base_order = { 0x99f8a5ef, 0xa2e0cc0d, 0x00020108, 0x00000000, 0x00000000, 0x00000004 };
#endif
#endif

static void gf2field_set_one(gf2elem_t x)
{
    /* Set first word to one */
    x[0] = 1;
    /* .. and the rest to zero */
    int i;
    for (i = 1; i < BITVEC_NWORDS; ++i)
    {
        x[i] = 0;
    }
}

static int gf2field_is_one(const gf2elem_t x)
{
    /* Check if first word == 1 */
    if (x[0] != 1)
    {
        return 0;
    }
    /* ...and if rest of words == 0 */
    int i;
    for (i = 1; i < BITVEC_NWORDS; ++i)
    {
        if (x[i] != 0)
        {
            break;
        }
    }
    return (i == BITVEC_NWORDS);
}

/* galois field(2^m) addition is modulo 2, so XOR is used instead - 'z := a + b' */
static void gf2field_add(gf2elem_t z, const gf2elem_t x, const gf2elem_t y)
{
    int i;
    for (i = 0; i < BITVEC_NWORDS; ++i)
    {
        z[i] = (x[i] ^ y[i]);
    }
}

/* increment element */
static void gf2field_inc(gf2elem_t x)
{
    x[0] ^= 1;
}


/* field multiplication 'z := (x * y)' */
static void gf2field_mul(gf2elem_t z, const gf2elem_t x, const gf2elem_t y)
{
    int i;
    gf2elem_t tmp;

//  TODO:  assert(z != y);

    bitvec_copy(tmp, x);

    /* LSB set? Then start with x */
    if (bitvec_get_bit(y, 0) != 0)
    {
        bitvec_copy(z, x);
    }
    else /* .. or else start with zero */
    {
        bitvec_set_zero(z);
    }

    /* Then add 2^i * x for the rest */
    for (i = 1; i < CURVE_DEGREE; ++i)
    {
        /* lshift 1 - doubling the value of tmp */
        bitvec_lshift(tmp, tmp, 1);

        /* Modulo reduction polynomial if degree(tmp) > CURVE_DEGREE */
        if (bitvec_get_bit(tmp, CURVE_DEGREE))
        {
            gf2field_add(tmp, tmp, polynomial);
        }

        /* Add 2^i * tmp if this factor in y is non-zero */
        if (bitvec_get_bit(y, i))
        {
            gf2field_add(z, z, tmp);
        }
    }
}

/* field inversion 'z := 1/x' */
static void gf2field_inv(gf2elem_t z, const gf2elem_t x)
{
    gf2elem_t u, v, g, h;
    int i;

    bitvec_copy(u, x);
    bitvec_copy(v, polynomial);
    bitvec_set_zero(g);
    gf2field_set_one(z);

    while (!gf2field_is_one(u))
    {
        i = (bitvec_degree(u) - bitvec_degree(v));

        if (i < 0)
        {
            bitvec_swap(u, v);
            bitvec_swap(g, z);
            i = -i;
        }

        bitvec_lshift(h, v, i);
        gf2field_add(u, u, h);
        bitvec_lshift(h, g, i);
        gf2field_add(z, z, h);
    }
}

static void gf2point_copy(gf2elem_t x1, gf2elem_t y1, const gf2elem_t x2, const gf2elem_t y2)
{
    bitvec_copy(x1, x2);
    bitvec_copy(y1, y2);
}

static void gf2point_set_zero(gf2elem_t x, gf2elem_t y)
{
    bitvec_set_zero(x);
    bitvec_set_zero(y);
}

static int gf2point_is_zero(const gf2elem_t x, const gf2elem_t y)
{
    return (    bitvec_is_zero(x)
                && bitvec_is_zero(y));
}

/* double the point (x,y) */
static void gf2point_double(gf2elem_t x, gf2elem_t y)
{
    /* iff P = O (zero or infinity): 2 * P = P */
    if (bitvec_is_zero(x))
    {
        bitvec_set_zero(y);
    }
    else
    {
        gf2elem_t l;

        gf2field_inv(l, x);
        gf2field_mul(l, l, y);
        gf2field_add(l, l, x);
        gf2field_mul(y, x, x);
        gf2field_mul(x, l, l);

        gf2field_inc(l);
        gf2field_add(x, x, l);
        gf2field_mul(l, l, x);
        gf2field_add(y, y, l);
    }
}


/* add two points together (x1, y1) := (x1, y1) + (x2, y2) */
static void gf2point_add(gf2elem_t x1, gf2elem_t y1, const gf2elem_t x2, const gf2elem_t y2)
{
    if (!gf2point_is_zero(x2, y2))
    {
        if (gf2point_is_zero(x1, y1))
        {
            gf2point_copy(x1, y1, x2, y2);
        }
        else
        {
            if (bitvec_equals(x1, x2))
            {
                if (bitvec_equals(y1, y2))
                {
                    gf2point_double(x1, y1);
                }
                else
                {
                    gf2point_set_zero(x1, y1);
                }
            }
            else
            {
                /* Arithmetic with temporary variables */
                gf2elem_t a, b, c, d;

                gf2field_add(a, y1, y2);
                gf2field_add(b, x1, x2);
                gf2field_inv(c, b);
                gf2field_mul(c, c, a);
                gf2field_mul(d, c, c);
                gf2field_add(d, d, c);
                gf2field_add(d, d, b);

                gf2field_inc(d);
                gf2field_add(x1, x1, d);
                gf2field_mul(a, x1, c);
                gf2field_add(a, a, d);
                gf2field_add(y1, y1, a);
                bitvec_copy(x1, d);
            }
        }
    }
}

/* point multiplication via double-and-add algorithm */
static void gf2point_mul(gf2elem_t x, gf2elem_t y, const scalar_t exp)
{
    gf2elem_t tmpx, tmpy;
    int i;
    int nbits = bitvec_degree(exp);

    gf2point_set_zero(tmpx, tmpy);

    for (i = (nbits - 1); i >= 0; --i)
    {
        gf2point_double(tmpx, tmpy);
        if (bitvec_get_bit(exp, i))
        {
            gf2point_add(tmpx, tmpy, x, y);
        }
    }
    gf2point_copy(x, y, tmpx, tmpy);
}


static int gf2point_on_curve(const gf2elem_t x, const gf2elem_t y)
{
    gf2elem_t a, b;

    if (gf2point_is_zero(x, y))
    {
        return 1;
    }
    else
    {
        gf2field_mul(a, x, x);
        gf2field_mul(b, a, x);
        gf2field_add(a, a, b);
        gf2field_add(a, a, coeff_b);
        gf2field_mul(b, y, y);
        gf2field_add(a, a, b);
        gf2field_mul(b, x, y);
        return bitvec_equals(a, b);
    }
}
#endif //MZI_ECCFIELD_H
