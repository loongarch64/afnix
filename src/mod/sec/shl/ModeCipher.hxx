// ---------------------------------------------------------------------------
// - ModeCipher.hxx                                                         -
// - afnix:sec module - block cipher modes                                   -
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

#include "Galois.hpp"
#include "System.hpp"
#include "Exception.hpp"
#include "ModeCipher.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - block padding                                                         -
  // -------------------------------------------------------------------------

  // this procedure computes a block padding: bit padding
  static bool bc_pad_bitm (t_byte* bi, const long bisz, const long size) {
    // compute the padding count
    long pcnt = bisz - size;
    if (pcnt > 0) {
      // pad the first byte
      bi[size] = 0x80;
      // pad with the serie 80 00 00 00 ...
      for (long i = size+1; i < bisz; i++) bi[i] = 0x00;
      // mark as padded
      return true;
    }
    return false;
  }

  // this procedure compute the next block padding: bit padding
  static bool bc_nxp_bitm (t_byte* bi, const long bisz) {
    // pad the first byte
    bi[0] = 0x80;
    // pad the rest of the block
    for (long i = 1; i < bisz; i++) bi[i] = 0x00;
    // mark as padded
    return true;
  }

  // this procedure computes the number of valid bytes: bit padding
  static long bc_unp_bitm (const t_byte* bo, const long bosz) {
    long pidx = 0;
    for (long i = bosz-1; i >= 0; i--) {
      if (bo[i] == 0x80) {
	pidx = i;
	break;
      }
      if (bo[i] != 0x00) {
	throw Exception ("cipher-error", "invalid padded block");
      }
    }
    // check padded index
    if (bo[pidx] != 0x80) {
      throw Exception ("cipher-error", "invalid padded block");
    }
    // the size if the previous byte index + 1, so is pidx
    return pidx;
  }

  // this procedure computes a block padding: ANSI X.923
  static bool bc_pad_ansi (t_byte* bi, const long bisz, const long size) {
    // compute the padding count
    long pcnt = bisz - size;
    // check the padding
    if (pcnt > 256) {
      throw Exception ("cipher-error", "padding size is too long");
    }
    if (pcnt > 0) {
      // pad with the pad size
      for (long i = size; i < bisz-1; i++) bi[i] = 0x00;
      bi[bisz-1] = (t_byte) pcnt;
      // mark as padded
      return true;
    }
    return false;
  }

  // this procedure computes the next block padding: ANSI X.923
  static bool bc_nxp_ansi (t_byte* bi, const long bisz) {
    // check block size
    if (bisz > 256) {
      throw Exception ("cipher-error", "padding size is too long");
    }
    // pad with the block size
    for (long i = 0; i < bisz-1; i++) bi[i] = 0x00;
    bi[bisz-1] = (t_byte) bisz;
    // mark as padded
    return true;
  }

  // this procedure computes the number of valid bytes: ANSI X.923
  static long bc_unp_ansi (const t_byte* bo, const long bosz) {
    // initialize result
    long result = bosz;
    // extract valid bytes
    long pad = bo[bosz-1];
    if (pad > bosz) {
      throw Exception ("cipher-error", "block padding is too long");	
    }
    // update counter
    result -= pad;
    // number of valid bytes
    return result;
  }

  // this procedure computes a block padding: NIST 800-38A
  static bool bc_pad_nist (t_byte* bi, const long bisz, const long size) {
    // compute the padding count
    long pcnt = bisz - size;
    // check the padding
    if (pcnt > 256) {
      throw Exception ("cipher-error", "padding size is too long");
    }
    if (pcnt > 0) {
      // pad with the pad size
      for (long i = size; i <= bisz; i++) bi[i] = (t_byte) pcnt;
      // mark as padded
      return true;
    }
    return false;
  }

  // this procedure computes the next block padding: NIST 800-38A
  static bool bc_nxp_nist (t_byte* bi, const long bisz) {
    // check block size
    if (bisz > 256) {
      throw Exception ("cipher-error", "padding size is too long");
    }
    // pad with the block size
    for (long i = 0; i < bisz; i++) bi[i] = (t_byte) bisz;
    // mark as padded
    return true;
  }

  // this procedure computes the number of valid bytes: NIST 800-38A
  static long bc_unp_nist (const t_byte* bo, const long bosz) {
    // initialize result
    long result = bosz;
    // extract valid bytes
    long pad = bo[bosz-1];
    if (pad > bosz) {
      throw Exception ("cipher-error", "block padding is too long");	
    }
    // update counter
    result -= pad;
    // number of valid bytes
    return result;
  }

  // this procedure computes a block padding by type
  static bool bc_pad (const ModeCipher::t_pmod pmod,
		      t_byte* bi, const long bisz, const long size) {
    // by default do not pad
    bool result = false;
    // check for valid block
    if ((bi == nullptr) || (bisz < 0)) return result;
    // select from the padding mode
    switch (pmod) {
    case ModeCipher::PMOD_BITM:
      result = bc_pad_bitm (bi, bisz, size);
      break;
    case ModeCipher::PMOD_X923:
      result = bc_pad_ansi (bi, bisz, size);
      break;
    case ModeCipher::PMOD_N800:
      result = bc_pad_nist (bi, bisz, size);
      break;
    default:
      result = false;
      break;
    }
    return result;
  }
  
  // this procedure computes the next block padding by type
  static bool bc_nxp (const ModeCipher::t_pmod pmod,
		      t_byte* bi, const long bisz) {
    // by default do not pad
    bool result = false;
    // check for valid block
    if ((bi == nullptr) || (bisz < 0)) return result;
    // select from the padding mode
    switch (pmod) {
    case ModeCipher::PMOD_BITM:
      result = bc_nxp_bitm (bi, bisz);
      break;
    case ModeCipher::PMOD_X923:
      result = bc_nxp_ansi (bi, bisz);
      break;
    case ModeCipher::PMOD_N800:
      result = bc_nxp_nist (bi, bisz);
      break;
    default:
      result = false;
      break;
    }
    return result;
  }

  // this procedure unpad a block by type
  static long bc_unp (const ModeCipher::t_pmod pmod,
		      t_byte* bo, const long bosz) {
    // by default do not pad
    long result = 0;
    // check for valid block
    if ((bo == nullptr) || (bosz < 0)) return result;
    // select from the padding mode
    switch (pmod) {
    case ModeCipher::PMOD_NONE:
      result = bosz;
      break;
    case ModeCipher::PMOD_BITM:
      result = bc_unp_bitm (bo, bosz);
      break;
    case ModeCipher::PMOD_X923:
      result = bc_unp_ansi (bo, bosz);
      break;
    case ModeCipher::PMOD_N800:
      result = bc_unp_nist (bo, bosz);
      break;
    }
    return result;
  }
  
  // -------------------------------------------------------------------------
  // - block modes                                                           -
  // -------------------------------------------------------------------------

  // this procedure preset the initial vector
  static t_byte* bc_piv (const long ivsz) {
    if (ivsz <= 0) return nullptr;
    t_byte* iv = new t_byte[ivsz];
    for (long i = 0; i < ivsz; i++) iv[i] = Utility::byternd ();
    return iv;
  }

  // this procedure presets the last block
  static void bc_pbl (const ModeCipher::t_cmod cmod, t_byte* bl,
		      const t_byte* iv, const long cbsz, const long ivsz) {
    // basic checks
    if ((bl == nullptr) || (iv == nullptr)) return;
    if ((cbsz <= 0L) || (ivsz <= 0L)) return;
    // check for cbc mode
    if (cmod == ModeCipher::CMOD_CBCM) {
      if (ivsz != cbsz) {
	throw Exception ("cipher-erro", "invalid cbc iv size");
      }
      for (long i = 0; i < cbsz; i++) bl[i] = iv[i];
    }
    // check for ebc mode
    if (cmod == ModeCipher::CMOD_EBCM) {
      if (ivsz != cbsz) {
	throw Exception ("cipher-erro", "invalid ebc iv size");
      }
      for (long i = 0; i < cbsz; i++) bl[i] = iv[i];
    }
    // check for cfb mode
    if (cmod == ModeCipher::CMOD_CFBM) {
      if (ivsz != cbsz) {
	throw Exception ("cipher-erro", "invalid cfb iv size");
      }
      for (long i = 0; i < cbsz; i++) bl[i] = iv[i];
    }
    // check for ofb mode
    if (cmod == ModeCipher::CMOD_OFBM) {
      if (ivsz != cbsz) {
	throw Exception ("cipher-erro", "invalid ofb iv size");
      }
      for (long i = 0; i < cbsz; i++) bl[i] = iv[i];
    }
  }

  // this procedure preprocess a block in ecb mode
  static void bc_pbi_ecb (const bool eflg,
			  t_byte* bt, t_byte* bi, t_byte* bl, const long cbsz) {
    if (eflg == true) {
      for (long i = 0; i < cbsz; i++) bt[i] = bi[i];
    } else {
      for (long i = 0; i < cbsz; i++) bt[i] = bi[i];
    }      
  }

  // this procedure preprocess a block in cbc mode
  static void bc_pbi_cbc (const bool eflg,
			  t_byte* bt, t_byte* bi, t_byte* bl, const long cbsz) {
    if (eflg == true) {
      for (long i = 0; i < cbsz; i++) bt[i] = bi[i] ^ bl[i];
    } else {
      for (long i = 0; i < cbsz; i++) bt[i] = bi[i];
    }
  }

  // this procedure postprocess a block in cbc mode
  static void bc_pbo_cbc (const bool eflg,
			  t_byte* bo, t_byte* bi, t_byte* bl, const long cbsz) {
    if (eflg == true) {
      for (long i = 0; i < cbsz; i++) bl[i] = bo[i];
    } else {
      for (long i = 0; i < cbsz; i++) {
	bo[i] ^= bl[i];
	bl[i]  = bi[i];
      }
    }
  }

  // this procedure preprocess a block in ebc mode
  static void bc_pbi_ebc (const bool eflg,
			  t_byte* bt, t_byte* bi, t_byte* bl, const long cbsz) {
    if (eflg == true) {
      for (long i = 0; i < cbsz; i++) bt[i] = bi[i] ^ bl[i];
    } else {
      for (long i = 0; i < cbsz; i++) bt[i] = bi[i];
    }
  }

  // this procedure postprocess a block in ebc mode
  static void bc_pbo_ebc (const bool eflg,
			  t_byte* bo, t_byte* bi, t_byte* bl, const long cbsz) {
    if (eflg == true) {
      for (long i = 0; i < cbsz; i++) bl[i] = bo[i];
    } else {
      for (long i = 0; i < cbsz; i++) {
	bo[i] ^= bl[i];
	bl[i]  = bi[i];
      }
    }
  }

  // this procedure preprocess a block in cfb mode
  static void bc_pbi_cfb (const bool eflg,
			  t_byte* bt, t_byte* bi, t_byte* bl, const long cbsz) {
    if (eflg == true) {
      for (long i = 0; i < cbsz; i++) bt[i] = bl[i];
    } else {
      for (long i = 0; i < cbsz; i++) bt[i] = bl[i];
    }
  }

  // this procedure postprocess a block in cfb mode
  static void bc_pbo_cfb (const bool eflg,
			  t_byte* bo, t_byte* bi, t_byte* bl, const long cbsz) {
    if (eflg == true) {
      for (long i = 0; i < cbsz; i++) {
	bo[i] ^= bi[i];
	bl[i]  = bo[i];
      }
    } else {
      for (long i = 0; i < cbsz; i++) {
	bo[i] ^= bi[i];
	bl[i]  = bi[i];
      }
    }
  }

  // this procedure preprocess a block in ofb mode
  static void bc_pbi_ofb (const bool eflg,
			  t_byte* bt, t_byte* bi, t_byte* bl, const long cbsz) {
    if (eflg == true) {
      for (long i = 0; i < cbsz; i++) bt[i] = bl[i];
    } else {
      for (long i = 0; i < cbsz; i++) bt[i] = bl[i];
    }
  }

  // this procedure postprocess a block in ofb mode
  static void bc_pbo_ofb (const bool eflg,
			  t_byte* bo, t_byte* bi, t_byte* bl, const long cbsz) {
    if (eflg == true) {
      for (long i = 0; i < cbsz; i++) {
	bl[i]  = bo[i];
	bo[i] ^= bi[i];
      }
    } else {
      for (long i = 0; i < cbsz; i++) {
	bl[i]  = bo[i];
	bo[i] ^= bi[i];
      }
    }
  }

  // this procedure preprocess a block cipher mode
  static void bc_pbi (const ModeCipher::t_cmod cmod, const bool eflg,
		      t_byte* bt, t_byte* bi, t_byte* bl, const long cbsz) {
    // check for valid block
    if ((bt == nullptr) || (bi == nullptr) ||
	(bl == nullptr) || (cbsz < 0)) return;
    // process the block mode
    switch (cmod) {
    case ModeCipher::CMOD_ECBM:
      bc_pbi_ecb (eflg, bt, bi, bl, cbsz);
      break;
    case ModeCipher::CMOD_CBCM:
      bc_pbi_cbc (eflg, bt, bi, bl, cbsz);
      break;
    case ModeCipher::CMOD_EBCM:
      bc_pbi_ebc (eflg, bt, bi, bl, cbsz);
      break;
    case ModeCipher::CMOD_CFBM:
      bc_pbi_cfb (eflg, bt, bi, bl, cbsz);
      break;
    case ModeCipher::CMOD_OFBM:
      bc_pbi_ofb (eflg, bt, bi, bl, cbsz);
      break;
    }
  }

  // this procedure postprocess a block cipher mode
  static void bc_pbo (const ModeCipher::t_cmod cmod, const bool eflg,
		      t_byte* bo, t_byte* bi, t_byte* bl, const long cbsz) {
    // check for valid block
    if ((bo == nullptr) || (bi == nullptr) ||
	(bl == nullptr) || (cbsz < 0)) return;
    // process the block mode
    switch (cmod) {
    case ModeCipher::CMOD_ECBM:
      break;
    case ModeCipher::CMOD_CBCM:
      bc_pbo_cbc (eflg, bo, bi, bl, cbsz);
      break;
    case ModeCipher::CMOD_EBCM:
      bc_pbo_ebc (eflg, bo, bi, bl, cbsz);
      break;
    case ModeCipher::CMOD_CFBM:
      bc_pbo_cfb (eflg, bo, bi, bl, cbsz);
      break;
    case ModeCipher::CMOD_OFBM:
      bc_pbo_ofb (eflg, bo, bi, bl, cbsz);
      break;
    }
  }

  // this procedure returns true for mode encoding
  static bool bc_bme (const ModeCipher::t_cmod cmod) {
    bool result = true;
    switch (cmod) {
    case ModeCipher::CMOD_ECBM:
    case ModeCipher::CMOD_CBCM:
    case ModeCipher::CMOD_EBCM:
    case ModeCipher::CMOD_OFBM:
    case ModeCipher::CMOD_CFBM:
      break;
    }
    return result;
  }

  // this procedure returns true for mode decoding
  static bool bc_bmd (const ModeCipher::t_cmod cmod) {
    bool result = true;
    switch (cmod) {
    case ModeCipher::CMOD_ECBM:
    case ModeCipher::CMOD_CBCM:
    case ModeCipher::CMOD_EBCM:
      break;
    case ModeCipher::CMOD_OFBM:
    case ModeCipher::CMOD_CFBM:
      result = false;
      break;
    }
    return result;
  }
}
