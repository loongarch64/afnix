// ---------------------------------------------------------------------------
// - Keccak.hxx                                                              -
// - afnix:sec module - private keccak hash funnction family                 -
// ---------------------------------------------------------------------------
// - This program is free software;  you can redistribute it  and/or  modify -
// - it provided that this copyright notice is kept intact.                  -
// -                                                                         -
// - This program  is  distributed in  the hope  that it will be useful, but -
// - without  any  warranty;  without  even   the   implied    warranty   of -
// - merchantability or fitness for a particular purpose.  In no event shall -
// - the copyright holder be liable for any  direct, indirect, incidental or -
// - special damages arising in any way out of the use of this software.     -
// ---------------------------------------------------------------------------
// - copyright (c) 1999-2021 amaury darsch                                   -
// ---------------------------------------------------------------------------

#ifndef AFNIX_KECCAK_HXX
#define AFNIX_KECCAK_HXX

#ifndef  AFNIX_CRYPTICS_HXX
#include "Cryptics.hxx"
#endif

namespace afnix {

  // the sha-3 state based on keccak state with a 1600 (5*5*64) bits capacity
  static const long   KCAK_SHA3_SIZE = 5 * 5;
  static const long   KCAK_SHA3_BITS = KCAK_SHA3_SIZE * 64;
  // the number of rounds
  static const long   KCAK_SHA3_RNDS = 24;
  static const t_octa KCAK_SHA3_CSTS[KCAK_SHA3_RNDS] =
    {
     0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808AULL,
     0x8000000080008000ULL, 0x000000000000808BULL, 0x0000000080000001ULL,
     0x8000000080008081ULL, 0x8000000000008009ULL, 0x000000000000008AULL,
     0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000AULL,
     0x000000008000808BULL, 0x800000000000008BULL, 0x8000000000008089ULL,
     0x8000000000008003ULL, 0x8000000000008002ULL, 0x8000000000000080ULL,
     0x000000000000800AULL, 0x800000008000000AULL, 0x8000000080008081ULL,
     0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL
    };

  // the schedule bits rate
  static long kcak_sch_rate (const long bits) {
    return KCAK_SHA3_BITS - (2 * bits);
  }
  // the schedule byte rate
  static long kcak_sch_bsiz (const long bits) {
    return kcak_sch_rate (bits) / 8;
  }
  // the schedule octa rate
  static long kcak_sch_osiz (const long bits) {
    return kcak_sch_rate (bits) / 64;
  }
  
  // the keccak step theta
  static inline void kcak_run_stta (t_octa* A) {
    // the theta xor loop
    t_octa C[5];
    for (long k = 0L; k < 5; k++) {
      C[k] = A[k] ^ A[k + 5] ^ A[k + 10] ^ A[k + 15] ^ A[k + 20];
    }
    // the theta rotation loop
    t_octa D[5];
    D[0] = orotl(C[1], 1) ^ C[4];
    D[1] = orotl(C[2], 1) ^ C[0];
    D[2] = orotl(C[3], 1) ^ C[1];
    D[3] = orotl(C[4], 1) ^ C[2];
    D[4] = orotl(C[0], 1) ^ C[3];
    // the theta state update
    for (long k = 0L; k < 5; k++) {
      A[k]      ^= D[k];
      A[k + 5]  ^= D[k];
      A[k + 10] ^= D[k];
      A[k + 15] ^= D[k];
      A[k + 20] ^= D[k];
    }
  }
  
  // the keccak step rho
  static inline void kcak_run_srho (t_octa* A) {
    A[ 1] = orotl(A[ 1],  1);
    A[ 2] = orotl(A[ 2], 62);
    A[ 3] = orotl(A[ 3], 28);
    A[ 4] = orotl(A[ 4], 27);
    A[ 5] = orotl(A[ 5], 36);
    A[ 6] = orotl(A[ 6], 44);
    A[ 7] = orotl(A[ 7],  6);
    A[ 8] = orotl(A[ 8], 55);
    A[ 9] = orotl(A[ 9], 20);
    A[10] = orotl(A[10],  3);
    A[11] = orotl(A[11], 10);
    A[12] = orotl(A[12], 43);
    A[13] = orotl(A[13], 25);
    A[14] = orotl(A[14], 39);
    A[15] = orotl(A[15], 41);
    A[16] = orotl(A[16], 45);
    A[17] = orotl(A[17], 15);
    A[18] = orotl(A[18], 21);
    A[19] = orotl(A[19],  8);
    A[20] = orotl(A[20], 18);
    A[21] = orotl(A[21],  2);
    A[22] = orotl(A[22], 61);
    A[23] = orotl(A[23], 56);
    A[24] = orotl(A[24], 14);
  }
  
  // the keccak step pi
  static inline void kcak_run_sxpi (t_octa* A) {
    t_octa AX = A[1];
    A[ 1] = A[ 6];
    A[ 6] = A[ 9];
    A[ 9] = A[22];
    A[22] = A[14];
    A[14] = A[20];
    A[20] = A[ 2];
    A[ 2] = A[12];
    A[12] = A[13];
    A[13] = A[19];
    A[19] = A[23];
    A[23] = A[15];
    A[15] = A[ 4];
    A[ 4] = A[24];
    A[24] = A[21];
    A[21] = A[ 8];
    A[ 8] = A[16];
    A[16] = A[ 5];
    A[ 5] = A[ 3];
    A[ 3] = A[18];
    A[18] = A[17];
    A[17] = A[11];
    A[11] = A[ 7];
    A[ 7] = A[10];
    A[10] = AX;
  }
  
  // the keccak step chi
  static inline void kcak_run_schi (t_octa* A) {
    for (long k = 0L; k < 25L; k += 5L) {
      t_octa A0 = A[0 + k];
      t_octa A1 = A[1 + k];
      
      A[0 + k] ^= ~A1 & A[2 + k];
      A[1 + k] ^= ~A[2 + k] & A[3 + k];
      A[2 + k] ^= ~A[3 + k] & A[4 + k];
      A[3 + k] ^= ~A[4 + k] & A0;
      A[4 + k] ^= ~A0 & A1;
    }
  }

  // the keccak step iota
  static inline void kcak_run_iota (const long ridx, t_octa* A) {
    A[0] ^= KCAK_SHA3_CSTS[ridx];
  }
  
  // the keccak round step
  static void kcak_run_rnds (const long ridx, t_octa* A) {
    // run the step theta
    kcak_run_stta (A);
    // run the step rho
    kcak_run_srho (A);
    // run the step pi
    kcak_run_sxpi (A);
    // run the step chi
    kcak_run_schi (A);
    // run the step iota
    kcak_run_iota (ridx, A);
  }
  
  // the keccak permutation loop
  static void kcak_run_loop (t_octa* hsts) {
    // check for null
    if (hsts == nullptr) return;
    // run the round loop
    for (long k = 0L; k < KCAK_SHA3_RNDS; k++) kcak_run_rnds (k, hsts);
  }

  // the keccak initialize loop
  static void kcak_ini_loop (t_octa* hsts, const t_octa* M , const long osiz) {
    for (long k = 0L; k < osiz; k++) hsts[k] ^= M[k];
  }  
}

#endif
