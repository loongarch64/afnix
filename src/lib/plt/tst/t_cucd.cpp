// ---------------------------------------------------------------------------
// - t_cucd.cpp                                                              -
// - standard platform library - unicode database tester                     -
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
#include "t_cucd.hxx"

namespace afnix {

  // -------------------------------------------------------------------------
  // - ucd section                                                           -
  // -------------------------------------------------------------------------

  // this procedure check a plane
  static bool ucd_check_plane (const long index) {
    // get the plane size and data
    const long   psize = c_ucdpsize (index);
    const s_cucd* plane = c_ucdplane (index);
    // compute start and end
    t_quad psi = (((t_quad) index) << 16) & 0xFFFF0000;
    t_quad pei = psi + 0x0000FFFF;
    // check plane
    if ((psize == 0) && (plane != nullptr)) return false;
    if ((psize != 0) && (plane == nullptr)) return false;
    for (long i = 0; i < psize; i++) {
      if (plane[i].d_code < psi) {
	return false;
      }
      if (plane[i].d_code > pei) {
	return false;
      }
    }
    return true;
  }

  // this procedure check a plane code point
  static bool ucd_check_point (const long index) {
    for (long i = 0; i < 65536; i++) {
      t_quad code = (t_quad) ((index << 16) + i);
      const s_cucd* ucd = c_getcucd (code);
      if (ucd == nullptr) continue;
      if (ucd->d_code != code) return false;
    }
    return true;
  }

  // ucd test function
  static bool ucd_main_test (void) {
    // check first the version consistency
    if (NFT_MAJOR != UCD_MAJOR) return false;
    if (NFT_MINOR != UCD_MINOR) return false;
    if (NFT_PATCH != UCD_PATCH) return false;
    
    // check consistent UCD_CDV_MAX SIZE/NFT_ELM_MAX
    // this ensure that the array have the same size
    if (UCD_CDV_MAX != NFT_ELM_MAX) return false;
    
    // get the planes and check
    for (long i = 0; i < 15; i++) {
      if (ucd_check_plane (i) == false) return 1;
      if (ucd_check_point (i) == false) return 1;
    }
    
    // check for combining properties (U+00C0 = U+0041 + U+00300)
    if (c_ucdncc (0x00000041) == false) return 1;
    if (c_ucdncc (0x00000300) == true)  return 1;

    // success
    return true;
  }

  // -------------------------------------------------------------------------
  // - nfd section                                                           -
  // -------------------------------------------------------------------------

  // this procedure returns true if a field is a single code point
  static bool nfd_is_single (const t_quad fld[NFT_ELM_MAX]) {
    if (fld[0] == nilq) return false;
    for (long i = 1; i < NFT_ELM_MAX; i++) {
      if (fld[i] != nilq) return false;
    }
    return true;
  }

  // this procedure compare 2 fields
  static bool nfd_cmp_fld (const t_quad dst[UCD_CDV_MAX], 
			   const t_quad fld[UCD_CDV_MAX]) {
    for (long i = 0; i < UCD_CDV_MAX; i++) {
      if (dst[i] != fld[i]) return false;
    }
    return true;
  }

  // this procedure test a nfd transformationd at index 0
  static bool nfd_test_00 (void) {
    // loop in the test data
    for (long i = 0; i < NFT_TEST_SIZE; i++) {
      // get the data sample
      const nft_t* data = &NFT_TEST_DATA[i];
      // check for single code point
      t_quad dst[UCD_CDV_MAX];
      if (nfd_is_single ((*data)[0]) == true) { 
	// get the code point and transform it
	t_quad code = (*data)[0][0];
	c_ucdnfd (dst, code);
	// compare two fields
	if (nfd_cmp_fld (dst, (*data)[2]) == false) {
	  return false;
	}
      }
      // check for a full array
      c_ucdnfd (dst, (*data)[0]);
      // compare two fields
      if (nfd_cmp_fld (dst, (*data)[2]) == false) {
	return false;
      }
    }
    return true;
  }

  // nfd test function
  static bool nfd_main_test (void) {
    // test the single code point first
    if (nfd_test_00 () == false) return false;

    // success
    return true;
  }
}

int main (int, char**) {
  using namespace afnix;

  // ucd test
  if (ucd_main_test () == false) return 1;
  // nfd test
  if (nfd_main_test () == false) return 1;

  // done
  return 0;
}
