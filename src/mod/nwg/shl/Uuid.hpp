// ---------------------------------------------------------------------------
// - Uuid.hpp                                                                -
// - afnix:nwg module - universal unique id class definition                 -
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

#ifndef  AFNIX_UUID_HPP
#define  AFNIX_UUID_HPP

#ifndef  AFNIX_SERIAL_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Uuid class is a class designed to represent a universal unique id
  /// as defined by RFC 4122. A uuid is a 16 bytes array which is to be
  /// interpreted according to its version. This class provides by default
  /// a uuid version 4, which is a randomly generated uuid.
  /// @author amaury darsch

  class Uuid : public virtual Serial {
  private:
    /// the uuid byte array
    t_byte* p_uuid;

  public:
    /// create a nil uuid
    Uuid (void);

    /// create a uuid by value
    /// @param uuid the uuid value
    Uuid (const String& uuid);
    
    /// copy construct this uuid
    /// @param that the object to copy
    Uuid (const Uuid& that);

    /// move construct this uuid
    /// @param that the object to move
    Uuid (Uuid&& that);

    /// destroy this uuid
    ~Uuid (void);

    /// assign a uuid to this one
    /// @param that the object to assign
    Uuid& operator = (const Uuid& uuid);
    
    /// move a uuid to this one
    /// @param that the object to assign
    Uuid& operator = (Uuid&& uuid);

    /// compare two uuid
    /// @param uuid the uuid to compare
    bool operator == (const Uuid& uuid) const;

    /// compare a uuid with a string uuid
    /// @param suid the string uuid
    bool operator == (const String& suid) const;
    
    /// compare two uuid
    /// @param uuid the uuid to compare
    bool operator != (const Uuid& uuid) const;

    /// compare a uuid with a string uuid
    /// @param suid the string uuid
    bool operator != (const String& suid) const;
    
    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const;
    
    /// serialize this uuid
    /// @param os the output stream
    void wrstream (OutputStream& os) const;

    /// deserialize this uuid
    /// @param is the input stream
    void rdstream (InputStream& os);

    /// reset this uuid
    void reset (void);

    /// @return true if the uuid is null
    bool isnil (void) const;
    
    /// generate a random uuid
    void random (void);

    /// parse a uuid string
    /// @param uuid the uuid value
    void parse (const String& uuid);
    
    /// @return the uuid as a string
    String tostring (void) const;

    /// @return a hash quad of the uuid
    t_quad hashq (void) const;
    
    /// @return a hash octa of the uuid
    t_octa hasho (void) const;
    
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
