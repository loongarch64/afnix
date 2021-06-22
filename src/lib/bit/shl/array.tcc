// ---------------------------------------------------------------------------
// - array.tcc                                                               -
// - afnix nuts and bolts - array template                                   -
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

#ifndef  AFNIX_ARRAY_TCC
#define  AFNIX_ARRAY_TCC

namespace afnix {

  /// The array template is a generic resizable container. In its simplest
  /// form the array is null and allocated when an element is inserted. A
  /// requested size can be specified as well.
  /// @author amaury darsch

  template <typename T>
  class t_array {
  protected:
    /// the array size
    long d_size;
    /// the array length
    long d_alen;
    /// the array data
    T* p_data;
    
  public:
    /// create a null array
    t_array (void) {
      d_size = 0L;
      d_alen = 0L;
      p_data = nullptr;
    }

    /// create an array by size
    t_array (const long size) {
      d_size = (size <= 0L) ? 0L : size;
      d_alen = 0L;
      p_data = (d_size == 0L) ? nullptr : new T[d_size];
    }

    /// copy construct this array
    /// @param that the array to copy
    t_array (const t_array<T>& that) {
      d_size = that.d_alen;
      d_alen = that.d_alen;
      p_data = nullptr;
      if (d_size > 0L) {
	p_data = new T[d_size];
	for (long k = 0L; k < d_size; k++) {
	  p_data[k] = that.p_data[k];
	}
      }
    }

    /// copy move this array
    /// @param that the array to copy
    t_array (t_array<T>&& that) noexcept {
      d_size = that.d_size; that.d_size = 0L;
      d_alen = that.d_alen; that.d_alen = 0L;
      p_data = that.p_data; that.p_data = nullptr;
    }

    /// destroy this array
    ~t_array (void) {
      delete [] p_data;
    }

    /// assign an array to this one
    /// @param that the array to assign
    t_array<T>& operator = (const t_array<T>& that) {
      // check for self-assignation
      if (this == &that) return *this;
      // clean old array
      reset ();
      // assign the array
      d_size = that.d_alen;
      d_alen = that.d_alen;
      p_data = nullptr;
      if (d_size > 0L) {
	p_data = new T[d_size];
	for (long k = 0L; k < d_size; k++) {
	  p_data[k] = that.p_data[k];
	}
      }
      return *this;
    }

    /// move an array into this one
    /// @param that the array to copy
    t_array<T>& operator = (t_array<T>&& that) noexcept {
      // check for self move
      if (this == &that) return *this;
      // move the array
      d_size = that.d_size; that.d_size = 0L;
      d_alen = that.d_alen; that.d_alen = 0L;
      p_data = that.p_data; that.p_data = nullptr;
      return *this;
    }

    /// @return a reference by position
    T& operator [] (const long pos) {
      // check for consistency
      if ((pos < 0L) || (pos >= d_alen)) throw "array index out of bound";
      // here it is
      return p_data[pos];
    }
    
    /// @return a reference by position
    const T& operator [] (const long pos) const {
      // check for consistency
      if ((pos < 0L) || (pos >= d_alen)) throw "array index out of bound";
      // here it is
      return p_data[pos];
    }

    /// reset this array
    void reset (void) {
      d_size = 0L;
      d_alen = 0L;
      delete [] p_data; p_data = nullptr;
    }

    /// @return the array length
    long length (void) const {
      return d_alen;
    }
    
    /// resize an array by size
    /// @param size the array size
    void resize (const long size) {
      // a null size resets the array
      if (size == 0L) {
	reset ();
	return;
      }
      // check for consistency
      if ((size <= 0L) || (size <= d_size)) return;
      // allocate a new array and copy
      T* data = new T[size];
      for (long k = 0; k < d_alen; k++) data[k] = p_data[k];
      delete [] p_data; p_data = data; d_size = size;
    }
    
    /// set an array element by position
    /// @param pos the array index
    /// @param val the array value
    void set (const long pos, const T& val) {
      // check for consistency
      if ((pos < 0L) || (pos >= d_size)) throw "array index out of bound";
      // set at position
      p_data[pos] = val;
    }

    /// @return an array value by position
    T get (const long pos) const {
      // check for consistency
      if ((pos < 0L) || (pos >= d_alen)) throw "array index out of bound";
      // get at position
      return p_data[pos];
    }

    /// push an element in the array
    /// @param val the value to push
    void push (const T& val) {
      // check for resize
      if (d_size == 0L) resize (1024L);
      if (d_alen >= d_size) resize (2*d_size);
      // push the value
      p_data[d_alen++] = val;
    }

    /// pop an element from the array
    T pop (void) {
      // check for consistency
      if (d_alen <= 0L) throw "nil array in pop operation";
      // get the value
      T result = p_data[--d_alen];
      return result;
    }

    /// merge an array into this one
    void merge (const t_array<T> tary) {
      // check for nil
      if (tary.d_alen == 0L) return;
      // loop and push
      for (long k = 0L; k < tary.d_alen; k++) push (tary.p_data[k]);
    }
    
    /// @return the data array
    T* getdata (void) {
      return p_data;
    }

    /// @return the data array
    const T* getdata (void) const {
      return p_data;
    }
  };
}

#endif
