// ----------------------------------------------------------------------------
// - Bloc.hpp                                                                 -
// - afnix:csm service - plist blob class definition                          -
// ----------------------------------------------------------------------------
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

#ifndef  AFNIX_BLOC_HPP
#define  AFNIX_BLOC_HPP

#ifndef  AFNIX_BLOB_HPP
#include "Blob.hpp"
#endif

#ifndef  AFNIX_CONDITIONAL_HPP
#include "Conditional.hpp"
#endif

namespace afnix {

  /// The Bloc class is a derived class which encapsulates the functionality
  /// of a blob coupled with a plist and a table of conditionals.
  /// @author amaury darsch

  class Bloc : public Blob {
  protected:
    /// the table of conditionals
    HashTable* p_cnds;
    
  public:
    /// create an empty plist blob
    Bloc (void);

    /// create a plist blob by name
    /// @param name the bloc name
    Bloc (const String& name);

    /// create a plist blob by name and info
    /// @param name the bloc name
    /// @param info the bloc info
    Bloc (const String& name, const String& info);

    /// create a plist blob by rid, name and info
    /// @param rid  the registration id
    /// @param name the registration name
    /// @param info the registration info
    Bloc (const String& rid, const String& name, const String& info);
    
    /// copy construct this bloc
    /// @param that the object copy
    Bloc (const Bloc& that);

    /// destroy this bloc
    ~Bloc (void);
    
    /// assign a bloc to this one
    /// @param that the object to assign
    Bloc& operator = (const Bloc& that);

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// @return the serial did
    t_word getdid (void) const override;

    /// @return the serial sid
    t_word getsid (void) const override;
    
    /// serialize this object
    /// @param os the output stream
    void wrstream (OutputStream& os) const override;

    /// deserialize this object
    /// @param is the input stream
    void rdstream (InputStream& os) override;

    /// @return the blob plist
    Plist getplst (void) const override;

    /// @return true if this bloc is swappable
    bool isswap (Part* part) const override;

    /// add a conditional to the blob
    /// @param cond the condtional to add
    virtual void addcond (const Conditional& cond);

    /// get a conditional by name
    /// @param name the conditional name
    virtual Conditional* getcond (const String& name) const;
    
    /// @return a vector of conditionals
    virtual Vector* getcnds (void) const;
    
  public:
    /// create a new object in a generic way
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
