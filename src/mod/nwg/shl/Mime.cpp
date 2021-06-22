// ---------------------------------------------------------------------------
// - Mime.cpp                                                                -
// - afnix:nwg module - mime base class implementation                       -
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

#include "Mime.hpp"
#include "Vector.hpp"
#include "Loopable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the default mime type
  static const String NWG_MIME_DEF = "text/plain";

  // the extension/mime type definition structure
  struct s_emtd {
    // the mime extension
    const char* p_mext;
    // the mime value
    const char* p_mval;
  };

  // the number of mime definition
  static const long NWG_MIME_TSZ = 535;
  // the base mime definition
  static const s_emtd NWG_MIME_TBL[NWG_MIME_TSZ] = {
    {"ez",       "application/andrew-inset"},
    {"atomcat",  "application/atomcat+xml"},
    {"atomsrv",  "application/atomserv+xml"},
    {"atom",     "application/atom+xml"},
    {"cal",      "application/cals-1840"},
    {"cap",      "application/cap"},
    {"pcap",     "application/cap"},
    {"cu",       "application/cu-seeme"},
    {"tsp",      "application/dsptype"},
    {"evy",      "application/envoy"},
    {"fif",      "application/fractals"},
    {"spl",      "application/futuresplash"},
    {"hta",      "application/hta"},
    {"acx",      "application/internet-property-stream"},
    {"jar",      "application/java-archive"},
    {"js",       "text/javascript"},
    {"mjs",      "text/javascript"},
    {"json",     "application/json"},
    {"ser",      "application/java-serialized-object"},
    {"class",    "application/java-vm"},
    {"hqx",      "application/mac-binhex40"},
    {"cpt",      "application/mac-compactpro"},
    {"nb",       "application/mathematica"},
    {"mp4",      "application/mp4"},
    {"mpeg4",    "application/mp4"},
    {"mdb",      "application/msaccess"},
    {"doc",      "application/msword"},
    {"dot",      "application/msword"},
    {"mxf",      "application/mxf"},
    {"bin",      "application/octet-stream"},
    {"oda",      "application/oda"},
    {"ogg",      "application/ogg"},
    {"ogx",      "application/ogg"},
    {"pdf",      "application/pdf"},
    {"asc",      "application/pgp-encrypted"},
    {"pgp",      "application/pgp-encrypted"},
    {"key",      "application/pgp-keys"},
    {"asc",      "application/pgp-signature"},
    {"pgp",      "application/pgp-signature"},
    {"prf",      "application/pics-rules"},
    {"p10",      "application/pkcs10"},
    {"p7c",      "application/pkcs7-mime"},
    {"p7m",      "application/pkcs7-mime"},
    {"p7s",      "application/pkcs7-signature"},
    {"crl",      "application/pkix-crl"},
    {"ai",       "application/postscript"},
    {"eps",      "application/postscript"},
    {"ps",       "application/postscript"},
    {"rar",      "application/rar"},
    {"rdf",      "application/rdf+xml"},
    {"rss",      "application/rss+xml"},
    {"rtf",      "application/rtf"},
    {"setpay",   "application/set-payment-initiation"},
    {"setreg",   "application/set-registration-initiation"},
    {"sgml",     "application/sgml"},
    {"sml",      "application/sgml"},
    {"smi",      "application/smil"},
    {"smil",     "application/smil"},
    {"cdy",      "application/vnd.cinderella"},
    {"kml",      "application/vnd.google-earth.kml+xml"},
    {"kmz",      "application/vnd.google-earth.kmz"},
    {"wk1",      "application/vnd.lotus-1-2-3"},
    {"wk2",      "application/vnd.lotus-1-2-3"},
    {"wk3",      "application/vnd.lotus-1-2-3"},
    {"wk4",      "application/vnd.lotus-1-2-3"},
    {"wks",      "application/vnd.lotus-1-2-3"},
    {"apr",      "application/vnd.lotus-approach"},
    {"apt",      "application/vnd.lotus-approach"},
    {"apx",      "application/vnd.lotus-approach"},
    {"dgm",      "application/vnd.lotus-freelance"},
    {"prz",      "application/vnd.lotus-freelance"},
    {"nsf",      "application/vnd.lotus-notes"},
    {"or2",      "application/vnd.lotus-organizer"},
    {"or3",      "application/vnd.lotus-organizer"},
    {"or4",      "application/vnd.lotus-organizer"},
    {"lwp",      "application/vnd.lotus-wordpro"},
    {"xul",      "application/vnd.mozilla.xul+xml"},
    {"xls",      "application/vnd.ms-excel"},
    {"msg",      "application/vnd.ms-outlook"},
    {"cat",      "application/vnd.ms-pki.seccat"},
    {"stl",      "application/vnd.ms-pki.stl"},
    {"pps",      "application/vnd.ms-powerpoint"},
    {"ppt",      "application/vnd.ms-powerpoint"},
    {"mpp",      "application/vnd.ms-project"},
    {"wcm",      "application/vnd.ms-works"},
    {"wdb",      "application/vnd.ms-works"},
    {"wks",      "application/vnd.ms-works"},
    {"wps",      "application/vnd.ms-works"},
    {"fpx",      "application/vnd.netfpx"},
    {"odc",      "application/vnd.oasis.opendocument.chart"},
    {"odb",      "application/vnd.oasis.opendocument.database"},
    {"odf",      "application/vnd.oasis.opendocument.formula"},
    {"odg",      "application/vnd.oasis.opendocument.graphics"},
    {"otg",      "application/vnd.oasis.opendocument.graphics-template"},
    {"odi",      "application/vnd.oasis.opendocument.image"},
    {"odp",      "application/vnd.oasis.opendocument.presentation"},
    {"otp",      "application/vnd.oasis.opendocument.presentation-template"},
    {"ods",      "application/vnd.oasis.opendocument.spreadsheet"},
    {"ots",      "application/vnd.oasis.opendocument.spreadsheet-template"},
    {"odm",      "application/vnd.oasis.opendocument.text-master"},
    {"odt",      "application/vnd.oasis.opendocument.text"},
    {"ott",      "application/vnd.oasis.opendocument.text-template"},
    {"oth",      "application/vnd.oasis.opendocument.text-web"},
    {"cod",      "application/vnd.rim.cod"},
    {"mmf",      "application/vnd.smaf"},
    {"sdc",      "application/vnd.stardivision.calc"},
    {"sds",      "application/vnd.stardivision.chart"},
    {"sda",      "application/vnd.stardivision.draw"},
    {"sdd",      "application/vnd.stardivision.impress"},
    {"sdf",      "application/vnd.stardivision.math"},
    {"sgl",      "application/vnd.stardivision.writer-global"},
    {"sdw",      "application/vnd.stardivision.writer"},
    {"sxc",      "application/vnd.sun.xml.calc"},
    {"stc",      "application/vnd.sun.xml.calc.template"},
    {"sxd",      "application/vnd.sun.xml.draw"},
    {"std",      "application/vnd.sun.xml.draw.template"},
    {"sxi",      "application/vnd.sun.xml.impress"},
    {"sti",      "application/vnd.sun.xml.impress.template"},
    {"sxm",      "application/vnd.sun.xml.math"},
    {"sxg",      "application/vnd.sun.xml.writer.global"},
    {"sxw",      "application/vnd.sun.xml.writer"},
    {"stw",      "application/vnd.sun.xml.writer.template"},
    {"sis",      "application/vnd.symbian.install"},
    {"vsd",      "application/vnd.visio"},
    {"wbxml",    "application/vnd.wap.wbxml"},
    {"wmlc",     "application/vnd.wap.wmlc"},
    {"wmlsc",    "application/vnd.wap.wmlscriptc"},
    {"hlp",      "application/winhlp"},
    {"wp5",      "application/wordperfect5.1"},
    {"doc",      "application/wordperfect"},
    {"wpd",      "application/wordperfect"},
    {"wk",       "application/x-123"},
    {"7z",       "application/x-7z-compressed"},
    {"abw",      "application/x-abiword"},
    {"als",      "application/x-afnix-source"},
    {"ams",      "application/x-afnix-math-source"},
    {"asp",      "application/x-afnix-spreadsheet"},
    {"asl",      "application/x-afnix-octet-stream"},
    {"aem",      "application/x-afnix-serial-encoded"},
    {"axc",      "application/x-afnix-source-compiled"},
    {"axl",      "application/x-afnix-librarian"},
    {"dmg",      "application/x-apple-diskimage"},
    {"bcpio",    "application/x-bcpio"},
    {"torrent",  "application/x-bittorrent"},
    {"bz2",      "application/x-bzip2"},
    {"cab",      "application/x-cab"},
    {"cbr",      "application/x-cbr"},
    {"cbz",      "application/x-cbz"},
    {"cdf",      "application/x-cdf"},
    {"vcd",      "application/x-cdlink"},
    {"pgn",      "application/x-chess-pgn"},
    {"tar.gz",   "application/x-compressed"},
    {"taz",      "application/x-compressed"},
    {"tgz",      "application/x-compressed"},
    {"z",        "application/x-compress"},
    {"cpio",     "application/x-cpio"},
    {"csh",      "application/x-csh"},
    {"deb",      "application/x-debian-package"},
    {"dcr",      "application/x-director"},
    {"dms",      "application/x-dms"},
    {"wad",      "application/x-doom"},
    {"dvi",      "application/x-dvi"},
    {"flac",     "application/x-flac"},
    {"gsf",      "application/x-font"},
    {"pcf",      "application/x-font"},
    {"pfa",      "application/x-font"},
    {"pfb",      "application/x-font"},
    {"mm",       "application/x-freemind"},
    {"spl",      "application/x-futuresplash"},
    {"gnumeric", "application/x-gnumeric"},
    {"sgf",      "application/x-go-sgf"},
    {"gcf",      "application/x-graphing-calculator"},
    {"gtar",     "application/x-gtar"},
    {"taz",      "application/x-gtar"},
    {"tgz",      "application/x-gtar"},
    {"gz",       "application/x-gzip"},
    {"hdf",      "application/x-hdf"},
    {"xht",      "application/xhtml+xml"},
    {"xhtml",    "application/xhtml+xml"},
    {"rhtml",    "application/x-httpd-eruby"},
    {"php3",     "application/x-httpd-php3"},
    {"php3p",    "application/x-httpd-php3-preprocessed"},
    {"php4",     "application/x-httpd-php4"},
    {"php",      "application/x-httpd-php"},
    {"pht",      "application/x-httpd-php"},
    {"phtml",    "application/x-httpd-php"},
    {"phps",     "application/x-httpd-php-source"},
    {"ica",      "application/x-ica"},
    {"ins",      "application/x-internet-signup"},
    {"isp",      "application/x-internet-signup"},
    {"iii",      "application/x-iphone"},
    {"iso",      "application/x-iso9660-image"},
    {"class",    "application/x-java-applet"},
    {"jcm",      "application/x-java-commerce"},
    {"jnlp",     "application/x-java-jnlp-file"},
    {"js",       "application/x-javascript"},
    {"java",     "application/x-java-source"},
    {"jmz",      "application/x-jmol"},
    {"chrt",     "application/x-kchart"},
    {"kil",      "application/x-killustrator"},
    {"skd",      "application/x-koan"},
    {"skm",      "application/x-koan"},
    {"skp",      "application/x-koan"},
    {"skt",      "application/x-koan"},
    {"kpr",      "application/x-kpresenter"},
    {"kpt",      "application/x-kpresenter"},
    {"ksp",      "application/x-kspread"},
    {"kwd",      "application/x-kword"},
    {"kwt",      "application/x-kword"},
    {"latex",    "application/x-latex"},
    {"lha",      "application/x-lha"},
    {"lyx",      "application/x-lyx"},
    {"lzh",      "application/x-lzh"},
    {"lzx",      "application/x-lzx"},
    {"fb",       "application/x-maker"},
    {"fm",       "application/x-maker"},
    {"frame",    "application/x-maker"},
    {"frm",      "application/x-maker"},
    {"maker",    "application/x-maker"},
    {"mif",      "application/x-mif"},
    {"dtd",      "application/xml-dtd"},
    {"xml",      "application/xml"},
    {"xsl",      "application/xml"},
    {"bat",      "application/x-msdos-program"},
    {"com",      "application/x-msdos-program"},
    {"dll",      "application/x-msdos-program"},
    {"exe",      "application/x-msdos-program"},
    {"msi",      "application/x-msi"},
    {"wmd",      "application/x-ms-wmd"},
    {"wmz",      "application/x-ms-wmz"},
    {"cdf",      "application/x-netcdf"},
    {"nc",       "application/x-netcdf"},
    {"pac",      "application/x-ns-proxy-autoconfig"},
    {"nwc",      "application/x-nwc"},
    {"o",        "application/x-object"},
    {"oza",      "application/x-oz-application"},
    {"pma",      "application/x-perfmon"},
    {"pmc",      "application/x-perfmon"},
    {"pml",      "application/x-perfmon"},
    {"pmr",      "application/x-perfmon"},
    {"pmw",      "application/x-perfmon"},
    {"p12",      "application/x-pkcs12"},
    {"pfx",      "application/x-pkcs12"},
    {"p7b",      "application/x-pkcs7-certificates"},
    {"spc",      "application/x-pkcs7-certificates"},
    {"p7r",      "application/x-pkcs7-certreqresp"},
    {"p7r",      "application/x-pkcs7-certreqresp"},
    {"crl",      "application/x-pkcs7-crl"},
    {"pyc",      "application/x-python-code"},
    {"pyo",      "application/x-python-code"},
    {"qtl",      "application/x-quicktimeplayer"},
    {"rpm",      "application/x-redhat-package-manager"},
    {"shar",     "application/x-shar"},
    {"swf",      "application/x-shockwave-flash"},
    {"swfl",     "application/x-shockwave-flash"},
    {"sh",       "application/x-sh"},
    {"sit",      "application/x-stuffit"},
    {"sitx",     "application/x-stuffit"},
    {"sv4cpio",  "application/x-sv4cpio"},
    {"sv4crc",   "application/x-sv4crc"},
    {"tar",      "application/x-tar"},
    {"tcl",      "application/x-tcl"},
    {"gf",       "application/x-tex-gf"},
    {"texi",     "application/x-texinfo"},
    {"texinfo",  "application/x-texinfo"},
    {"pk",       "application/x-tex-pk"},
    {"backup",   "application/x-trash"},
    {"bak",      "application/x-trash"},
    {"old",      "application/x-trash"},
    {"man",      "application/x-troff-man"},
    {"me",       "application/x-troff-me"},
    {"ms",       "application/x-troff-ms"},
    {"roff",     "application/x-troff"},
    {"t",        "application/x-troff"},
    {"tr",       "application/x-troff"},
    {"ustar",    "application/x-ustar"},
    {"src",      "application/x-wais-source"},
    {"wz",       "application/x-wingz"},
    {"crt",      "application/x-x509-ca-cert"},
    {"xcf",      "application/x-xcf"},
    {"fig",      "application/x-xfig"},
    {"xpi",      "application/x-xpinstall"},
    {"zip",      "application/zip"},
    {"au",       "audio/basic"},
    {"snd",      "audio/basic"},
    {"kar",      "audio/midi"},
    {"mid",      "audio/midi"},
    {"midi",     "audio/midi"},
    {"m4a",      "audio/mpeg"},
    {"mp2",      "audio/mpeg"},
    {"mp3",      "audio/mpeg"},
    {"mpega",    "audio/mpeg"},
    {"mpga",     "audio/mpeg"},
    {"m3u",      "audio/mpegurl"},
    {"oga",      "audio/ogg"},
    {"spx",      "audio/ogg"},
    {"sid",      "audio/prs.sid"},
    {"aif",      "audio/x-aiff"},
    {"aifc",     "audio/x-aiff"},
    {"aiff",     "audio/x-aiff"},
    {"gsm",      "audio/x-gsm"},
    {"m3u",      "audio/x-mpegurl"},
    {"wax",      "audio/x-ms-wax"},
    {"wma",      "audio/x-ms-wma"},
    {"ra",       "audio/x-pn-realaudio"},
    {"ram",      "audio/x-pn-realaudio"},
    {"rm",       "audio/x-pn-realaudio"},
    {"ra",       "audio/x-realaudio"},
    {"pls",      "audio/x-scpls"},
    {"sd2",      "audio/x-sd2"},
    {"wav",      "audio/x-wav"},
    {"alc",      "chemical/x-alchemy"},
    {"cac",      "chemical/x-cache"},
    {"cache",    "chemical/x-cache"},
    {"csf",      "chemical/x-cache-csf"},
    {"cascii",   "chemical/x-cactvs-binary"},
    {"cbin",     "chemical/x-cactvs-binary"},
    {"ctab",     "chemical/x-cactvs-binary"},
    {"cdx",      "chemical/x-cdx"},
    {"cer",      "chemical/x-cerius"},
    {"c3d",      "chemical/x-chem3d"},
    {"chm",      "chemical/x-chemdraw"},
    {"cif",      "chemical/x-cif"},
    {"cmdf",     "chemical/x-cmdf"},
    {"cml",      "chemical/x-cml"},
    {"cpa",      "chemical/x-compass"},
    {"bsd",      "chemical/x-crossfire"},
    {"csm",      "chemical/x-csml"},
    {"csml",     "chemical/x-csml"},
    {"ctx",      "chemical/x-ctx"},
    {"cef",      "chemical/x-cxf"},
    {"cxf",      "chemical/x-cxf"},
    {"smi",      "chemical/x-daylight-smiles"},
    {"emb",      "chemical/x-embl-dl-nucleotide"},
    {"embl",     "chemical/x-embl-dl-nucleotide"},
    {"spc",      "chemical/x-galactic-spc"},
    {"gam",      "chemical/x-gamess-input"},
    {"gamin",    "chemical/x-gamess-input"},
    {"inp",      "chemical/x-gamess-input"},
    {"fch",      "chemical/x-gaussian-checkpoint"},
    {"fchk",     "chemical/x-gaussian-checkpoint"},
    {"cub",      "chemical/x-gaussian-cube"},
    {"gau",      "chemical/x-gaussian-input"},
    {"gjc",      "chemical/x-gaussian-input"},
    {"gjf",      "chemical/x-gaussian-input"},
    {"gal",      "chemical/x-gaussian-log"},
    {"gcg",      "chemical/x-gcg8-sequence"},
    {"gen",      "chemical/x-genbank"},
    {"hin",      "chemical/x-hin"},
    {"ist",      "chemical/x-isostar"},
    {"istr",     "chemical/x-isostar"},
    {"dx",       "chemical/x-jcamp-dx"},
    {"jdx",      "chemical/x-jcamp-dx"},
    {"kin",      "chemical/x-kinemage"},
    {"mcm",      "chemical/x-macmolecule"},
    {"mmd",      "chemical/x-macromodel-input"},
    {"mmod",     "chemical/x-macromodel-input"},
    {"mol",      "chemical/x-mdl-molfile"},
    {"rd",       "chemical/x-mdl-rdfile"},
    {"rxn",      "chemical/x-mdl-rxnfile"},
    {"sd",       "chemical/x-mdl-sdfile"},
    {"sdf",      "chemical/x-mdl-sdfile"},
    {"tgf",      "chemical/x-mdl-tgf"},
    {"mif",      "chemical/x-mif"},
    {"mcif",     "chemical/x-mmcif"},
    {"mol2",     "chemical/x-mol2"},
    {"b",        "chemical/x-molconn-Z"},
    {"gpt",      "chemical/x-mopac-graph"},
    {"dat",      "chemical/x-mopac-input"},
    {"mop",      "chemical/x-mopac-input"},
    {"mopcrt",   "chemical/x-mopac-input"},
    {"mpc",      "chemical/x-mopac-input"},
    {"zmt",      "chemical/x-mopac-input"},
    {"moo",      "chemical/x-mopac-out"},
    {"mvb",      "chemical/x-mopac-vib"},
    {"ent",      "chemical/x-ncbi-asn1-ascii"},
    {"prt",      "chemical/x-ncbi-asn1-ascii"},
    {"asn",      "chemical/x-ncbi-asn1"},
    {"aso",      "chemical/x-ncbi-asn1-binary"},
    {"val",      "chemical/x-ncbi-asn1-binary"},
    {"asn",      "chemical/x-ncbi-asn1-spec"},
    {"ent",      "chemical/x-pdb"},
    {"pdb",      "chemical/x-pdb"},
    {"ros",      "chemical/x-rosdal"},
    {"sw",       "chemical/x-swissprot"},
    {"vms",      "chemical/x-vamas-iso14976"},
    {"vmd",      "chemical/x-vmd"},
    {"xtel",     "chemical/x-xtel"},
    {"xyz",      "chemical/x-xyz"},
    {"cgm",      "image/cgm"},
    {"g3",       "image/g3fax"},
    {"gif",      "image/gif"},
    {"ief",      "image/ief"},
    {"jfif",     "image/jpeg"},
    {"jpe",      "image/jpeg"},
    {"jpeg",     "image/jpeg"},
    {"jpg",      "image/jpeg"},
    {"pcx",      "image/pcx"},
    {"png",      "image/png"},
    {"svg",      "image/svg+xml"},
    {"svgz",     "image/svg+xml"},
    {"tif",      "image/tiff"},
    {"tiff",     "image/tiff"},
    {"djv",      "image/vnd.djvu"},
    {"djvu",     "image/vnd.djvu"},
    {"dwg",      "image/vnd.dwg"},
    {"dxf",      "image/vnd.dxf"},
    {"fpx",      "image/vnd.fpx"},
    {"fpx",      "image/vnd.net-fpx"},
    {"wbmp",     "image/vnd.wap.wbmp"},
    {"ras",      "image/x-cmu-raster"},
    {"cdr",      "image/x-coreldraw"},
    {"pat",      "image/x-coreldrawpattern"},
    {"cdt",      "image/x-coreldrawtemplate"},
    {"cpt",      "image/x-corelphotopaint"},
    {"ico",      "image/x-icon"},
    {"art",      "image/x-jg"},
    {"jng",      "image/x-jng"},
    {"bmp",      "image/x-ms-bmp"},
    {"psd",      "image/x-photoshop"},
    {"pnm",      "image/x-portable-anymap"},
    {"pbm",      "image/x-portable-bitmap"},
    {"pgm",      "image/x-portable-graymap"},
    {"ppm",      "image/x-portable-pixmap"},
    {"rgb",      "image/x-rgb"},
    {"xbm",      "image/x-xbitmap"},
    {"xpm",      "image/x-xpixmap"},
    {"xwd",      "image/x-xwindowdump"},
    {"eml",      "message/rfc822"},
    {"iges",     "model/iges"},
    {"igs",      "model/iges"},
    {"mesh",     "model/mesh"},
    {"msh",      "model/mesh"},
    {"silo",     "model/mesh"},
    {"dwf",      "model/vnd.dwf"},
    {"vrml",     "model/vrml"},
    {"wrl",      "model/vrml"},
    {"ics",      "text/calendar"},
    {"icz",      "text/calendar"},
    {"css",      "text/css"},
    {"csv",      "text/csv"},
    {"323",      "text/h323"},
    {"htm",      "text/html"},
    {"html",     "text/html"},
    {"sht",      "text/html"},
    {"shtml",    "text/html"},
    {"uls",      "text/iuls"},
    {"mml",      "text/mathml"},
    {"asc",      "application/pgp-encrypted"},
    {"txt",      "text/plain"},
    {"text",     "text/plain"},
    {"pot",      "text/plain"},
    {"rtx",      "text/richtext"},
    {"rtf",      "text/rtf"},
    {"sct",      "text/scriptlet"},
    {"wsc",      "text/scriptlet"},
    {"tsv",      "text/tab-separated-values"},
    {"tm",       "text/texmacs"},
    {"ts",       "text/texmacs"},
    {"jad",      "text/vnd.sun.j2me.app-descriptor"},
    {"wmls",     "text/vnd.wap.wmlscript"},
    {"wml",      "text/vnd.wap.wml"},
    {"bib",      "text/x-bibtex"},
    {"boo",      "text/x-boo"},
    {"h++",      "text/x-c++hdr"},
    {"h",        "text/x-chdr"},
    {"hh",       "text/x-c++hdr"},
    {"hpp",      "text/x-c++hdr"},
    {"hxx",      "text/x-c++hdr"},
    {"htc",      "text/x-component"},
    {"csh",      "text/x-csh"},
    {"c++",      "text/x-c++src"},
    {"c",        "text/x-csrc"},
    {"cc",       "text/x-c++src"},
    {"cpp",      "text/x-c++src"},
    {"cxx",      "text/x-c++src"},
    {"diff",     "text/x-diff"},
    {"patch",    "text/x-diff"},
    {"d",        "text/x-dsrc"},
    {"f",        "text/x-fortran"},
    {"f77",      "text/x-fortran"},
    {"f90",      "text/x-fortran"},
    {"for",      "text/x-fortran"},
    {"hs",       "text/x-haskell"},
    {"java",     "text/x-java"},
    {"lhs",      "text/x-literate-haskell"},
    {"moc",      "text/x-moc"},
    {"p",        "text/x-pascal"},
    {"gcd",      "text/x-pcs-gcd"},
    {"pl",       "text/x-perl"},
    {"pm",       "text/x-perl"},
    {"pl",       "text/x-prolog"},
    {"pro",      "text/x-prolog"},
    {"prolog",   "text/x-prolog"},
    {"py",       "text/x-python"},
    {"etx",      "text/x-setext"},
    {"sh",       "text/x-sh"},
    {"tcl",      "text/x-tcl"},
    {"tk",       "text/x-tcl"},
    {"cls",      "text/x-tex"},
    {"ltx",      "text/x-tex"},
    {"sty",      "text/x-tex"},
    {"tex",      "text/x-tex"},
    {"vcs",      "text/x-vcalendar"},
    {"vcf",      "text/x-vcard"},
    {"3gp",      "video/3gpp"},
    {"dl",       "video/dl"},
    {"dif",      "video/dv"},
    {"dv",       "video/dv"},
    {"fli",      "video/fli"},
    {"gl",       "video/gl"},
    {"mp4",      "video/mp4"},
    {"mpe",      "video/mpeg"},
    {"mpeg",     "video/mpeg"},
    {"mpg",      "video/mpeg"},
    {"ogv",      "video/ogg"},
    {"mov",      "video/quicktime"},
    {"qt",       "video/quicktime"},
    {"mxu",      "video/vnd.mpegurl"},
    {"lsf",      "video/x-la-asf"},
    {"lsx",      "video/x-la-asf"},
    {"mng",      "video/x-mng"},
    {"asf",      "video/x-ms-asf"},
    {"asx",      "video/x-ms-asf"},
    {"avi",      "video/x-msvideo"},
    {"wmv",      "video/x-ms-wmv"},
    {"wm",       "video/x-ms-wm"},
    {"wmx",      "video/x-ms-wmx"},
    {"wvx",      "video/x-ms-wvx"},
    {"movie",    "video/x-sgi-movie"},
    {"ice",      "x-conference/x-cooltalk"},
    {"sisx",     "x-epoc/x-sisx-app"},
    {"vrm",      "x-world/x-vrml"},
    {"vrml",     "x-world/x-vrml"},
    {"wrl",      "x-world/x-vrml"}
  };
  
  // the valid xml mime types
  static const long  NWG_MIME_XSZ = 14;
  static const char* NWG_MIME_XML[NWG_MIME_XSZ] = {
    "application/atomcat+xml",
    "application/atomserv+xml",
    "application/atom+xml",
    "application/rdf+xml",
    "application/rss+xml",
    "application/vnd.google-earth.kml+xml",
    "application/vnd.mozilla.xul+xml",
    "application/xhtml+xml",
    "application/xhtml+xml",
    "application/xml-dtd",
    "application/xml",
    "application/xml",
    "image/svg+xml",
    "image/svg+xml"
  };

  // -------------------------------------------------------------------------
  // - public section                                                         -
  // -------------------------------------------------------------------------

  // check if a mime extension is defined

  bool Mime::ismext (const String& mext) {
    // loop in the mime table
    for (long i = 0; i < NWG_MIME_TSZ; i++) {
      if (mext == NWG_MIME_TBL[i].p_mext) return true;
    }
    return false;
  }

  // check if a mime type is defined

  bool Mime::ismval (const String& mval) {
    // loop in the mime table
    for (long i = 0; i < NWG_MIME_TSZ; i++) {
      if (mval == NWG_MIME_TBL[i].p_mval) return true;
    }
    return false;
  }

  // check if a mime type is an xml type

  bool Mime::isxmlm (const String& mval) {
    for (long k = 0; k < NWG_MIME_XSZ; k++) {
      if (mval == NWG_MIME_XML[k]) return true;
    }
    return false;
  }

  // get a mime extension by object

  String Mime::tomext (const String& mime) {
    // loop in the mime table
    for (long i = 0; i < NWG_MIME_TSZ; i++) {
      if (mime == NWG_MIME_TBL[i].p_mval) {
	String result = NWG_MIME_TBL[i].p_mext;
	return result;
      }
    }
    // invalid mime type
    throw Exception ("mime-error", "invalid mime object to convert", mime);
  }

  // get a mime object by extension

  String Mime::tomime (const String& mext) {
    return Mime::tomime (mext, false);
  }
  
  // get a mime object by extension

  String Mime::tomime (const String& mext, const bool dflg) {
    // loop in the mime table
    for (long i = 0; i < NWG_MIME_TSZ; i++) {
      if (mext == NWG_MIME_TBL[i].p_mext) {
	String result = NWG_MIME_TBL[i].p_mval;
	return result;
      }
    }
    // fallback on default type if requested
    if (dflg == true) return NWG_MIME_DEF;
    // invalid mime type
    throw Exception ("mime-error", "invalid extension to convert", mext);
  }

  // get the default mime definition

  String Mime::getmdef (void) {
    return NWG_MIME_DEF;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default mime object

  Mime::Mime (void) {
    d_mime = NWG_MIME_DEF;
  }

  // create a mime object by type

  Mime::Mime (const String& type) {
    d_mime = type;
  }

  // copy construct a mime object

  Mime::Mime (const Mime& that) {
    that.rdlock ();
    try {
      d_mime = that.d_mime;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // assign a mime object to this one

  Mime& Mime::operator = (const Mime& that) {
    if (this == &that) return *this;
    wrlock ();
    that.rdlock ();
    try {
      d_mime = that.d_mime;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // return the document class name

  String Mime::repr (void) const {
    return "Mime";
  }

  // return the mime value

  String Mime::getmime (void) const {
    rdlock ();
    try {
      String result = d_mime;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
    
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_WRITE   = zone.intern ("write");
  static const long QUARK_GETMIME = zone.intern ("get-mime");

  // return true if the given quark is defined

  bool Mime::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Object::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* Mime::apply (Evaluable* zobj, Nameset* nset, const long quark,
		       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETMIME) return new String (getmime ());
      if (quark == QUARK_WRITE) {
	auto lobj = dynamic_cast<Loopable*>(zobj);
	OutputStream* os = (lobj == nullptr) ? nullptr : lobj->getos ();
	if (os != nullptr) write (*os);
	return nullptr;
      }
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_WRITE) {
	Object* obj = argv->get (0);
	// check for an output stream
	OutputStream* os = dynamic_cast <OutputStream*> (obj);
	if (os != nullptr) {
	  write (*os);
	  return nullptr;
	}
	// check for a buffer
	Buffer* buf = dynamic_cast <Buffer*> (obj);
	if (buf != nullptr) {
	  write (*buf);
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object with write",
			 Object::repr (obj));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
