#// ---------------------------------------------------------------------------
// - Selector.hpp                                                            -
// - afnix:sio module - i/o select class definition                          -
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

#ifndef  AFNIX_SELECTOR_HPP
#define  AFNIX_SELECTOR_HPP

#ifndef  AFNIX_VECTOR_HPP
#include "Vector.hpp"
#endif

#ifndef  AFNIX_MONITOR_HPP
#include "Monitor.hpp"
#endif

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif

#ifndef  AFNIX_OUTPUTSTREAM_HPP
#include "OutputStream.hpp"
#endif

namespace afnix {

  /// The Selector class is a input/output selector which blocks until one
  /// object change its status. For an input stream object, a status change
  /// indicates that a character can be read. For an output stream, a status
  /// change indicates that a character can be written. When a call to
  /// the 'wait' method succeds, the method returns the first available 
  /// stream. If the 'waitall' method is called, the method returns a 
  /// vector with  all ready steams. The selector can be configured to
  /// operate in marking mode. In such mode, the selector can be marked as 
  /// ready by a thread independently of the bounded streams. This is a
  /// usefull mechanism which can be used to cancel a select loop. The 'mark'
  /// method is designed to mark the selector while the 'ismarked' method
  /// returns true if the stream has been marked.
  /// @author amaury darsch

  class Selector : public Object {
  private:
    /// the input streams vector
    Vector  d_isv;
    /// the output streams vector
    Vector  d_osv;
    /// the private handle
    void*   p_handle;
    /// the marking monitor
    mutable Monitor d_mmon;

  public:
    /// create an empty selector
    Selector (void);

    /// create a selector by mode
    /// @param mflg the marking mode
    Selector (const bool mflg);

    /// destroy this selector
    ~Selector (void);

    /// @return the class name
    String repr (void) const;

    /// add an input stream to the select list
    /// @param is the input stream to add
    void add (InputStream* is);

    /// add an output stream to the select list
    /// @param is the input stream to add
    void add (OutputStream* os);

    /// @return the number of input streams
    long ilength (void) const;

    /// @return the number of output streams
    long olength (void) const;

    /// @return an input stream by index
    InputStream* iget (const long index) const;

    /// @return an output stream by index
    OutputStream* oget (const long index) const;

    /// wait for one stream to be ready
    /// @param tout the timeout before returning
    Stream* wait (const long tout) const;

    /// get all stream that are ready
    /// @param tout the timeout before returning
    Vector* waitall (const long tout) const;

    /// mark a selector
    void mark (void);

    /// @return true if a selector is marked
    bool ismarked (void) const;

  private:
    // make the copy constructor private
    Selector (const Selector&);
    // make the assignment operator private
    Selector& operator = (const Selector&);

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
