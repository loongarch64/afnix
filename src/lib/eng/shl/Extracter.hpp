// ---------------------------------------------------------------------------
// - Extracter.hpp                                                           -
// - afnix engine - form extracter class definition                          -
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

#ifndef  AFNIX_EXTRACTER_HPP
#define  AFNIX_EXTRACTER_HPP

#ifndef  AFNIX_FORMER_HPP
#include "Former.hpp"
#endif

namespace afnix {

  /// The Extracter class is a former object that is suitable for 
  /// previously compiled file. The class deserialize the input stream
  /// and return a cons cell for the engine.
  /// @author amaury darsch

  class Extracter : public Former {
  private:
    /// the input stream
    InputStream* p_is;

  public:
    /// create a default extracter
    Extracter (void);

    /// create a new extracter with a stream
    /// @param is the input stream
    Extracter (InputStream* is);

    /// destroy this extracter
    ~Extracter (void);

    /// @return the class name
    String repr (void) const;

    /// @return a form from this reader or nil in case of eos
    Form* parse (void);

    /// @return an approximate line number for this reader
    long getlnum (void) const;

  private:
    // make the copy constructor private
    Extracter (const Extracter&);
    // make the assignement operator private
    Extracter& operator = (const Extracter&);
  };
}

#endif
