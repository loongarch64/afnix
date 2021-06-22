// ---------------------------------------------------------------------------
// - csio.hpp                                                                -
// - standard platform library - c i/o function definition                   -
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

#ifndef  AFNIX_CSIO_HPP
#define  AFNIX_CSIO_HPP

#ifndef  AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {
  /// This file constains the procedures that handle the basic i/o 
  /// oeprations. The interface is designed to be platform independant
  /// and provides the functions that can operate by name or identifier.
  /// @author amaury darsch

  /// the file information structure
  struct s_finfo {
    /// file size
    t_long  d_size;
    /// the modification time
    t_long  d_mtime;
    /// initialize the structure
    s_finfo (void) {
      d_size  = 0;
      d_mtime = 0;
    }
  };

  /// @return the standard input stream associated with this process
  int c_stdin (void);

  /// @return the standard output stream associated with this process
  int c_stdout (void);

  /// @return the standard error stream associated with this process
  int c_stderr (void);

  /// check if the name is a regular file
  /// @param name the file name
  bool c_isfile (const char* name);

  /// check if the name is a directory
  /// @param name the directory name
  bool c_isdir (const char* name);

  /// open a file for reading. 
  /// @param name the file name
  /// @return a file descriptor or -1 in case of error
  int c_openr (const char* name);

  /// open a file for writing. 
  /// @param name the file name
  /// @param tflag the truncate flag
  /// @param aflag the append flag
  /// @return a file descriptor or -1 in case of error
  int c_openw (const char* name, const bool tflag, const bool aflag);

  /// open a file for reading and writing.
  /// @param name the file name
  /// @param tflag the truncate flag
  /// @param aflag the append flag
  /// @return a file descriptor or -1 in case of error
  int c_openrw (const char* name, const bool tflag, const bool aflag);

  /// open a session file for writing by user and reading for other
  /// @param name the file name
  /// @return a file descriptor or -1 in case of error
  int c_opensd (const char* name);

  /// open a private file for reading/writing by user
  /// @param name the file name
  /// @return a file descriptor or -1 in case of error
  int c_openpd (const char* name);

  /// wait for one character or timeout
  /// @param sid the stream id
  /// @param tout the eventual time out in milliseconds
  /// @return true on success
  bool c_rdwait (const int sid, const long tout);

  /// read n bytes from the specified stream.
  /// @param sid the stream id
  /// @param buffer the character buffer
  /// @param the number of character to read
  /// @return the number of character read or an error code
  t_long c_read (const int sid, char* buffer, const t_long count);

  /// write n bytes to the specified stream. 
  /// @param sid the stream id
  /// @param buffer the character buffer
  /// @param the number of character to write
  /// @return the number of character writen or an error code
  t_long c_write (const int sid, const char* buffer, const t_long count);

  /// duplicate a stream id
  /// @param sid the stream id
  int c_dup (const int sid);
  
  /// close this stream. 
  /// @param sid the stream id
  bool c_close (const int sid);

  /// return the size of a file in bytes
  /// @param  sid the file id
  /// @return the file size or -1 in case of error
  t_long c_fsize (const int sid);

  /// return the file modification time
  /// @param  sid the file id
  /// @return the atc time or -1 in case of error
  t_long c_mtime (const int sid);

  /// get some file information
  /// @param name the file name to query
  s_finfo* c_finfo (const char* name);

  /// position a file at a certain position
  /// @param  sid the file id
  /// @param  pos the file position to go
  /// @return true on success
  bool c_lseek (const int sid, const t_long pos);

  /// lock completly a file or wait
  /// @param sid the file id to lock
  /// @param wlk flag for lock in reading or writing
  bool c_flock (const int sid, const bool wlk);

  /// unlock a file
  /// @param sid the file id to lock
  bool c_funlock (const int sid);
  
  /// move a file in the file system. 
  /// @param name the file name to move
  /// @param path the target path
  /// @return true on success
  bool c_mv (const char* name, const char* path);

  /// remove a file from the file system. 
  /// @param name the file name to remove
  /// @return true on success
  bool c_rm (const char* name);

  /// create a new selector by mode
  /// @param mflg the marking mode
  void* c_shnew (const bool mflg);

  /// free a selector handle
  /// @param handle the selector handle
  void c_shfree (void* handle);

  /// add an input descriptor to the select handle
  /// @param handle the selector handle
  /// @param sid the descriptor to add
  void c_shiadd (void* handle, const int sid);

  /// add an output descriptor to the select handle
  /// @param handle the selector handle
  /// @param sid the descriptor to add
  void c_shoadd (void* handle, const int sid);

  /// wait for a descriptor to be ready
  /// @param handle the selector handle
  /// @param tout the timeout in milliseconds
  long c_shwait (void* handle, const long tout);

  /// mark a selector if possible
  /// @param handle the selector handle
  void c_shmark (void* handle);

  /// return true if a marking descriptor is set
  /// @param handle the selector handle
  bool c_shmtst (void* handle);

  /// return true if an input descriptor is set
  /// @param handle the selector handle
  /// @param sid the descriptor to test
  bool c_shitst (void* handle, const int sid);

  /// return true if an input descriptor is set
  /// @param handle the selector handle
  /// @param sid the descriptor to test
  bool c_shotst (void* handle, const int sid);
}

#endif
