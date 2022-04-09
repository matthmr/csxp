/**
 * preset for:
 *      https://www.archlinux.org/feeds/packages
 */

#ifndef LOCK_PRESET__ARCHLINUX_RSS
#  define LOCK_PRESET__ARCHLINUX_RSS

/********************
 * <pkg>:           *
 *    <description> *
 ********************/

// locals
#  define FIELDS 2
#  define field1 0
#  define field2 1

// macros
#  define AVAILABLE (_iter[i-1] < iter[i].iter.leq)
#  define MATCH(x) (_iter[x] == iter[x+1].iter.leq)
#  define START_I (__iter_len - _iter_len + 1)

Common common[] = {
	BEGIN
	{
		.string = "rss",
		.len = 4,
	},
	{
		.string = "channel",
		.len = 8,
	},
	{
		.string = "item",
		.len = 5,
	},
	END
};

Iter iter[] = {
	{
		.len = 2,
	},
	{
		.iter = {
			.string = "title",
			.leq = 1,
			.len = 5,
		}
	},
	{
		.iter = {
			.string = "description",
			.leq = 1,
			.len = 11,
		}
	},
};

unsigned int _iter[FIELDS]; // iteration number per field
unsigned int _iter_len = FIELDS;
unsigned int __iter_len = FIELDS;

struct fmtbuffer {
	unsigned int glen; // global length
	unsigned int clen; // current length
};

char* lbuffer[FIELDS];
struct fmtbuffer lbufferlen[FIELDS];

unsigned char field;
bool ltoken;

__action_enter_ret__ action_enter (unsigned int offset) {

	Iter* iter = p.preset.iter;

	// loop through the iterable fields looking for the one that matches;
	// then assign that as the new iterable parent
	for (unsigned int i = START_I; i <= __iter_len; i++)

		if (iter[i].iter.len == offset)

			if (AVAILABLE &&
				!strncmp (
					(const char*)nbuffer,
					(const char*)iter[i].iter.string,
					offset)
			)

			{
				ltoken = 1;
				_iter[i-1]++;
				_iter_len--;
				return;
			}

			else if (! AVAILABLE)
				continue;

}

void clean_iter (void) {
	for (unsigned int i = 0; i < FIELDS; i++)
		_iter[i] = 0;
}

#  define ESC                     "\x1b["
#  define __RESET__               ESC "0m"
#  define __BOLD__(x)             ESC "1m"  x __RESET__
#  define __BOLD_RED__(x)         ESC "1;31m" x __RESET__
#  define __BOLD_LIGHT_GREEN__(x) ESC "1;92m" x __RESET__
#  define __BOLD_DARK_GREEN__(x)  ESC "1;32m" x __RESET__
#  define __FORMAT__ \
	__BOLD_RED__               ("rss/") \
	__BOLD__                   ("%s") \
	__BOLD_LIGHT_GREEN__       (" %s") \
	__BOLD_DARK_GREEN__        (" (%s)") \
	                           "\n\t%s\n"

#  define __LIST_ITEM__ __FORMAT__,\
	_fmt[0],\
	_fmt[1],\
	_fmt[2],\
	lbuffer[field2]

void fmtheader (char** __fmt) {

	char* str = lbuffer[field1];
	char* next = str;

	for (unsigned int i = 0; i < 3; i++) {
		if (i < 2) {
			next = strchr (str, ' ');
			*next = '\0';
			__fmt[i] = str;
			str = next+1;
		}
		else
			__fmt[i] = str;

	}

}

void clean_lbuffer (void) {

	bool _field;
	char* _str;

	unsigned int clen;

	unsigned int amount;
	unsigned int calls;
	unsigned int rem;

	for (unsigned int i = 0; i < FIELDS; i++) {

		_field = i;
		clen = lbufferlen[_field].clen;
		_str = lbuffer[_field];

		amount = (
			&lbuffer[_field][lbufferlen[_field].glen] -
			&lbuffer[_field][lbufferlen[_field].clen]
		);

		if (! amount)
			continue;

		calls = amount / 4;
		rem = amount % 4;

		unsigned int j;

		if (calls)
			*(int*)&_str[clen] = 0; // honestly, C impresses my sometimes...

		else for (j = 0; j < rem; j++)
			_str[clen + j] = '\0';

	}

}

void print (void) {

	char* _fmt[3];

	clean_lbuffer ();

	fmtheader ((char**)_fmt);
	printf (__LIST_ITEM__);

}

__action_exit_ret__ action_exit (void) {

	if (field == field2) {
		_iter_len = 2;
		field = field1;

		_common = 0;
		parent--;

		clean_iter ();
		print ();
	}
	else
		field++;

}

__action_load_ret__ action_load (char* match_buffer, char* base_buffer) {

	unsigned int len = match_buffer - base_buffer;
	Iter* iter = p.preset.iter;

	if (MATCH (field)) { // arrange first two fields

		unsigned int llen = lbufferlen[field].glen;

		if (! llen || llen < len)
			lbufferlen[field].glen = len;

		if (llen < lbufferlen[field].glen) {
			llen = lbufferlen[field].glen;

			// `lbuffer[field1]` has to have an extra character
			// because we want to format it
			lbuffer[field] = (char*) realloc (lbuffer[field], (field == field1)? llen + 1: llen);
		}

		lbufferlen[field].clen = len; // (field == field1)? len + 1: len;
		strncpy (lbuffer[field], base_buffer, len);

	}

}

const Preset archlinuxrss = {
	.type = ITER_VAL,

	.common = common,
	.iter = iter,

	.action_enter = &action_enter,
	.action_exit = &action_exit,
	.action_load  = &action_load,
};

#endif
