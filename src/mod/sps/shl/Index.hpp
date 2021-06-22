// ---------------------------------------------------------------------------
// - Index.hpp                                                               -
// - afnix:sps module - cell index class definition                          -
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

#ifndef  AFNIX_INDEX_HPP
#define  AFNIX_INDEX_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Index class is a base class that maintain a cell index at the
  /// folio level. A cell index is composed of the sheet index, the record
  /// index and the cell index. The index object can be used to access in
  /// a generic way a particular cell. Additionaly, the folio name can also
  /// be stored in the index. It is possible to have multiple records
  /// that represents the same cell.
  /// @author amaury darsch

  class Index : public virtual Object {
  private:
    /// the index size
    long d_size;
    /// the default position
    long d_dpos;
    /// the index structure
    struct s_indx* p_indx;

  public:
    /// create an empty index
    Index (void);

    /// create an index by coordinates
    /// @param cidx the cell index
    Index (const long cidx);

    /// create an index by coordinates
    /// @param cidx the cell index
    /// @param ridx the record index
    Index (const long cidx, const long ridx);

    /// create an index by coordinates
    /// @param cidx the cell index
    /// @param ridx the record index
    /// @param sidx the sheet index
    Index (const long cidx, const long ridx, const long sidx);

    /// copy construct this index
    /// @param that the index to copy
    Index (const Index& that);

    /// destroy this index
    ~Index (void);

    /// @return the object name
    String repr (void) const;

    /// assign an index to this one
    /// @param that the index to assign
    Index& operator = (const Index& that);
    
    /// reset this index
    void reset (void);

    /// @return the index length
    long length (void) const;

    /// @return true if an index entry exists by coordinates
    virtual bool exists (const long cidx) const;

    /// @return true if an index entry exists by coordinates
    virtual bool exists (const long cidx, const long ridx) const;

    /// @return true if an index entry exists by coordinates
    virtual bool exists (const long cidx, const long ridx, 
			 const long sidx) const;

    /// add an index by coordinates
    /// @param cidx the cell index
    virtual void add (const long cidx);

    /// add an index by coordinates
    /// @param cidx the cell index
    /// @param ridx the record index
    virtual void add (const long cidx, const long ridx);

    /// add an index by coordinates
    /// @param cidx the cell index
    /// @param ridx the record index
    /// @param sidx the sheet index
    virtual void add (const long cidx, const long ridx, const long sidx);

    /// set the default cell index 
    /// @param cidx the cell index to set
    virtual void setcidx (const long cidx);

    /// set the cell index by index
    /// @param ipos the index position
    /// @param cidx the cell index to set
    virtual void setcidx (const long ipos, const long cidx);

    /// updata all cell indexes with a new value
    /// @param cidx the cell index to set
    virtual void updcidx (const long cidx);

    /// @return the default cell index
    virtual long getcidx (void) const;

    /// @return the cell index
    virtual long getcidx (const long ipos) const;

    /// set the default record index
    /// @param ridx the record index to set
    virtual void setridx (const long ridx);

    /// set the record index by position
    /// @param ipos the index position
    /// @param ridx the record index to set
    virtual void setridx (const long ipos, const long ridx);

    /// updata all record indexes with a new value
    /// @param ridx the record index to set
    virtual void updridx (const long ridx);

    /// @return the default record index
    virtual long getridx (void) const;

    /// @return the record index by position
    virtual long getridx (const long ipos) const;

    /// set the default sheet index
    /// @param sidx the record index to set
    virtual void setsidx (const long sidx);

    /// set the sheet index by position
    /// @param ipos the index position
    /// @param sidx the record index to set
    virtual void setsidx (const long ipos, const long sidx);

    /// update all sheet indexes with a new value
    /// @param sidx the sheet index to set
    virtual void updsidx (const long sidx);

    /// @return the default sheet index
    virtual long getsidx (void) const;

    /// @return the sheet index by position
    virtual long getsidx (const long ipos) const;

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
