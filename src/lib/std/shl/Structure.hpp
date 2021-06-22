// ---------------------------------------------------------------------------
// - Structure.hpp                                                           -
// - standard object library - structure class definition                    -
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

#ifndef  AFNIX_STRUCTURE_HPP
#define  AFNIX_STRUCTURE_HPP

#ifndef  AFNIX_BUFFER_HPP
#include "Buffer.hpp"
#endif

namespace afnix {

  /// The Structure class is a structured buffer those description is given
  /// by a vector of locales. The structure block is initialized by calling
  /// the open method. The close method closes the block with eventually some
  /// byte padding and push the block in the buffer.
  /// @author amaury darsch

  class Structure : public Buffer {
  protected:
    /// the structure descriptor
    struct s_sdsc* p_desc;
    /// the structure block
    struct s_sblk* p_blok;
        
  public:
    /// create a null structure
    Structure (void);

    /// copy move this structure
    /// @param that the object to copy
    Structure (Structure&& that) noexcept;

    /// destroy this structure
    ~Structure (void);
    
    /// move structure to this one
    /// @param that the obejct to move
    Structure& operator = (Structure&& that) noexcept;

    /// @return the class name
    String repr (void) const override;

    /// @return the serial id
    t_word getdid (void) const override;

    /// @return the serial id
    t_word getsid (void) const override;

    /// serialize this character to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const override;

    /// deserialize a character from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is) override;

    /// reset this structure
    void reset (void) override;

    /// @return the structure size
    virtual long getssiz (void) const;

    /// @return the descriptor size 
    virtual long getdsiz (void) const;
    
    /// add a locale definition
    /// @param ldef the locale descriptor
    virtual bool addldef (const String& ldef);

    /// set the structure padding
    /// @param spad the padding to set
    virtual void setspad (const long spad);

    /// @return the locale indexes
    virtual long* getlidx (void) const;
    
    /// open the structure block
    virtual void open (void);

    /// close the structure block
    virtual void close (void);

    /// add an object to the structure
    /// @param xobj the object to add
    virtual bool add (Object* xobj);

    /// get an object by coordinates
    /// @param sidx the structure index
    /// @param didx the descriptor index
    virtual Object* get (const long sidx, const long didx) const;
    
    /// fill a structure by size
    /// @param size the number of records
    virtual bool fill (const long size);
    
  private:
    // make the copy constructor private
    Structure (const Structure&) =delete;
    // make the assignment operator private
    Structure& operator = (const Structure&) =delete;
    
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
