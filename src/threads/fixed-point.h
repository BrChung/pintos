
#ifndef FP_ARITH_H_
#define FP_ARITH_H_

/* Fixed-point real type. */
typedef int Float;

/* Number of bits reserved for the fractional part. */
#define FRACTIONAL_BITS 15

/* Convert a Float to int */
static inline int FP_TO_INT (Float fpr);

/* Convert an int to Float */
static inline Float INT_TO_FP (int);

/* Increment a Float by 1 */
static inline Float FP_INC (Float *);

/* Multiply a Float by an int */
static inline Float FP_MUL_INT (Float, int);

/* Divide a Float by an int */
static inline Float FP_DIV_INT (Float, int);

/* Add an int to a Float */
static inline Float FP_ADD_INT (Float, int);

/* Subtract an int from a Float */
static inline Float FP_SUB_INT (Float, int);

/* Multiply two Floats */
static inline Float FP_MUL_FP (Float, Float);

/* Divide a Float by another */
static inline Float FP_DIV_FP (Float, Float);

static inline int
FP_TO_INT (Float fpr) {
   return fpr >> FRACTIONAL_BITS;
}

static inline Float
INT_TO_FP (int i) {
   return i << FRACTIONAL_BITS;
}

static inline
Float FP_INC (Float * fpr) {
   (*fpr) += (1 << FRACTIONAL_BITS);
   return *fpr;
}

static inline
Float FP_MUL_INT (Float fpr_a, int b) {
   return fpr_a * b;
}

static inline
Float FP_DIV_INT (Float fpr_a, int b) {
   return fpr_a / b;
}

static inline
Float FP_ADD_INT (Float fpr_a, int b) {
   return fpr_a + (b << FRACTIONAL_BITS);
}

static inline
Float FP_SUB_INT (Float fpr_a, int b) {
   return fpr_a - (b << FRACTIONAL_BITS);
}

static inline
Float FP_MUL_FP (Float fpr_a, Float fpr_b) {
   int64_t extended_a = fpr_a;
   extended_a *= (int64_t) fpr_b;
   extended_a >>= FRACTIONAL_BITS;
   return (Float) extended_a;
}

static inline
Float FP_DIV_FP (Float fpr_a, Float fpr_b) {
   int64_t extended_a = fpr_a;
   extended_a <<= FRACTIONAL_BITS;
   extended_a /= (int64_t) fpr_b;
   return (Float) extended_a;
}

#endif
