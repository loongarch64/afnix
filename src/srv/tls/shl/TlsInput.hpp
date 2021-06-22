// ---------------------------------------------------------------------------
// - TlsInput.hpp                                                            -
// - afnix:tls service - tls input stream class definition                   -
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

#ifndef  AFNIX_TLSINPUT_HPP
#define  AFNIX_TLSINPUT_HPP

#ifndef  AFNIX_TLSPROTO_HPP
#include "TlsProto.hpp"
#endif

#ifndef  AFNIX_INPUTBUFFER_HPP
#include "InputBuffer.hpp"
#endif

namespace afnix {

  /// The TlsInput class is the tls input stream which encapsulates
  /// the read tls operations. Since the class binds the tls state and its
  /// associated prototocol, the read operations is performed by decoding
  /// and veriying the tls packet.
  /// @author amaury darsch

  class TlsInput : public TlsInfos, public InputBuffer {
  protected:
    /// the tls state
    TlsState* p_tlss;
    /// the tls proto
    TlsProto* p_tlsp;
    /// the input stream
    mutable InputStream* p_is;

  public:
    /// create a tls input streams by state
    /// @param is  the input stream
    /// @param sta the tls state
    TlsInput (InputStream* is, TlsState* sta);

    /// destroy this tls stream
    ~TlsInput (void);

    /// @return the class name
    String repr (void) const;

    /// reset this object
    void reset (void);

    /// @return the info as a plist
    Plist getinfo (void) const;

    /// close this input stream
    bool close (void);
    
    /// @return the stream timeout
    long gettout (void) const;
    
    /// set the stream timeout
    /// @param tout the stream time to set
    void settout (const long tout);

    /// @return true if we are at the eos
    bool iseos (void) const;
    
    /// check if we can read one character
    bool valid (void) const;

    /// @return the next available character
    char read (void);

    /// @return the tls state
    virtual TlsState* getstate (void) const;

  private:
    // make the copy constructor private
    TlsInput (const TlsInput&) =delete;
    // make the assignment operator private
    TlsInput& operator = (const TlsInput&) =delete;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
    
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;
    
    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv);
  };
}

#endif
