# ----------------------------------------------------------------------------
# - std-adp-texut                                                            -
# - afnix:adp latex utilities classes module                                 -
# ----------------------------------------------------------------------------
# - This program is  free software;  you can  redistribute it and/or  modify -
# - it provided that this copyright notice is kept intact.                   -
# -                                                                          -
# - This  program  is  distributed in the hope  that it  will be useful, but -
# - without  any   warranty;  without  even   the   implied    warranty   of -
# - merchantability  or fitness for a particular purpose. In not event shall -
# - the copyright holder be  liable for  any direct, indirect, incidental or -
# - special damages arising in any way out of the use of this software.      -
# ----------------------------------------------------------------------------
# - copyright (c) 1999-2021 amaury darsch                                    -
# ----------------------------------------------------------------------------

# ----------------------------------------------------------------------------
# - function section                                                         -
# ----------------------------------------------------------------------------

# fix a text string as a text string
# @param text the text to fix

const afnix:adp:text-to-tex (text) {
  # fix the less character
  const rl (Regex "$<")
  trans ls (rl:replace text "{\\textless}")
  # fix the greater character
  const rg (Regex "$>")
  trans gs (rg:replace ls "{\\textgreater}")  
  # fix the underscore character
  const ru (Regex "_")
  trans ut (ru:replace gs "\\_")
  # fix the sharp character
  const rs (Regex "\\?$#")
  trans us (rs:replace ut "\\#")
  # fix the caret character
  const rc (Regex "$^")
  trans cs (rc:replace us "\\^")
  # fix the dollar character
  const rd (Regex "$$")
  trans ds (rd:replace cs "\\$")
  # fix the esperluette character
  const re (Regex "$&")
  trans es (re:replace ds "\\&")
  # fix the percent character
  const rp (Regex "$%")
  trans ps (rp:replace es "\\%")
  # fix the sigma character
  const ra (Regex (String 'U+3A3'))
  trans as (ra:replace ps "$\Sigma$")
  # fix the left guillemets
  const gl (Regex "«")
  trans lt (gl:replace as "\\guillemotleft")
  const gr (Regex "»")
  gr:replace lt "\\guillemotright"
}

# ----------------------------------------------------------------------------
# - simple command section                                                   -
# ----------------------------------------------------------------------------

# the latex simple command class
const afnix:adp:texsc (class)

# preset the simple command
# @param name the command name

trans afnix:adp:texsc:preset (name) {
  const this:cnam (name:clone)
  trans this:cdat (String)
}

# set the command data
# @param text the command data text

trans afnix:adp:texsc:set-data (text) {
  trans this:cdat (text:clone)
}

# write the simple command to an output stream
# @param os the output stream to write

trans afnix:adp:texsc:write (os) {
  # write the command if any
  if (not (this:cnam:nil-p)) {
    os:newline
    os:write '\\'
    os:write this:cnam
  }
  # write the command data
  if (not (this:cdat:nil-p)) {
    os:write ' '
    os:write this:cdat
  }
  os:newline
}

# ----------------------------------------------------------------------------
# - complex command section                                                   -
# ----------------------------------------------------------------------------

# the latex complex command class
const afnix:adp:texcc (class)

# preset the complex command
# @param name the command name

trans afnix:adp:texcc:preset (head name) {
  const this:head (head:clone)
  const this:cnam (name:clone)
  trans this:cdat (String)
}

# set the command data
# @param text the command data text

trans afnix:adp:texcc:set-data (text) {
  trans this:cdat (text:clone)
}

# write the simple command to an output stream
# @param os the output stream to write

trans afnix:adp:texcc:write (os) {
  # write the command if any
  if (and (not (this:head:nil-p)) (not (this:cnam:nil-p))) {
    os:newline
    os:write '\\'
    os:write this:head
    os:write '{'
    os:write this:cnam
    os:write '}'
  }
  # write the command data
  if (not (this:cdat:nil-p)) {
    os:write '{'
    os:write this:cdat
    os:write '}'
  }
  os:newline
}

# ----------------------------------------------------------------------------
# - paragraph section                                                        -
# ----------------------------------------------------------------------------

# the para command class
const afnix:adp:texpc (class)

# preset the command class

trans afnix:adp:texpc:preset nil {
  trans this:cdat (String)
}

# get the paragraph text

trans afnix:adp:texpc:to-text nil {
  if (this:cdat:nil-p) (eval " ")
  this:cdat:clone
}

# write the command to an output stream
# @param os the output stream to write

trans afnix:adp:texpc:write (os) {
  # write the command data
  if (not (this:cdat:nil-p)) {
    os:newline
    os:write this:cdat
    os:newline
  }
}

# add some text to the paragraph
# @param text the text to add

trans afnix:adp:texpc:add-text (text) {
  if (this:cdat:nil-p) (this:cdat:+= (text:strip-left " \t\r\n")) {
    this:cdat:+= (afnix:adp:text-to-tex text)
  }
}

# add teletype text to the paragraph
# @param text the text to add

trans afnix:adp:texpc:add-txtt (text) {
  this:add-text (+ (+ " \\texttt{" text) "} ")
}

# add bracketed teletype text to the paragraph
# @param text the text to add

trans afnix:adp:texpc:add-txbt (text) {
  this:add-text (+ (+ " \\texttt{[" text) "]} ")
}

# add bold text to the paragraph
# @param text the text to add

trans afnix:adp:texpc:add-txbf (text) {
  this:add-text (+ (+ " \\textbf{" text) "} ")
}

# add italic text to the paragraph
# @param text the text to add

trans afnix:adp:texpc:add-txit (text) {
  this:add-text (+ (+ " \\textit{" text) "} ")
}

# add index text to the paragraph
# @param text the text to add

trans afnix:adp:texpc:add-indx (text) {
  this:add-text (+ (+ " \\index{" text) "} ")
}

# ----------------------------------------------------------------------------
# - ruled box section                                                         -
# ----------------------------------------------------------------------------

# the latex example command class
const afnix:adp:texrb (class)

# preset the command class
# @param name the command name

trans afnix:adp:texrb:preset (text) {
  trans this:cdat (text:clone)
}

# write the command to an output stream
# @param os the output stream to write

trans afnix:adp:texrb:write (os) {
  if (not (this:cdat:nil-p)) {
    os:writeln "\\begin{lstlisting}"
    os:write this:cdat
    os:newline
    os:writeln "\\end{lstlisting}"
  }
  os:newline
}

# ----------------------------------------------------------------------------
# - table section                                                            -
# ----------------------------------------------------------------------------

# the table command class
const afnix:adp:textb (class)

# preset the command class
# @param clen the number of columns

trans afnix:adp:textb:preset (clen) {
  const this:clen (clen:clone)
  trans this:nrow false
  trans this:hdat (String)
  trans this:cdat (String)
}

# add a new row in the table command

trans afnix:adp:textb:new-row nil {
  trans this:nrow true
  if (not (this:cdat:nil-p)) (this:cdat:+= "\\\\\n")
}

# add a new head text in the table command
# @param text the text to add

trans afnix:adp:textb:add-head (text) {
  # add a separator is needed
  if (not (this:hdat:nil-p)) (this:hdat:+= " & ")
  # add the string
  this:hdat:+= "\\textbf{"
  this:hdat:+= (afnix:adp:text-to-tex (text:strip " \t\r\n"))
  this:hdat:+= "}"
}

# add a new data text in the table command
# @param text the text to add

trans afnix:adp:textb:add-data (text) {
  # add a separator is needed
  if this:nrow {
    trans this:nrow false
  } (this:cdat:+= " & ")
  # add the string
  this:cdat:+= (afnix:adp:text-to-tex (text:strip " \t\r\n"))
}

# write the command to an output stream
# @param os the output stream to write

trans afnix:adp:textb:write (os) {
  # do nothing if nil
  if (and (== this:clen 0) (this:cdat:nil-p)) (return)
  # emit the table start
  os:newline
  os:writeln "\\begin{table}"
  os:writeln "\\vskip\\lineskip"
  os:write   "\\begin{tabularx}{\\linewidth}{|"
  loop (trans i 0) (< i this:clen) (i:++) (os:write "X|")
  os:writeln "}"
  os:writeln "\\hline"
  # emit the head if any
  if (not (this:hdat:nil-p)) {
    os:writeln this:hdat "\\\\"
    os:writeln "\\hline\\hline"
  }
  # emit the table
  os:write this:cdat
  os:writeln "\\\\"
  # emit the footer
  os:writeln "\\hline"
  os:writeln "\\end{tabularx}"
  os:writeln "\\vskip\\lineskip"
  os:writeln "\\end{table}"
}

# ----------------------------------------------------------------------------
# - call section                                                             -
# ----------------------------------------------------------------------------

# the call command class
const afnix:adp:texfc (class)

# preset the call command

trans afnix:adp:texfc:preset nil {
  trans this:cnam (String)
  trans this:cret (String)
  trans this:carg (String)
  trans this:cdat (String)
}

# set the command name
# @param name the command name to set

trans afnix:adp:texfc:set-name (name) {
  trans this:cnam (name:clone)
}

# set the command return text
# @param text the return text to set

trans afnix:adp:texfc:set-retn (text) {
  trans this:cret (text:clone)
}

# set the command arguments
# @param text the arguments text to set

trans afnix:adp:texfc:set-args (text) {
  trans this:carg (text:clone)
}

# set the command data
# @param text the command data text

trans afnix:adp:texfc:set-data (text) {
  trans this:cdat (afnix:adp:text-to-tex (text:strip " \t\r\n"))
}

# write the command to an output stream
# @param os the output stream to write

trans afnix:adp:texfc:write (os) {
  # start by indenting
  os:newline
  # add the method name
  trans mcna (afnix:adp:text-to-tex this:cnam)
  trans marg (+ (+ "(" this:carg) ")")
  trans mrtn (+ (+ " $\\rightarrow$ " this:cret) " ")
  trans mnam (+ (+ (+ (+ "\\item\\textit{" mcna) mrtn ) marg) "}\\\\")
  os:writeln mnam
  os:writeln this:cdat
  os:writeln (+ (+ "\\index{" mcna) "}")
}

# ----------------------------------------------------------------------------
# - option section                                                             -
# ----------------------------------------------------------------------------

# the client option class
const afnix:adp:texco (class)

# preset the option command

trans afnix:adp:texco:preset nil {
  trans this:onam (String)
  trans this:carg (String)
  trans this:cdat (String)
}

# set the option name
# @param name the option name to set

trans afnix:adp:texco:set-name (name) {
  trans this:onam (name:clone)
}

# set the command arguments
# @param text the arguments text to set

trans afnix:adp:texco:set-args (text) {
  trans this:carg (text:clone)
}

# set the command data
# @param text the command data text

trans afnix:adp:texco:set-data (text) {
  trans this:cdat (afnix:adp:text-to-tex (text:strip " \t\r\n"))
}

# write the command to an output stream
# @param os the output stream to write

trans afnix:adp:texco:write (os) {
  # start by indenting
  os:newline
  os:writeln "\\vskip\\lineskip"
  # add the option name
  if (this:carg:nil-p) {
    trans text (+ (+ "\\noindent\\textit{" this:onam) "}\\\\")
  } {
    trans marg (+ (+ " [" this:carg) "]")
    trans text (+ (+ (+ "\\noindent\\textit{" this:onam) marg) "}\\\\")
  }
  os:writeln text
  os:writeln "\\hrule"
  os:writeln "\\vskip\\lineskip"
  os:writeln (+ "\\noindent " this:cdat)
}

# ----------------------------------------------------------------------------
# - control section                                                          -
# ----------------------------------------------------------------------------

# the client control class
const afnix:adp:texct (class)

# preset the control command
# @param name the control name

trans afnix:adp:texct:preset (name) {
  trans this:cnam (name:clone)
}

# write the command to an output stream
# @param os the output stream to write

trans afnix:adp:texct:write (os) {
  # start by indenting
  os:newline
  os:writeln "\\vskip\\lineskip"
  # add the control name
  os:writeln this:cnam
  os:writeln "\\hrule"
  os:writeln "\\vskip\\lineskip"
}
