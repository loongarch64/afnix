// ---------------------------------------------------------------------------
// - TlsShake.hpp                                                            -
// - afnix:tls service - tls handshake message class definition              -
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

#ifndef  AFNIX_TLSSHAKE_HPP
#define  AFNIX_TLSSHAKE_HPP

#ifndef  AFNIX_ITERABLE_HPP
#include "Iterable.hpp"
#endif

#ifndef  AFNIX_TLSMESSAGE_HPP
#include "TlsMessage.hpp"
#endif

namespace afnix {

  /// The TlsShake class is the handshake class for the tls protocol.
  /// A handshake class is designed to hold one or several message. Since
  /// multiple messages might be present, the class provides a mechanism
  /// to retreive them, one after another.
  /// @author amaury darsch

  class TlsShake : public Iterable, public TlsMessage {
  public:
    /// create an empty handshake
    TlsShake (void);

    /// create a handshake by record
    /// @param rcd the tls record
    TlsShake (TlsRecord* rcs);

    /// create a handshake by version
    /// @param vmaj the major version
    /// @param vmin the minor version
    TlsShake (const t_byte vmaj, const t_byte vmin);

    /// copy construct this handshake
    /// @param that the object to copy
    TlsShake (const TlsShake& that);

    /// assign a record to this one
    /// @param that the record to assign
    TlsShake& operator = (const TlsShake& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// reset this handshake
    void reset (void);

    /// @return the message info as a plist
    Plist getinfo (void) const;

    /// @return a new iterator for this handshake
    Iterator* makeit (void);

    /// add chunk block by type
    /// @param type the chunk type
    /// @param chnk the chunk block
    virtual void add (const t_byte type, const TlsChunk& chnk);

  private:
    // make the vector iterator a friend
    friend class TlsShakeit;

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

  /// The TlsShakeit class is the iterator for the TlsShake class. Such 
  /// iterator is constructed with the "makeit" vector method. The iterator
  /// is reset to the beginning of the message.
  /// @author amaury darsch

  class TlsShakeit : public Iterator {
  private:
    /// the handshake object
    TlsShake* p_hobj;
    /// the record position
    long d_rpos;
    /// the message type
    t_byte d_type;
    /// the message length
    long d_mlen;
    
  public:
    /// create a new iterator from a message
    /// @param hobj the object to iterate
    TlsShakeit (TlsShake* hobj);

    /// destroy this iterator
    ~TlsShakeit (void);

    /// @return the class name
    String repr (void) const;

    /// reset the iterator to the begining
    void begin (void);

    /// reset the iterator to the end
    void end (void);

    /// move the vector iterator to the next position
    void next (void);

    /// move the vector iterator to the previous position
    void prev (void);

    /// @return the object at the current position
    Object* getobj (void) const;

    /// @return true if the iterator is at the end
    bool isend (void) const;

  private:
    // make the copy constructor private
    TlsShakeit (const TlsShakeit&);
    // make the assignment operator private
    TlsShakeit& operator = (const TlsShakeit&);
  };
}

#endif
