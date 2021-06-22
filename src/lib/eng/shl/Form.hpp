// ---------------------------------------------------------------------------
// - Form.hpp                                                                -
// - afnix engine - form class definition                                    -
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

#ifndef  AFNIX_FORM_HPP
#define  AFNIX_FORM_HPP

#ifndef  AFNIX_CONS_HPP
#include "Cons.hpp"
#endif

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Form class is a reader object that is derived from a cons cell.
  /// The form stores on top of the cons cell the file name and line number
  /// where it was created. There is nothing fancy except that it is the
  /// primary object returned by the afnix reader.
  /// @author amaury darsch

  class Form : public Cons {
  protected:
    /// the form file name
    String d_name;
    /// the line number
    long d_lnum;

  public:
    /// create a default form
    Form (void);

    /// create a new form with a type
    /// @param cctp the cons cell type
    Form (const t_cctp cctp);

    /// create a new form with a type and line number
    /// @param cctp the cons cell type
    /// @param lnum the form line number
    Form (const t_cctp cctp, const long lnum);

    /// create a form with a car
    /// @param car the car of this cons cell
    Form (Object* car);

    /// create a form with a type and a car
    /// @param cctp the cons cell type
    /// @param car the car of this cons cell
    Form (const t_cctp cctp, Object* car);

    /// copy constructor for this form
    /// @param that the cons to copy
    Form (const Form& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// assign a form to another one
    /// @param that the cons cell to assign
    Form& operator = (const Form& that);

    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const;
    
    /// serialize this form to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const;

    /// deserialize a form from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is);

    /// set the form information
    /// @param name the file name
    /// @param lnum the line number
    void setinfo (const String& name, const long lnum);

    /// set the form file name
    /// @param name the file name to set
    void setname (const String& name);

    /// @return the form file name
    String getname (void) const;

    /// set the form line number
    /// @param lnum the form line number
    void setlnum (const long lnum);

    /// @return the form line number
    long getlnum (void) const;

  public:
    /// evaluate this object within the calling nameset
    /// @param zobj the current evaluable
    /// @param nset the current nameset 
    Object* eval (Evaluable* zobj, Nameset* nset);
  };
}

#endif
