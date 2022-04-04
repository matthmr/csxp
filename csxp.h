// Made by mH (https://github.com/matthmr)

#ifndef LOCK_CSXP
#  define LOCK_CSXP

// `NULL`
#  ifndef NULL
#    define NULL (void*)0
#  endif

// `common`
#  define END { NULL, 0 }
#  define BEGIN END,

// `bool`
#  define false 0
#  define true 1
typedef unsigned char bool;

// `feed_line`
#  define _OK 0
#  define _EOL 1
#  define _MORE 2
#    define _MORE_LIM 3
#    define _MORE_SEEK_LIM 4
#    define _MORE_SEEK 5
#  define _IGNORE 6
#  define _END 7

// <--> `arg`
#  define INFO -1
#  define OK 0
#  define ERR 1
#    define NOT_SUPPORTED 101
#    define NO_SUCH_PRESET 202
#    define NO_SUCH_FILE 404
typedef char callback;

// utils
#  define POW2(x) (unsigned)(1 << x)
#  define KiB POW2 (10)
#  define __IO__ 4*KiB
#  define LENGTH(x) (sizeof (x) / sizeof (*x))
#  define WHITESPACE(x) (x == ' ' || x = '\t' || x = '\r' || x = '\t')

// `struct preset`
enum type {
	ITER_VAL,  // DONE
	ITER_ATTR, // TODO
	ONCE,      // TODO
	SO_THAT,   // TODO
};

// `struct preset`
struct iter {
	char* string;
	unsigned int leq;
};

// `struct preset`
struct common {
	char* string;
	unsigned int len;
};

// `struct presetlist`
struct preset {
	enum type type;
	struct common* common;
	struct iter* iter;
};

struct presetlist {
	const struct preset preset;
	const char* string;
};

typedef enum type Type;
typedef struct common Common;
typedef struct iter Iter;
typedef struct preset Preset;
typedef struct presetlist PresetList;

#endif

#ifndef LOCK_XML
#  define LOCK_XML

const char* tag_open[] = { "<", ">" };
const char* tag_close[] = { "</", ">" };
// const char* attr[] = { "\"", "\"" }; // not implemented
// const char* comment[] = { "<!--", "-->" }; // not implemented

enum promise {
	PTAG_OPEN = 0,
	PTAG_CLOSE,
	// PATTR, // not implemented
	// PCOMMENT, // not implemented
};

struct xml_promise {
	enum promise group;

	unsigned int str;
	unsigned int ch;
};

const char** matches[2] = {
	[PTAG_OPEN] = tag_open,
	[PTAG_CLOSE] = tag_close,
	// [PATTR] = attr, // not implemented
	// [PCOMMENT] = comment, // not implemented
};

enum compound {
	XML_NULLC = 0,   // >
	XML_CLOSEC,      // </
};

const unsigned int xml_matchers = 1; // indexed
struct xml_promise resolve;

struct common* parent;
struct common* end;

unsigned int goffset = 0;
unsigned int nbufferlen;
char* nbuffer;

typedef enum compound Compound;

#endif
