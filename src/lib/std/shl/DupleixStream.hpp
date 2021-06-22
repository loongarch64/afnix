// ---------------------------------------------------------------------------
// - DupleixStream.hpp                                                       -
// - standard object library - dupleix stream class definition               -
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

#ifndef  AFNIX_DUPLEIXSTREAM_HPP
#define  AFNIX_DUPLEIXSTREAM_HPP

#ifndef  AFNIX_INPUTBUFFER_HPP
#include "InputBuffer.hpp"
#endif

#ifndef  AFNIX_INPUTTIMEOPUT_HPP
#include "InputTimeout.hpp"
#endif

#ifndef  AFNIX_OUTPUTSTREAM_HPP
#include "OutputStream.hpp"
#endif

namespace afnix {

  /// The DupleixStream class is a class which combines an input stream
  /// with an ouput stream to form a dupleix stream. Since the class share
  /// the same lock, the class is a by default a half-dupleix stream which
  /// can be turned into a full dupleix stream by rebuilding the individual
  /// input and output stream with help methods.
  /// @author amaury darsch

  class DupleixStream : public InputBuffer, public InputTimeout,
			public OutputStream {
  public:
    /// @return the class name
    String repr (void) const override;

    /// @return the input stream channel
    virtual InputStream* getis (void);

    /// @return the output stream channel
    virtual OutputStream* getos (void);
    
  public:
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
                   Vector* argv) override;
  };
}

#endif
