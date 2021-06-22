// ---------------------------------------------------------------------------
// - cucd.cxx                                                                -
// - standard platform library - unicode database functions implementation   -
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

#include "cucd.hpp"
#include "cthr.hpp"
#include "csys.hpp"
#include "cucd.hxx"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the plane size 0x8000 = 32768
  static const long UCD_DPA_MAX = 65536;
  // the indirect plane size 0x8000 = 32768
  static const long UCD_IPA_MAX = 32768;

  // the indirect plane creation function
  static const s_cucd*** cucd_init_plane (void);
  // the indirect plane access array
  static const s_cucd*** p_ipa = cucd_init_plane ();

  // static mutex creation function
  static void* cucd_umtx_create (void);
  // mutex or network services
  static void* cucd_umtx = cucd_umtx_create ();
  // this function destroy the mutex at exit
  static void cucd_umtx_destroy (void) {
    // destroy each ipa
    if (p_ipa != nullptr) {
      for (long i = 0; i < UCD_IPA_MAX; i++) delete [] p_ipa[i];
    }
    // destroy the indirect array
    delete [] p_ipa;
    // destroy the mutex
    c_mtxdestroy (cucd_umtx);
  }
  // this function initialize a mutex statically
  static void* cucd_umtx_create (void) {
    void* result = c_mtxcreate ();
    c_atexit (cucd_umtx_destroy);
    return result;
  }

  // this function fill a plane by index
  static const s_cucd** cucd_fill_plane (const long index) {
    // initialize the array
    const s_cucd** dpa = new const s_cucd*[UCD_DPA_MAX];
    for (long i = 0; i < UCD_DPA_MAX; i++) dpa[i] = nullptr;
    // get the plane size and data
    const long   psize = c_ucdpsize (index);
    const s_cucd* plane = c_ucdplane (index);
    // initialize the direct access plane
    for (long i = 0; i < psize; i++) {
      long pidx = (long) (plane[i].d_code & 0x0000FFFF);
      dpa[pidx] = &plane[i];
    }
    // here is the plane
    return dpa;
  }

  // this function initialize the plane system
  static const s_cucd*** cucd_init_plane (void) {
    // preset the indirect array
    const s_cucd*** ipa = new const s_cucd**[UCD_IPA_MAX];
    for (long i = 0; i < UCD_IPA_MAX; i++) ipa[i] = nullptr;
    // fill the plane 0
    ipa[0] = cucd_fill_plane (0);
    // here is the initial indirect plane
    return ipa;
  }

  // this function loads a plane by index
  static void cucd_load_plane (const long index) {
    // check the plane index
    if ((index <= 0) || (index >= UCD_IPA_MAX)) return;
    // lock the mutex
    c_mtxlock (cucd_umtx);
    // fill the plane
    if (p_ipa[index] == nullptr) p_ipa[index] = cucd_fill_plane (index);
    // unlock the mutex
    c_mtxunlock (cucd_umtx);
  }

  // fill an array with a canonical decompostion and an index

  static bool cucd_fill_nfd (t_quad dst[UCD_CDV_MAX], long& index, 
			    const t_quad code) {
    // check the index
    if (index >= UCD_CDV_MAX) return false;
    // get the ucd record
    const s_cucd* ucd = c_getcucd (code);
    if (ucd == nullptr) {
      dst[index++] = code;
      return true;
    }
    // check if a canonical decomposition exists
    if (ucd->d_pdmv != UCD_DMV_NIL) {
      dst[index++] = code;
      return true;
    }
    // check if the first decomposition is null - if yes then there is no
    // decomposition and the character is mapped in place
    if (ucd->d_cdmv[0] == nilq) {
      dst[index++] = code;
      return true;
    }
    // here a decomposition exists - the decomposition mapping value is nil
    // so it means that it is a canonical decomposition - let's go with it!
    for (long i = 0; i < UCD_CDV_MAX; i++) {
      // get the decomposition value
      t_quad c = ucd->d_cdmv[i];
      if (c == nilq) break;
      // recursivelly remap it
      cucd_fill_nfd (dst, index, c);
    }
    return true;
  }

  // this procedure find the ccc value for a code point
  static inline long cucd_find_ccc (const t_quad code) {
    // get the ucd record
    const s_cucd* ucd = c_getcucd (code);
    // extract ccc value
    return (ucd == nullptr) ? 0 : ucd->d_pccc;
  }

  // -------------------------------------------------------------------------
  // - ucd section                                                           -
  // -------------------------------------------------------------------------

  // return the ucd plane size by index

  const long c_ucdpsize (const long index) {
    switch (index) {
    case 0x0000: return UCD_PSIZE_0000;
    case 0x0001: return UCD_PSIZE_0001; 
    case 0x0002: return UCD_PSIZE_0002;
    case 0x0003: return UCD_PSIZE_0003;
    case 0x0004: return UCD_PSIZE_0004;
    case 0x0005: return UCD_PSIZE_0005;
    case 0x0006: return UCD_PSIZE_0006;
    case 0x0007: return UCD_PSIZE_0007;
    case 0x0008: return UCD_PSIZE_0008;
    case 0x0009: return UCD_PSIZE_0009;
    case 0x000A: return UCD_PSIZE_000A;
    case 0x000B: return UCD_PSIZE_000B;
    case 0x000C: return UCD_PSIZE_000C;
    case 0x000D: return UCD_PSIZE_000D;
    case 0x000E: return UCD_PSIZE_000E;
    default:
      break;
    }
    return 0;
  }

  // return the ucd plane array by index

  const s_cucd* c_ucdplane (const long index) {
    switch (index) {
    case 0x0000: return UCD_PLANE_0000;
    case 0x0001: return UCD_PLANE_0001;
    case 0x0002: return UCD_PLANE_0002;
    case 0x0003: return UCD_PLANE_0003;
    case 0x0004: return UCD_PLANE_0004;
    case 0x0005: return UCD_PLANE_0005;
    case 0x0006: return UCD_PLANE_0006;
    case 0x0007: return UCD_PLANE_0007;
    case 0x0008: return UCD_PLANE_0008;
    case 0x0009: return UCD_PLANE_0009;
    case 0x000A: return UCD_PLANE_000A;
    case 0x000B: return UCD_PLANE_000B;
    case 0x000C: return UCD_PLANE_000C;
    case 0x000D: return UCD_PLANE_000D;
    case 0x000E: return UCD_PLANE_000E;
    default:
      break;
    }
    return nullptr;
  }

  // return a ucd structure by code point

  const s_cucd* c_getcucd (const t_quad code) {
    // get the plane index
    long plane = code >> 16;
    // check if we are loaded
    cucd_load_plane (plane);
    // get the indirect plane
    if (p_ipa == nullptr) return nullptr;
    const s_cucd** dpa = p_ipa[plane];
    if (dpa == nullptr) return nullptr;
    // get the ucd structure
    long index = (long) (code & 0x0000FFFF);
    return dpa[index];
  }

  // fill an array with a canonical decomposition

  bool c_ucdnfd (t_quad dst[UCD_CDV_MAX], const t_quad code) {
    // initialize the array
    for (long i = 0; i < UCD_CDV_MAX; i++) dst[i] = nilq;
    // get the decomposition
    long index = 0;
    bool result = cucd_fill_nfd (dst, index, code);
    if (result == false) return false;
    // update the result with the ccc coding
    c_ucdcof (dst, UCD_CDV_MAX);
    return true;
  }

  // fill an array with a canonical decomposition - this one is for
  // test purpose only since the array is bounded

  bool c_ucdnfd (t_quad dst[UCD_CDV_MAX], const t_quad src[UCD_CDV_MAX]) {
    // initialize the array
    for (long i = 0; i < UCD_CDV_MAX; i++) dst[i] = nilq;
    // get the decomposition
    long  index = 0;
    bool status = true;
    for (long i = 0; i < UCD_CDV_MAX; i++) {
      t_quad code = src[i];
      if (code == nilq) break;
      status &= cucd_fill_nfd (dst, index, code);
    }		  
    if (status == false) return false;
    // update the result with the ccc coding
    c_ucdcof (dst, UCD_CDV_MAX);
    return true;
  }

  // return a nil allocated string

  t_quad* c_ucdnil (void) {
    t_quad* result = new t_quad[1];
    result[0] = nilq;
    return result;
  }

  // normalize a character buffer into a canonical form

  t_quad* c_ucdnrm (const char* s, const long size) {
    // check for nil first
    if ((s == nullptr) || (size == 0)) return c_ucdnil ();
    // create a temporary buffer that holds the quad representation
    t_quad* buf = new t_quad[size];
    for (long i = 0; i < size; i++) buf[i] = ((t_quad) s[i]) & 0x000000FF;
    try {
      // convert the buffer
      t_quad* result = c_ucdnrm (buf, size);
      // clean and return
      delete [] buf;
      return result;
    } catch (...) {
      delete [] buf;
      throw;
    }
  }

  // normalize a string into a canonical form

  t_quad* c_ucdnrm (const t_quad* s, const long size) {
    if ((s == nullptr) || (size <= 0)) return c_ucdnil ();
    // allocate a buffer of sufficent size an initialize it
    long    len = size * UCD_CDV_MAX + 1;
    t_quad* buf = new t_quad[len];
    for (long i = 0; i < len; i++) buf[i] = nilq;
    // loop in the string and update the buffer
    long pos = 0;
    for (long i = 0; i < size; i++) {
      // get the code and check for nil
      t_quad code = s[i];
      if (code == nilq) {
	buf[pos++] = nilq;
	break;
      }
      // get the character mapping
      t_quad dst[UCD_CDV_MAX];
      if (c_ucdnfd (dst, code) == false) {
	delete [] buf;
	return c_ucdnil ();
      }
      // update the buffer with the mapping
      for (long j = 0; j < UCD_CDV_MAX; j++) {
	t_quad c = dst[j];
	if (c == nilq) break;
	buf[pos++] = c;
      }
    }
    // put the buffer in canonical order
    c_ucdcof (buf, len);
    return buf;
  }

  // put a character array in a canonical order form

  void c_ucdcof (t_quad* buf, const long size) {
    // check for 0 order
    if ((buf == nullptr) || (size == 0)) return;
    // order in place
    for (long i = 1; i < size; i++) {
      // get the code point and exit if null
      t_quad code = buf[i];
      if (code == nilq) break;
      // get the code point ccc - if the ccc is 0 continue
      long ccci = cucd_find_ccc (code);
      if (ccci == 0) continue;
      // find the initial position for swaping - the scan position is 0
      // or the first position with a ccc of 0
      long pos = i;
      for (long j = pos; j >= 0; j--) {
	long cccj = cucd_find_ccc (buf[j]);
	if (cccj == 0) break;
	pos = j;
      }
      // loop from position and eventuall swap if a lower condition is found
      // as a matter of fact we do no swap but rather rotate from left to right
      for (long j = pos; j < i; j++) {
	long cccj = cucd_find_ccc (buf[j]);
	if (ccci < cccj) {
	  for (long k = i; k > j; k--) buf[k] = buf[k-1];
	  buf[j] = code;
	  i = j;
	  break;
	}
      }  
    }
  }

  // convert a unicode character to lower case

  long c_ucdtol (t_quad dst[UCD_LCM_MAX], const t_quad code) {
    // get the ucd record and do nothing if it does not exist
    const s_cucd* ucd = c_getcucd (code);
    if (ucd == nullptr) {
      dst[0] = code;
      return 1;
    }
    // loop in the lower map
    long result = 0;
    for (long i = 0; i < UCD_LCM_MAX; i++) {
      t_quad c = ucd->d_lmap[i];
      if (c == nilq) break;
      dst[i] = c;
      result++;
    }
    // if the result is null just map the existing character
    if (result == 0) dst[0] = code;
    return 1;
  }
  
  // convert a unicode character to upper case
  
  long c_ucdtou (t_quad dst[UCD_UCM_MAX], const t_quad code) {
    // get the ucd record and do nothing if it does not exist
    const s_cucd* ucd = c_getcucd (code);
    if (ucd == nullptr) {
      dst[0] = code;
      return 1;
    }
    // loop in the upper map
    long result = 0;
    for (long i = 0; i < UCD_UCM_MAX; i++) {
      t_quad c = ucd->d_umap[i];
      if (c == nilq) break;
      dst[i] = c;
      result++;
    }
    // if the result is null just map the existing character
    if (result == 0) dst[0] = code;
    return 1;
  }

  // return true if the code point is not combining

  bool c_ucdncc (const t_quad code) {
    // get the ucd record and do nothing if it does not exist
    const s_cucd* ucd = c_getcucd (code);
    if (ucd == nullptr) return false;
    // check for ccc 0
    return (ucd->d_pccc == 0);
  }
};
