// Made by mH (https://github.com/matthmr)

#ifndef LOCK_CSXP
#  define LOCK_CSXP

// `NULL`
#  ifndef NULL
#    define NULL (void*)0
#  endif

// `common`
#  define END { "", 0 },
#  define BEGIN END

// `bool`
#  define false 0
#  define true 1
typedef unsigned char bool;

// `feed_line`
#  define _OK 0
#  define _EOL 1
#  define _MORE 2
#  define _IGNORE 6
#  define _END 7
#  define _ERR 8

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
#  define WHITESPACE(x) (x == ' ' || x == '\t' || x == '\r' || x == '\t')

// XML standard
#  define __MAGIC__ "<?xml"

// `struct preset`
enum type {
	ITER_VAL,  // DONE
	ITER_ATTR, // TODO
	ONCE,      // TODO
	SO_THAT,   // TODO
};

// `union iter`
struct _iter {
	char* string;
	unsigned int leq;
	unsigned int len;
};

union iter {
	unsigned int len;
	struct _iter iter;
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
	union iter* iter;

	__action_enter_ret__ (*action_enter) (__action_enter_args__);
	__action_exit_ret__ (*action_exit) (__action_exit_args__);
	__action_load_ret__ (*action_load) (__action_load_args__);
};

struct presetlist {
	const struct preset preset;
	const char* string;
};

typedef enum type Type;
typedef struct common Common;
typedef union iter Iter;
typedef struct preset Preset;
typedef struct presetlist PresetList;

#endif

#ifndef LOCK_XML
#  define LOCK_XML

const char* tag[] = { "<", ">" };
// const char* attr[] = { "\"", "\"" }; // not implemented
// const char* comment[] = { "<!--", "-->" }; // not implemented

enum promise {
	PTAG = 0,
	// PATTR, // not implemented
	// PCOMMENT, // not implemented
};

struct xml_promise {
	enum promise group;

	unsigned int str;
	unsigned int ch;
};

const char** matches[1] = {
	tag,
	// [PATTR] = attr, // not implemented
	// [PCOMMENT] = comment, // not implemented
};

const unsigned int xml_matchers = 0; // indexed
struct xml_promise resolve;

struct common* parent;
struct common* end;

unsigned int goffset = 0;
unsigned int nbufferlen;

char* nbuffer;
char* cnbuffer;

bool _common = 0; // lock common; begin iterating
bool _close = 0;  // seek close
char* _match;     // matching string from @matches

struct promises {
	char HFILE;
	char HPRESET;

	char PFILE;
	char PPRESET;

	FILE* file;
	Preset preset;
};

typedef char arg;
typedef struct promises promise;

extern promise p;

#endif
