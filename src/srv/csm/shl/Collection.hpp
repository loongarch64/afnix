// ---------------------------------------------------------------------------
// - Collection.hpp                                                          -
// - afnix:csm service - part collection class definition                    -
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

#ifndef  AFNIX_COLLECTION_HPP
#define  AFNIX_COLLECTION_HPP

#ifndef  AFNIX_SET_HPP
#include "Set.hpp"
#endif

#ifndef  AFNIX_PART_HPP
#include "Part.hpp"
#endif

namespace afnix {

  /// The Collection class is an aggregation of parts which can be accessed
  /// by index or key. Since the collection is a set, there is no duplicate
  /// by construction. The collection is also a part. Et voila ...
  /// @author amaury darsch

  class Collection : public Part {
  protected:
    /// the collection set
    Set* p_cset;

  public:
    /// create an default collection
    Collection (void);

    /// create a collection by name
    /// @param name the collection name
    Collection (const String& name);

    /// create a collection by name
    /// @param name the collection name
    /// @param info the collection info
    Collection (const String& name, const String& info);

    /// copy construct this collection
    /// @param that the collection to copy 
    Collection (const Collection& that);

    /// copy move this collection
    /// @param that the object to copy
    Collection (Collection&& that) noexcept;
    
    /// destroy this collection
    ~Collection (void);

    /// assign a collection to this one
    /// @param that the collection to assign
    Collection& operator = (const Collection& that);

    /// move a collection to this one
    /// @param that the object to assign
    Collection& operator = (Collection&& that) noexcept;
    
    /// @return the class name
    String repr (void) const override;

    /// @return the serial did
    t_word getdid (void) const override;

    /// @return the serial sid
    t_word getsid (void) const override;
    
    /// serialize this domain
    /// @param os the output stream
    void wrstream (OutputStream& os) const override;

    /// deserialize this domain
    /// @param is the input stream
    void rdstream (InputStream& os) override;

    /// @return a subpart by name
    /// @param name the subpart name
    Part* subpart (const String& name) const override;
    
    /// reset this collection
    virtual void reset (void);

    /// @return the number of part in the domain
    virtual long length (void) const;

    /// @return true if the domain is empty
    virtual bool empty (void) const;

    /// @return true if a part exists in the set
    virtual bool exists (const Uuid& kid) const;

    /// @return true if a part exists in the set
    virtual bool exists (const String& kid) const;

    /// add a part in this collection
    /// @param part the part to add
    virtual Uuid addpart (Part* part);

    /// set a part in this collection
    /// @param part the part to set
    virtual bool setpart (Part* part);

    /// get a part at position
    /// @param pidx the part position index
    virtual Part* getat (const long pidx) const;

    /// get a part by kid 
    /// @param kid the part kid
    virtual Part* getby (const Uuid& kid) const;

    /// get a part by kid 
    /// @param kid the part kid
    virtual Part* getby (const String& kid) const;

    /// delete a part at position
    /// @param pidx the part position index
    virtual bool delat (const long pidx);

    /// remove a part by uuid
    /// @param kid the part kid to remove
    virtual bool remove (const Uuid& kid);
    
    /// remove a part by string
    /// @param kid the part kid to remove
    virtual bool remove (const String& kid);
    
    /// merge a collection into this one
    /// @param colo the collection object
    virtual void merge (const Collection& colo);
    
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
