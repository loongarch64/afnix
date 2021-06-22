// ---------------------------------------------------------------------------
// - InputTimeout.hpp                                                        -
// - standard object library - timeout input stream class definition         -
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

#ifndef  AFNIX_INPUTTIMEOUT_HPP
#define  AFNIX_INPUTTIMEOUT_HPP

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif

namespace afnix {

  /// The InputTimeout class is an abstract class which implements the timeout
  /// portion of the input stream class. The timeout is managed with the help
  /// of an integer value that is used by the derived class to control the 
  /// timeout i/o operations.
  /// @author amaury darsch

  class InputTimeout : public virtual InputStream {
  protected:
    /// the read timeout
    long d_tout;

  public:
    /// create a default input timeout
    InputTimeout (void);

    /// @return the stream timeout
    virtual long gettout (void) const;

    /// set the stream timeout
    /// @param tout the stream time to set
    virtual void settout (const long tout);
  };
}

#endif
