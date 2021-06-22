// ---------------------------------------------------------------------------
// - Whatis.hpp                                                              -
// - afnix:csm service - blob information class definition                   -
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

#ifndef  AFNIX_WHATIS_HPP
#define  AFNIX_WHATIS_HPP

#ifndef  AFNIX_UUID_HPP
#include "Uuid.hpp"
#endif

#ifndef  AFNIX_PLIST_HPP
#include "Plist.hpp"
#endif

namespace afnix {

  /// The Whatis class is an informative class designed to report information
  /// about a blob since a blob can be quite large. The blob information
  /// is particularly useful with serialized domain as it avoid the
  /// serialization of the blob content. This class is similar to the domain
  /// whois class.
  /// @author amaury darsch

  class Whatis : public Taggable {
  protected:
    /// the blob rid
    String d_rid;
    /// the blob kid
    Uuid d_kid;
    /// the blob type
    String d_type;
    /// the creation time
    t_long d_ctim;
    /// the modification time
    t_long d_mtim;

  public:
    /// create a default domain whatis
    Whatis (void);
    
    /// copy construct this domain whatis 
    /// @param that the object to copy 
    Whatis (const Whatis& that);
    
    /// assign a domain whatis to this one
    /// @param that the object to assign
    Whatis& operator = (const Whatis& that);

    /// @return the class name
    String repr (void) const override;

    /// @return the serial did
    t_word getdid (void) const override;

    /// @return the serial sid
    t_word getsid (void) const override;
    
    /// serialize this domain whatis
    /// @param os the output stream
    void wrstream (OutputStream& os) const override;

    /// deserialize this domain whatis
    /// @param is the input stream
    void rdstream (InputStream& os) override;

    /// @return true if the object is valid
    virtual bool valid (void) const;
    
    /// @return the blob rid
    virtual String getrid (void) const;

    /// @return the blob kid
    virtual Uuid getkid (void) const;

    /// @return the blob type
    virtual String gettype (void) const;

    /// @return the creation time
    virtual t_long getctim (void) const;

    /// @return the modification time
    virtual t_long getmtim (void) const;

    /// @return the whatis plist
    virtual Plist getplst (void) const;
    
  protected:
    // make the blob class a friend
    friend class Blob;
    
  public:
    /// create a object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

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
