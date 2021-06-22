// ---------------------------------------------------------------------------
// - TlsOutput.hpp                                                           -
// - afnix:tls service - tls output stream class definition                  -
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

#ifndef  AFNIX_TLSOUTPUT_HPP
#define  AFNIX_TLSOUTPUT_HPP

#ifndef  AFNIX_TLSPROTO_HPP
#include "TlsProto.hpp"
#endif

#ifndef  AFNIX_OUTPUTSTREAM_HPP
#include "OutputStream.hpp"
#endif

namespace afnix {

  /// The TlsOutput class is the tls output stream which encapsulates
  /// the write tls operations. Since the class binds the tls state and its
  /// associated prototocol, the write operations is performed by encoding
  /// and the tls packet.
  /// @author amaury darsch

  class TlsOutput : public TlsInfos, public OutputStream {
  protected:
    /// the tls state
    TlsState* p_tlss;
    /// the tls proto
    TlsProto* p_tlsp;
    /// the output stream
    mutable OutputStream* p_os;

  public:
    /// create a tls output streams by state
    /// @param os  the output stream
    /// @param sta the tls state
    TlsOutput (OutputStream* os, TlsState* sta);

    /// destroy this tls stream
    ~TlsOutput (void);

    /// @return the class name
    String repr (void) const;

    /// reset this object
    void reset (void);

    /// @return the info as a plist
    Plist getinfo (void) const;

    /// close this output stream
    bool close (void);

    /// write one character on the output stream.
    /// @param value the character to write
    long write (const char value);

    /// write a character string to the output stream
    /// @param value the character string to write
    long write (const char* value);

    /// @return the tls state
    virtual TlsState* getstate (void) const;

  private:
    // make the copy constructor private
    TlsOutput (const TlsOutput&) =delete;
    // make the assignment operator private
    TlsOutput& operator = (const TlsOutput&) =delete;

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
