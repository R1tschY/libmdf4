/*
 * Copyright (C) 2012 Christoph Gärtner
 * Copyright (C) 2014 Richard Liebscher <r1tschy@yahoo.de>
 *
 * Distributed under the Boost Software License, Version 1.0
 *
 */

#include "xml.h"

#include <cstring>
#include <boost/lexical_cast.hpp>

#include "macros.h"

namespace mdf {

#define UNICODE_MAX 0x10FFFFul

static const char * const NAMED_ENTITIES[][2] =
    { { "AElig;", "Ã†" }, { "Aacute;", "Ã" }, { "Acirc;", "Ã‚" }, { "Agrave;",
        "Ã€" }, { "Alpha;", "Î‘" }, { "Aring;", "Ã…" }, { "Atilde;", "Ãƒ" }, {
        "Auml;", "Ã„" }, { "Beta;", "Î’" }, { "Ccedil;", "Ã‡" },
        { "Chi;", "Î§" }, { "Dagger;", "â€¡" }, { "Delta;", "Î”" }, { "ETH;",
            "Ã" }, { "Eacute;", "Ã‰" }, { "Ecirc;", "ÃŠ" },
        { "Egrave;", "Ãˆ" }, { "Epsilon;", "Î•" }, { "Eta;", "Î—" }, { "Euml;",
            "Ã‹" }, { "Gamma;", "Î“" }, { "Iacute;", "Ã" }, { "Icirc;", "ÃŽ" },
        { "Igrave;", "ÃŒ" }, { "Iota;", "Î™" }, { "Iuml;", "Ã" }, { "Kappa;",
            "Îš" }, { "Lambda;", "Î›" }, { "Mu;", "Îœ" }, { "Ntilde;", "Ã‘" }, {
            "Nu;", "Î" }, { "OElig;", "Å’" }, { "Oacute;", "Ã“" }, { "Ocirc;",
            "Ã”" }, { "Ograve;", "Ã’" }, { "Omega;", "Î©" },
        { "Omicron;", "ÎŸ" }, { "Oslash;", "Ã˜" }, { "Otilde;", "Ã•" }, {
            "Ouml;", "Ã–" }, { "Phi;", "Î¦" }, { "Pi;", "Î " }, { "Prime;",
            "â€³" }, { "Psi;", "Î¨" }, { "Rho;", "Î¡" }, { "Scaron;", "Å " }, {
            "Sigma;", "Î£" }, { "THORN;", "Ãž" }, { "Tau;", "Î¤" }, { "Theta;",
            "Î˜" }, { "Uacute;", "Ãš" }, { "Ucirc;", "Ã›" },
        { "Ugrave;", "Ã™" }, { "Upsilon;", "Î¥" }, { "Uuml;", "Ãœ" }, { "Xi;",
            "Îž" }, { "Yacute;", "Ã" }, { "Yuml;", "Å¸" }, { "Zeta;", "Î–" }, {
            "aacute;", "Ã¡" }, { "acirc;", "Ã¢" }, { "acute;", "Â´" }, {
            "aelig;", "Ã¦" }, { "agrave;", "Ã " }, { "alefsym;", "â„µ" }, {
            "alpha;", "Î±" }, { "amp;", "&" }, { "and;", "âˆ§" }, { "ang;",
            "âˆ " }, { "apos;", "'" }, { "aring;", "Ã¥" }, { "asymp;", "â‰ˆ" },
        { "atilde;", "Ã£" }, { "auml;", "Ã¤" }, { "bdquo;", "â€ž" }, { "beta;",
            "Î²" }, { "brvbar;", "Â¦" }, { "bull;", "â€¢" }, { "cap;", "âˆ©" },
        { "ccedil;", "Ã§" }, { "cedil;", "Â¸" }, { "cent;", "Â¢" }, { "chi;",
            "Ï‡" }, { "circ;", "Ë†" }, { "clubs;", "â™£" }, { "cong;", "â‰…" },
        { "copy;", "Â©" }, { "crarr;", "â†µ" }, { "cup;", "âˆª" }, { "curren;",
            "Â¤" }, { "dArr;", "â‡“" }, { "dagger;", "â€ " },
        { "darr;", "â†“" }, { "deg;", "Â°" }, { "delta;", "Î´" }, { "diams;",
            "â™¦" }, { "divide;", "Ã·" }, { "eacute;", "Ã©" },
        { "ecirc;", "Ãª" }, { "egrave;", "Ã¨" }, { "empty;", "âˆ…" }, { "emsp;",
            "â€ƒ" }, { "ensp;", "â€‚" }, { "epsilon;", "Îµ" },
        { "equiv;", "â‰¡" }, { "eta;", "Î·" }, { "eth;", "Ã°" },
        { "euml;", "Ã«" }, { "euro;", "â‚¬" }, { "exist;", "âˆƒ" }, { "fnof;",
            "Æ’" }, { "forall;", "âˆ€" }, { "frac12;", "Â½" },
        { "frac14;", "Â¼" }, { "frac34;", "Â¾" }, { "frasl;", "â„" }, {
            "gamma;", "Î³" }, { "ge;", "â‰¥" }, { "gt;", ">" },
        { "hArr;", "â‡”" }, { "harr;", "â†”" }, { "hearts;", "â™¥" }, {
            "hellip;", "â€¦" }, { "iacute;", "Ã­" }, { "icirc;", "Ã®" }, {
            "iexcl;", "Â¡" }, { "igrave;", "Ã¬" }, { "image;", "â„‘" }, {
            "infin;", "âˆž" }, { "int;", "âˆ«" }, { "iota;", "Î¹" }, {
            "iquest;", "Â¿" }, { "isin;", "âˆˆ" }, { "iuml;", "Ã¯" }, {
            "kappa;", "Îº" }, { "lArr;", "â‡" }, { "lambda;", "Î»" }, {
            "lang;", "ã€ˆ" }, { "laquo;", "Â«" }, { "larr;", "â†" }, {
            "lceil;", "âŒˆ" }, { "ldquo;", "â€œ" }, { "le;", "â‰¤" }, {
            "lfloor;", "âŒŠ" }, { "lowast;", "âˆ—" }, { "loz;", "â—Š" }, {
            "lrm;", "\xE2\x80\x8E" }, { "lsaquo;", "â€¹" }, { "lsquo;", "â€˜" },
        { "lt;", "<" }, { "macr;", "Â¯" }, { "mdash;", "â€”" },
        { "micro;", "Âµ" }, { "middot;", "Â·" }, { "minus;", "âˆ’" }, { "mu;",
            "Î¼" }, { "nabla;", "âˆ‡" }, { "nbsp;", "Â " }, { "ndash;", "â€“" },
        { "ne;", "â‰ " }, { "ni;", "âˆ‹" }, { "not;", "Â¬" },
        { "notin;", "âˆ‰" }, { "nsub;", "âŠ„" }, { "ntilde;", "Ã±" }, { "nu;",
            "Î½" }, { "oacute;", "Ã³" }, { "ocirc;", "Ã´" }, { "oelig;", "Å“" },
        { "ograve;", "Ã²" }, { "oline;", "â€¾" }, { "omega;", "Ï‰" }, {
            "omicron;", "Î¿" }, { "oplus;", "âŠ•" }, { "or;", "âˆ¨" }, {
            "ordf;", "Âª" }, { "ordm;", "Âº" }, { "oslash;", "Ã¸" }, {
            "otilde;", "Ãµ" }, { "otimes;", "âŠ—" }, { "ouml;", "Ã¶" }, {
            "para;", "Â¶" }, { "part;", "âˆ‚" }, { "permil;", "â€°" }, {
            "perp;", "âŠ¥" }, { "phi;", "Ï†" }, { "pi;", "Ï€" },
        { "piv;", "Ï–" }, { "plusmn;", "Â±" }, { "pound;", "Â£" }, { "prime;",
            "â€²" }, { "prod;", "âˆ" }, { "prop;", "âˆ" }, { "psi;", "Ïˆ" }, {
            "quot;", "\"" }, { "rArr;", "â‡’" }, { "radic;", "âˆš" }, { "rang;",
            "ã€‰" }, { "raquo;", "Â»" }, { "rarr;", "â†’" },
        { "rceil;", "âŒ‰" }, { "rdquo;", "â€" }, { "real;", "â„œ" }, { "reg;",
            "Â®" }, { "rfloor;", "âŒ‹" }, { "rho;", "Ï" }, { "rlm;",
            "\xE2\x80\x8F" }, { "rsaquo;", "â€º" }, { "rsquo;", "â€™" }, {
            "sbquo;", "â€š" }, { "scaron;", "Å¡" }, { "sdot;", "â‹…" }, {
            "sect;", "Â§" }, { "shy;", "\xC2\xAD" }, { "sigma;", "Ïƒ" }, {
            "sigmaf;", "Ï‚" }, { "sim;", "âˆ¼" }, { "spades;", "â™ " }, {
            "sub;", "âŠ‚" }, { "sube;", "âŠ†" }, { "sum;", "âˆ‘" }, { "sup;",
            "âŠƒ" }, { "sup1;", "Â¹" }, { "sup2;", "Â²" }, { "sup3;", "Â³" }, {
            "supe;", "âŠ‡" }, { "szlig;", "ÃŸ" }, { "tau;", "Ï„" }, { "there4;",
            "âˆ´" }, { "theta;", "Î¸" }, { "thetasym;", "Ï‘" }, { "thinsp;",
            "â€‰" }, { "thorn;", "Ã¾" }, { "tilde;", "Ëœ" }, { "times;", "Ã—" },
        { "trade;", "â„¢" }, { "uArr;", "â‡‘" }, { "uacute;", "Ãº" }, { "uarr;",
            "â†‘" }, { "ucirc;", "Ã»" }, { "ugrave;", "Ã¹" }, { "uml;", "Â¨" },
        { "upsih;", "Ï’" }, { "upsilon;", "Ï…" }, { "uuml;", "Ã¼" }, {
            "weierp;", "â„˜" }, { "xi;", "Î¾" }, { "yacute;", "Ã½" }, { "yen;",
            "Â¥" }, { "yuml;", "Ã¿" }, { "zeta;", "Î¶" }, { "zwj;",
            "\xE2\x80\x8D" }, { "zwnj;", "\xE2\x80\x8C" } };

static int cmp_string(const char* const * const key, boost::string_ref value) {
  return boost::string_ref(key[0]).compare(value);
}

static boost::string_ref get_named_entity(boost::string_ref name) {
  auto entry = std::lower_bound(RANGE(NAMED_ENTITIES), name, cmp_string);
  if (entry != std::end(NAMED_ENTITIES)) {
    return (*entry)[1];
  } else {
    return boost::string_ref();
  }
}

static void putc_utf8(unsigned long cp, std::string& buffer) {
  if (cp <= 0x007Ful) {
    buffer.push_back(static_cast<char>(cp));

  } else if (cp <= 0x07FFul) {
    buffer.push_back(static_cast<char>((2 << 6) | (cp & 0x3F)));
    buffer.push_back(static_cast<char>((6 << 5) | (cp >> 6)));

  } else if (cp <= 0xFFFFul) {
    buffer.push_back(static_cast<char>((2 << 6) | (cp & 0x3F)));
    buffer.push_back(static_cast<char>((2 << 6) | ((cp >> 6) & 0x3F)));
    buffer.push_back(static_cast<char>((14 << 4) | (cp >> 12)));

  } else if (cp <= 0x10FFFFul) {
    buffer.push_back(static_cast<char>((2 << 6) | (cp & 0x3F)));
    buffer.push_back(static_cast<char>((2 << 6) | ((cp >> 6) & 0x3F)));
    buffer.push_back(static_cast<char>((2 << 6) | ((cp >> 12) & 0x3F)));
    buffer.push_back(static_cast<char>((30 << 3) | (cp >> 18)));
  }
}

static bool parse_entity(boost::string_ref::const_iterator begin, boost::string_ref::const_iterator end, std::string& to) {
  if (begin[1] == '#') {
    char *tail = NULL;
    int errno_save = errno;
    bool hex = begin[2] == 'x' || begin[2] == 'X';

    errno = 0;
    unsigned long cp = strtoul(&(*begin) + (hex ? 3 : 2), &tail, hex ? 16 : 10);

    bool fail = errno || tail != end || cp > UNICODE_MAX;
    errno = errno_save;
    if (fail)
      return false;

    putc_utf8(cp, to);

    return true;

  } else {
    boost::string_ref entry = get_named_entity(boost::string_ref(begin, end - begin));
    to.append(RANGE(entry));
    return true;
  }

  return false;
}

size_t decode_entities(const boost::string_ref src, std::string& dest) {
  dest.clear();
  if (src.empty())
    return 0;

  boost::string_ref::iterator current_begin = src.begin();
  boost::string_ref::iterator end_input = src.end();
  boost::string_ref::iterator current;

  while ((current = std::find(current_begin, end_input, '&')) != end_input) {
    if (current_begin != current) {
      dest.append(current_begin, current);
    }

    auto end = std::find(current + 1, end_input, ';');
    if (end == end_input) {
      return 0;
    }
    if (std::distance(current, end) < 2) {
      current_begin = end + 1;
      continue;
    }

    if (parse_entity(current, end, dest)) {
      current_begin = end + 1;
      continue;
    }

    current_begin = end + 1;
  }

  dest.append(current_begin, end_input);

  return 0;
}

} // namespace mdf
