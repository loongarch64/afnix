// ---------------------------------------------------------------------------
// - Literate.hpp                                                            -
// - afnix:txt module - transliterate object class definition                -
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

#ifndef  AFNIX_LITERATE_HPP
#define  AFNIX_LITERATE_HPP

#ifndef  AFNIX_UNIMAPPER_HPP
#include "Unimapper.hpp"
#endif

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif
 
namespace afnix {
  
  /// A Literate object is a special object that permits to perform 
  /// transliteration with an input stream. The transliteration process
  /// is a two steps process. The first step map a character into another
  /// one and the second step escape a character. To do so, the object
  /// maintains two tables. The first table is the mapping table and the
  /// second table is the escape table.
  /// @author amaury darsch

  class Literate : public Object {
  public:
    /// transliteration models
    enum t_model {
      NONE = 0, // no transliteration
      ASTR = 1  // afnix string escape
    };
      
  private:
    /// the escape byte
    char d_escb;
    /// the byte mapping table
    char d_mptb[256];
    /// the byte escape table
    char d_estb[256];
    /// the escape character
    t_quad d_escc;
    /// the character mapping table
    Unimapper d_mptc;
    /// the character escape table
    Unimapper d_estc;

  public:
    /// create a default literate
    Literate (void);

    /// create a literate with an escape byte
    /// @param escb the escape byte to use
    Literate (const char escb);

    /// create a literate with an escape character
    /// @param escc the escape character to use
    Literate (const t_quad escc);

    /// create a literate with a model
    /// @param model the literate model description
    Literate (const t_model model);

    /// @return the class name
    String repr (void) const;

    /// reset this literate
    void reset (void);

    /// set a translation model
    /// @param model the model to set
    void setmodel (const t_model model);

    /// set a mapping byte
    /// @param mapb the mapping byte
    /// @param altb the alternate byte
    void setmapb (const char mapb, const char altb);

    /// @return a mapping byte
    char getmapb (const char mapb) const;

    /// set a mapping character
    /// @param mapc the mapping character
    /// @param altc the alternate character
    void setmapc (const t_quad mapc, const t_quad altc);

    /// @return a mapping character
    t_quad getmapc (const t_quad mapc) const;

    /// set the escape byte
    /// @param escb the escape byte to set
    void setescb (const char escb);

    /// @return the escape byte
    char getescb (void) const;

    /// set the escape character
    /// @param escc the escape character to set
    void setescc (const t_quad escc);

    /// @return the escape character
    t_quad getescc (void) const;

    /// set an escape mapping sequence
    /// @param escb the escaped byte
    /// @param altb the alternate byte
    void setemcb (const char mapb, const char altb);

    /// @return an escaped mapping byte
    char getemcb (const char mapb) const;

    /// set an escape mapping sequence
    /// @param escc the escaped character
    /// @param altc the alternate character
    void setemcc (const t_quad mapc, const t_quad altc);

    /// @return an escaped mapping character
    t_quad getemcc (const t_quad mapc) const;

    /// transliterate a string into another string
    /// @param s the string to transliterate
    String translate (const String& s) const;

    /// @return a literate byte from a stream
    char read (InputStream& is) const;

    /// @return a literate character from a stream
    t_quad getu (InputStream& is) const;

  private:
    // make the copy constructor private
    Literate (const Literate&);
    // make the assignement operator private
    Literate& operator = (const Literate&);

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
