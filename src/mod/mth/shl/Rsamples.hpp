// ---------------------------------------------------------------------------
// - Rsamples.hpp                                                            -
// - afnix:mth module - real array samples class definitions                 -
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

#ifndef  AFNIX_RSAMPLES_HPP
#define  AFNIX_RSAMPLES_HPP

#ifndef  AFNIX_TAGGABLE_HPP
#include "Taggable.hpp"
#endif

#ifndef  AFNIX_PRINTTABLE_HPP
#include "PrintTable.hpp"
#endif
 
namespace afnix {

  /// This Rsamples class is a two dimension array of real samples. Samples
  /// are organized by rows with a predefined column size. The samples can
  /// be added by position or by rows. Initially, samples are marked as NAN.
  /// The array is self-resizable and numerous methods are provided to extract 
  /// any relevant information.
  /// @author amaury darsch

  class Rsamples : public Taggable {    
  protected:
    /// the columns size
    long d_cols;
    /// the rows size
    long d_size;
    /// the used rows
    long d_rows;
    /// the precision size
    long d_psiz;
    /// the scientific flag
    bool d_sflg;
    /// the time stamps
    t_real*  p_time;
    /// the rows arrays
    t_real** p_data;

  public:
    /// create a one column array samples
    Rsamples (void);

    /// create an array sample by columns
    /// @param cols the number of columns
    Rsamples (const long cols);

    /// create an array sample by name, info and columns
    /// @param name the number of columns
    /// @param info the number of columns
    /// @param cols the number of columns
    Rsamples (const String& name, const String& info, const long cols);

    /// copy construct this array
    /// @param that the array to copy
    Rsamples (const Rsamples& that);

    /// destroy this array
    ~Rsamples (void);

    /// assign an array to this one
    /// @param that the array to assign
    Rsamples& operator = (const Rsamples& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const;
    
    /// serialize this object
    /// @param os the output stream
    void wrstream (OutputStream& os) const;

    /// deserialize this object
    /// @param is the input stream
    void rdstream (InputStream& os);

    /// clear this sample array
    virtual void clear (void);

    /// @return the number of rows
    virtual long getrows (void) const;

    /// set the number of columns
    /// @param cols the number of columns
    virtual void setcols (const long cols);
    
    /// @return the number of colums
    virtual long getcols (void) const;

    /// set the number precision
    /// @param psiz the precision to set
    virtual void setpsiz (const long psiz);

    /// @return the number precision
    virtual long getpsiz (void) const;

    /// set the scientific flag
    /// @param sflg the flag to set
    virtual void setsflg (const bool sflg);

    /// @return the scientific flag
    virtual bool getsflg (void) const;

    /// @return true if the samples are stamped
    virtual bool stamped (void) const;

    /// get the time by position
    /// @param row the sample row
    virtual t_real gettime (const long row) const;

    /// set a sample by position and value
    /// @param row the sample row
    /// @param col the sample column
    /// @param val the sample value
    virtual void set (const long row, const long col, const t_real val);

    /// get a sample by position
    /// @param row the sample row
    /// @param col the sample column
    virtual t_real get (const long row, const long col) const;

    /// allocate a new data row
    virtual long newrow (void);

    /// allocate a new data row by time stamp
    /// @param tval the data row time stamp
    virtual long newrow (const t_real tval);

    /// resize the array sample
    /// @param size the new size to allocate
    virtual void resize (const long size);

    /// @return the minimum signed time
    virtual t_real minst (void) const;

    /// @return the maximum signed time
    virtual t_real maxst (void) const;

    /// get the minimum column value
    /// @param col the column to scan
    virtual t_real minsc (const long col) const;

    /// get the maximum column value
    /// @param col the column to scan
    virtual t_real maxsc (const long col) const;

    /// smooth an array sample upto a relative error
    virtual Rsamples* smooth (void) const;

    /// smooth an array sample upto a relative error
    /// @param reps the relative precision
    virtual Rsamples* smooth (const t_real reps) const;

    /// create a print table with the samples
    /// @param tflg the time stamp flag
    virtual PrintTable* toptbl (const bool tflg) const;

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
