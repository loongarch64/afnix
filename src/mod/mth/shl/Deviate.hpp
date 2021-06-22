// ---------------------------------------------------------------------------
// - Deviate.hpp                                                             -
// - afnix:mth module - distribution deviate class definition                -
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

#ifndef  AFNIX_DEVIATE_HPP
#define  AFNIX_DEVIATE_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// The Deviate class is a class that implements various algorithms used
  /// to produce distribution deviate to be used as a random source. Note
  /// that the uniform deviate is stricly the builtin random generator which
  /// is included here for the sake of completness. Most of the deviates are
  /// generated in the [0.0 1.0] range.
  /// @author amaury darsch

  class Deviate {
  public:
    /// @return a uniform deviate
    static t_real uniform (void);

    /// get a uniform deviate by mean and deviation
    /// @param mu the distribution mean
    /// @param sg the distribution deviation
    static t_real uniform (const t_real mu, const t_real sg);

    /// @return a normal deviate
    static t_real normal (void);

    /// get a normal deviate by mean and deviation
    /// @param mu the distribution mean
    /// @param sg the distribution deviation
    static t_real normal (const t_real mu, const t_real sg);
  };
}

#endif
