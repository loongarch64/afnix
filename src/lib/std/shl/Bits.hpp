// ---------------------------------------------------------------------------
// - Bits.hpp                                                                -
// - standard object library - forward bits definition                       -
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

#ifndef  AFNIX_BITS_HPP
#define  AFNIX_BITS_HPP

namespace afnix {

  /// This file contains the forward declarations of the generic template
  /// object which are embedded into the standard library. The protocol is
  /// to not expose those objects at the interface level.
  /// @author amaury darsch

  /// the array template class
  template <typename> class t_array;
}

#endif
