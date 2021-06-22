// ---------------------------------------------------------------------------
// - Md2.cpp                                                                 -
// - afnix:sec module - MD2 message digest class implementation              - 
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

#include "Md2.hpp"
#include "Ascii.hpp"
#include "Vector.hpp"
#include "Cryptics.hxx"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // MD2 algorithm constants
  static const char* MD2_ALGO_NAME   = "MD-2";
  static const long  MD2_BMSG_LENGTH = 16;
  static const long  MD2_HASH_LENGTH = 16;

  // MD2 message padding
  const t_byte* PAD[] = {
    (const t_byte*)
    "",
    (const t_byte*)
    "\001",
    (const t_byte*)
    "\002\002",
    (const t_byte*)
    "\003\003\003",
    (const t_byte*)
    "\004\004\004\004",
    (const t_byte*)
    "\005\005\005\005\005",
    (const t_byte*)
    "\006\006\006\006\006\006",
    (const t_byte*)
    "\007\007\007\007\007\007\007",
    (const t_byte*)
    "\010\010\010\010\010\010\010\010",
    (const t_byte*)
    "\011\011\011\011\011\011\011\011\011",
    (const t_byte*)
    "\012\012\012\012\012\012\012\012\012\012",
    (const t_byte*)
    "\013\013\013\013\013\013\013\013\013\013\013",
    (const t_byte*)
    "\014\014\014\014\014\014\014\014\014\014\014\014",
    (const t_byte*)
    "\015\015\015\015\015\015\015\015\015\015\015\015\015",
    (const t_byte*)
    "\016\016\016\016\016\016\016\016\016\016\016\016\016\016",
    (const t_byte*)
    "\017\017\017\017\017\017\017\017\017\017\017\017\017\017\017",
    (const t_byte*)
    "\020\020\020\020\020\020\020\020\020\020\020\020\020\020\020\020"
  };
  
  // MD2 constants
  static t_byte PIC[256] = {
    41,  46,  67,  201, 162, 216, 124, 1,   
    61,  54,  84,  161, 236, 240, 6,   19,
    98,  167, 5,   243, 192, 199, 115, 140,
    152, 147, 43,  217, 188, 76,  130, 202,
    30,  155, 87,  60,  253, 212, 224, 22,
    103, 66,  111, 24,  138, 23,  229, 18,
    190, 78,  196, 214, 218, 158, 222, 73,
    160, 251, 245, 142, 187, 47,  238, 122,
    169, 104, 121, 145, 21,  178, 7,   63,
    148, 194, 16,  137, 11,  34,  95,  33,
    128, 127, 93,  154, 90,  144, 50,  39,
    53,  62,  204, 231, 191, 247, 151, 3,
    255, 25,  48,  179, 72,  165, 181, 209,
    215, 94,  146, 42,  172, 86,  170, 198,
    79,  184, 56,  210, 150, 164, 125, 182,
    118, 252, 107, 226, 156, 116, 4,   241,
    69,  157 ,112, 89,  100, 113, 135, 32,
    134, 91,  207, 101, 230, 45,  168, 2,
    27,  96,  37,  173, 174, 176, 185, 246,
    28,  70,  97,  105, 52,  64,  126, 15,
    85,  71,  163, 35,  221, 81,  175, 58,
    195, 92,  249, 206, 186, 197, 234, 38,
    44,  83,  13,  110, 133, 40,  132, 9,
    211, 223, 205, 244, 65,  129, 77,  82,
    106, 220, 55,  200, 108, 193, 171, 250,
    36,  225, 123, 8,   12,  189, 177, 74,
    120, 136, 149, 139, 227, 99,  232, 109,
    233, 203, 213, 254, 59,  0,   29,  57,
    242, 239, 183, 14,  102, 88,  208, 228,
    166, 119, 114, 248, 235, 117, 75,  10,
    49,  68,  80, 180, 143,  237, 31,  26,
    219, 153, 141, 51,  159, 17,  131, 20
  };
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default hasher (MD-2)

  Md2::Md2 (void) : Hasher (MD2_ALGO_NAME, MD2_BMSG_LENGTH, MD2_HASH_LENGTH) {
    reset ();
  }

  // create a hasher with a result length (MD-2)

  Md2::Md2 (const long rlen) : Hasher (MD2_ALGO_NAME, MD2_BMSG_LENGTH, 
				       MD2_HASH_LENGTH, rlen) {
    reset ();
  }

  // return the class name

  String Md2::repr (void) const {
    return "Md2";
  }

  // reset this MD2 object

  void Md2::reset (void) {
    wrlock ();
    Hasher::reset ();
    for (long i = 0; i < 16; i++) {
      d_state[i] = nilc;
      d_cksum[i] = nilc;
    }
    unlock ();
  }

  // update the hasher state with the buffer data
  
  bool Md2::update (void) {
    wrlock ();
    try {
      // make sure the buffer is full
      if (length () != MD2_BMSG_LENGTH) {
	unlock ();
	return false;
      }
      // initialize working buffer
      t_byte x[48];
      for (long i = 0; i < 16; i++) x[i]    = d_state[i];
      for (long i = 0; i < 16; i++) x[i+16] = p_data[i];
      for (long i = 0; i < 16; i++) x[i+32] = d_state[i] ^ p_data[i];
      
      // block update - 18 rounds
      t_byte t = nilc;
      for (long i = 0; i < 18; i++) {
	for (long j = 0; j < 48; j++) t = x[j] ^= PIC[t];
	t += i;
      }
      
      // state update
      for (long i = 0; i < 16; i++) d_state[i] = x[i];
      
      // checksum update
      t = d_cksum[15];
      for (long i = 0; i < 16; i++) 
	t = d_cksum[i] ^= PIC[p_data[i] ^ t];
      
      // clear the buffer count to indicate processing
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // finish processing by padding the message
  
  void Md2::finish (void) {
    wrlock ();
    try {
      // compute how much padding is needed
      // modulo 16 bytes - add 16 bytes if block is full
      long blen = length () % MD2_BMSG_LENGTH;
      long plen = MD2_BMSG_LENGTH - blen;
      // process with the padding
      process (PAD[plen], plen);
      // add the checksum
      process (d_cksum, MD2_BMSG_LENGTH);
      // update the result array
      for (long i = 0; i < MD2_HASH_LENGTH; i++) p_hash[i] = d_state[i];
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------


  // create a new object in a generic way
  
  Object* Md2::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Md2;
    // check for 1 argument
    if (argc == 1) {
      long rlen = argv->getlong (0);
      return new Md2 (rlen);
    }
    // invalid arguments
    throw Exception ("argument-error", "too many arguments for MD2");
  }
}
