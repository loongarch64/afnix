// ---------------------------------------------------------------------------
// - Stream.hpp                                                              -
// - standard object library - base stream class definition                  -
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

#ifndef  AFNIX_STREAM_HPP
#define  AFNIX_STREAM_HPP

#ifndef  AFNIX_TRANSCODER_HPP
#include "Transcoder.hpp"
#endif

namespace afnix {

  /// The stream class is a base class which defines the stream operating mode.
  /// BY default, the stream operates in byte mode but can be set to operate
  /// in UTF-8 mode when unicode character are used. When operating in byte
  /// mode, the transcoder is used to map a byte to a unicode character. 
  /// When operating in other mode, the stream might read or write several 
  /// bytes before accepting or displaying a character.
  /// @author amaury darsch

  class Stream : public Transcoder {
  protected:
    /// the encoding mode
    Encoding::t_emod d_emod;

  public:
    /// create a default byte stream
    Stream (void);

    /// create a stream by encoding mode
    /// @param emod the system encoding mode to set
    Stream (const Encoding::t_emod emod);

    /// close this stream
    virtual bool close (void);
    
    /// @return the stream descriptor
    virtual int getsid (void) const;

    /// @return the stream encoding mode
    virtual Encoding::t_emod getemod (void) const;

    /// set the stream encoding mode
    /// @param emod the stream encoding mode to set
    virtual void setemod (const Encoding::t_emod emod);

    /// set the stream encoding mode by name
    /// @param mode the coding mode to set
    virtual void setemod (const String& mode);

  public:
    /// evaluate an object data member
    /// @param zobj  zobj the current evaluable
    /// @param nset  the current nameset
    /// @param quark the quark to evaluate
    static Object* meval (Evaluable* zobj, Nameset* nset, const long quark);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments  to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv) override;
  };
}

#endif
