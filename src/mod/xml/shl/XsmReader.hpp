// ---------------------------------------------------------------------------
// - XsmReader.hpp                                                           -
// - afnix:xml module - xsm reader class definition                          -
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

#ifndef  AFNIX_XSMREADER_HPP
#define  AFNIX_XSMREADER_HPP

#ifndef  AFNIX_XSMNODE_HPP
#include "XsmNode.hpp"
#endif

namespace afnix {

  /// The XsmReader class is the simple model node reader. The reader
  /// operates with the parse method and return a node or nil if the
  /// end of stream is reached. Unlike the xml reader, this reader does
  /// not build a tree and the tag content is not even parsed. In this
  /// model, the node content is to be interpreted at the user discretion.
  /// @author amaury darsch

  class XsmReader : public Object {
  protected:
    /// the error resume flag
    bool d_erf;
    /// the input stream
    InputStream* p_is;

  public:
    /// create an empty reader
    XsmReader (void);

    /// create a reader with an input stream
    /// @param is the input stream to map
    XsmReader (InputStream* is);

    /// create a reader with a string to parse
    /// @param xval the string to parse
    XsmReader (const String& xval);

    /// create a reader with an input stream
    /// @param is  the input stream to map
    /// @param erf the error resume flag
    XsmReader (InputStream* is, const bool erf);

    /// destroy this reader
    ~XsmReader (void);

    /// @return the class name
    String repr (void) const;

    /// set the error resume flag
    /// @param erf the error flag
    virtual void seterf (const bool erf);

    /// @return the error resume flag
    virtual bool geterf (void) const;

    /// set the reader input stream
    /// @param is the input stream to set
    virtual void setis (InputStream* is);

    /// set the reader input stream by string
    /// @param xval the string to parse
    virtual void setis (const String& xval);

    /// @return the next available node
    /// @param is the input steam to read
    virtual XsmNode* getnode (void);

  private:
    // make the copy constructor private
    XsmReader (const XsmReader&);
    // make the assignment operator private
    XsmReader& operator = (const XsmReader&);

  public:
    /// create an object in a generic way
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
