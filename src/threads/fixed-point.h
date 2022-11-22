
#ifndef FPR_ARITH_H_
#define FPR_ARITH_H_

/* Fixed-point real type. */
typedef int Float;

/* Number of bits reserved for the fractional part. */
#define FRACBITS 14

/* Convert an Float to int */
static inline int FPR_TO_INT (Float fpr);

/* Convert an int to Float */
static inline Float INT_TO_FPR (int);

/* Increment an Float by 1 */
static inline Float FPR_INC (Float *);

/* Add two Floats */
static inline Float FPR_ADD_FPR (Float, Float);

/* Subtract one Float from another */
static inline Float FPR_SUB_FPR (Float, Float);

/* Multiply an Float by an int */
static inline Float FPR_MUL_INT (Float, int);

/* Divide an Float by an int */
static inline Float FPR_DIV_INT (Float, int);

/* Add an int to an Float */
static inline Float FPR_ADD_INT (Float, int);

/* Subtract an int from an Float */
static inline Float FPR_SUB_INT (Float, int);

/* Multiply two Floats */
static inline Float FPR_MUL_FPR (Float, Float);

/* Divide an Float by another */
static inline Float FPR_DIV_FPR (Float, Float);

/* Divide two ints and return the real result as an Float */
static inline Float INT_DIV_INT (int, int);




/* ==================================*/
/* Float functions implementations. */

/* Convert an Float to int */
static inline int
FPR_TO_INT (Float fpr) {
   return fpr >> FRACBITS;
}

/* Convert an int to Float */
static inline Float
INT_TO_FPR (int i) {
   return i << FRACBITS;
}

/* Increment an Float by 1 */
static inline
Float FPR_INC (Float * fpr) {
   (*fpr) += (1 << FRACBITS);
   return *fpr;
}

/* Add two Floats */
static inline
Float FPR_ADD_FPR (Float fpr_a, Float fpr_b) {
   return fpr_a + fpr_b;
}

/* Subtract one Float from another */
static inline
Float FPR_SUB_FPR (Float fpr_a, Float fpr_b) {
   return fpr_a - fpr_b;
}

/* Multiply an Float by an int */
static inline
Float FPR_MUL_INT (Float fpr_a, int b) {
   return fpr_a * b;
}

/* Divide an Float by an int */
static inline
Float FPR_DIV_INT (Float fpr_a, int b) {
   return fpr_a / b;
}

/* Add an int to an Float */
static inline
Float FPR_ADD_INT (Float fpr_a, int b) {
   return fpr_a + (b << FRACBITS);
}

/* Subtract an int from an Float */
static inline
Float FPR_SUB_INT (Float fpr_a, int b) {
   return fpr_a - (b << FRACBITS);
}

/* Multiply two Floats */
static inline
Float FPR_MUL_FPR (Float fpr_a, Float fpr_b) {
   int64_t extended_a = fpr_a;
   extended_a *= (int64_t) fpr_b;
   extended_a >>= FRACBITS;
   return (Float) extended_a;
}

/* Divide an Float by another */
static inline
Float FPR_DIV_FPR (Float fpr_a, Float fpr_b) {
   int64_t extended_a = fpr_a;
   extended_a <<= FRACBITS;
   extended_a /= (int64_t) fpr_b;
   return (Float) extended_a;
}

/* Divide two ints and return the real result as an Float */
static inline
Float INT_DIV_INT (int a, int b) {
   int64_t exta = a;
   exta <<= FRACBITS;
   exta /= b;
   return (Float) exta;
}

#endif
