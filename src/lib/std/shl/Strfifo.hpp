// ---------------------------------------------------------------------------
// - Strfifo.hpp                                                             -
// - standard object library - string fifo object class definition           -
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

#ifndef  AFNIX_STRFIFO_HPP
#define  AFNIX_STRFIFO_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Strfifo class is a string fifo class designed to store a large 
  /// number of strings. The fifo is organized as a list of record, which
  /// are part of a hash table. The fifo can operate in a uniq mode where
  /// only one occurence of the string is found, hence the need of a hash
  /// table to ease the string search.
  /// @author amaury darsch

  class Strfifo : public virtual Serial {
  private:
    /// the uniq flag
    bool d_uniq;
    /// the hash structure
    struct s_htbl* p_htbl;
    /// the fifo structure
    struct s_fifo* p_fifo;

  public:
    /// create a string fifo
    Strfifo (void);

    /// create a string fifo with a uniq flag
    /// @param uniq the uniq flag
    Strfifo (const bool uniq);
  
    /// destroy the string fifo 
    ~Strfifo (void);

    /// return the class name
    String repr (void) const;

    /// @return the string fifo did
    t_word getdid (void) const;

    /// @return the string fifo sid
    t_word getsid (void) const;

    /// serialize this string vector to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const;

    /// deserialize a string vector from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is);

    /// reset the string fifo
    void reset (void);

    /// set the uniq flag
    /// @param uniq the uniq flag to set
    void setuniq (const bool uniq);

    /// @return the uniq flag
    bool getuniq (void) const;

    /// @return the string fifo length
    long length (void) const;

    /// @return true if the fifo is empty
    bool empty (void) const;

    /// @return true if the fifo is active
    bool active (void) const;

    /// @return true if the key exists in this fifo
    bool exists (const String& key) const;

    /// add a new string in this fifo
    /// @param key the string to add
    bool add (const String& key);

    /// pop a string from this fifo
    String pop (void);

  private:
    // make the copy constructor private
    Strfifo (const Strfifo&);
    // make the assignment operator private
    Strfifo& operator = (const Strfifo&);    

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
