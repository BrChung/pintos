
#ifndef FP_ARITH_H_
#define FP_ARITH_H_

/* Fixed-point real type. */
typedef int Float;

/* Number of bits reserved for the fractional part. */
#define FRACBITS 14

/* Convert a Float to int */
static inline int FP_TO_INT (Float fpr);

/* Convert an int to Float */
static inline Float INT_TO_FP (int);

/* Increment a Float by 1 */
static inline Float FP_INC (Float *);

/* Add two Floats */
static inline Float FP_ADD_FP (Float, Float);

/* Subtract one Float from another */
static inline Float FP_SUB_FP (Float, Float);

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

/* Divide two ints and return the real result as a Float */
static inline Float INT_DIV_INT (int, int);




/* ==================================*/
/* Float functions implementations. */

/* Convert a Float to int */
static inline int
FP_TO_INT (Float fpr) {
   return fpr >> FRACBITS;
}

/* Convert an int to Float */
static inline Float
INT_TO_FP (int i) {
   return i << FRACBITS;
}

/* Increment a Float by 1 */
static inline
Float FP_INC (Float * fpr) {
   (*fpr) += (1 << FRACBITS);
   return *fpr;
}

/* Add two Floats */
static inline
Float FP_ADD_FP (Float fpr_a, Float fpr_b) {
   return fpr_a + fpr_b;
}

/* Subtract one Float from another */
static inline
Float FP_SUB_FP (Float fpr_a, Float fpr_b) {
   return fpr_a - fpr_b;
}

/* Multiply a Float by an int */
static inline
Float FP_MUL_INT (Float fpr_a, int b) {
   return fpr_a * b;
}

/* Divide a Float by an int */
static inline
Float FP_DIV_INT (Float fpr_a, int b) {
   return fpr_a / b;
}

/* Add an int to a Float */
static inline
Float FP_ADD_INT (Float fpr_a, int b) {
   return fpr_a + (b << FRACBITS);
}

/* Subtract an int from a Float */
static inline
Float FP_SUB_INT (Float fpr_a, int b) {
   return fpr_a - (b << FRACBITS);
}

/* Multiply two Floats */
static inline
Float FP_MUL_FP (Float fpr_a, Float fpr_b) {
   int64_t extended_a = fpr_a;
   extended_a *= (int64_t) fpr_b;
   extended_a >>= FRACBITS;
   return (Float) extended_a;
}

/* Divide a Float by another */
static inline
Float FP_DIV_FP (Float fpr_a, Float fpr_b) {
   int64_t extended_a = fpr_a;
   extended_a <<= FRACBITS;
   extended_a /= (int64_t) fpr_b;
   return (Float) extended_a;
}

/* Divide two ints and return the real result as a Float */
static inline
Float INT_DIV_INT (int a, int b) {
   int64_t exta = a;
   exta <<= FRACBITS;
   exta /= b;
   return (Float) exta;
}

#endif
