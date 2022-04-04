// Made by mH (https://github.com/matthmr)

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "config.h"
#include "csxp.h"

#define INC(x) x++; goffset++

#define matchg resolve.group // enum promise -- group from `matcher`
#define matchstr resolve.str // unsigned int -- string from `@matcher`
#define matchch resolve.ch   // unsigned int -- char from `matcher/@string`

#define matcher ((char**)(matches[matchg]))

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

static promise p = {
	.HFILE = false,
	.HPRESET = false,
	.PFILE = false,
	.PPRESET = false,
	.file = NULL,
	.preset = def_preset,
};
static int ENO;
static char fbuffer[__IO__];

static arg parseargs (int, char**);
static int parse (FILE*);
static bool pload (char*);
static void list_presets (void);
static callback feed (char*);
static void pcomp (void);
static callback ensurename (char**);
static callback seek (char**, char, Compound);
static callback getdata (char**, int);
static int die (void);

arg parseargs (int margc, char** margv) {

	char* s;
	char c;

	if (margc == 1)
		goto def;

	for (unsigned int i = 1; i < margc; i++) {

		c = *margv[i];

		// resolve promise -- file
		if (p.PFILE) {

			if (p.HFILE)
				return ERR;

			else if (margv[i][0] == '-' && margv[i][1] == '\0') // -f -
				goto pfile_reset;

			else if (! (p.file = fopen (s = margv[i], "rb"))) {
				ENO = NO_SUCH_FILE;
				fprintf (stderr, "No such file \"%s\"\n", s);
				return ERR;
			}

			pfile_reset: {
				p.PFILE = false;
				p.HFILE = true;
			}

		}

		// resolve promise -- preset
		else if (p.PPRESET) {

			if (p.HPRESET)
				return ERR;

			else if (! pload (s = margv[i])) {
				ENO = NO_SUCH_PRESET;
				fprintf (stderr, "No such preset \"%s\"\n", s);
				return ERR;
			}

			ppreset_reset: {
				p.PPRESET = false;
				p.HPRESET = true;
			}

		}

		// set promise
		else if (c == '-') {

			// info
			if ((c = margv[i][1]) == 'v') {
				if (margv[i][2] != '\0')
					return ERR;

				puts (VERSION);
				return INFO;
			}
			else if (c == 'h') {
				if (margv[i][2] != '\0')
					return ERR;

				puts (HELP);
				return INFO;
			}

			else if (c == 'l') {
				if (margv[i][2] != '\0')
					return ERR;

				list_presets ();
				return INFO;
			}


			// set promise -- file
			else if (c == 'f') {
				if (margv[i][2] != '\0') {
					ENO = NOT_SUPPORTED;
					return ERR;
				}

				p.PFILE = true;
			}

			// set promise -- preset
			else if (c == 'p') {
				if (margv[i][2] != '\0') {
					ENO = NOT_SUPPORTED;
					return ERR;
				}

				p.PPRESET = true;
			}

			// unknown option
			else
				return ERR;
		}

		// fallthrough
		else
			return ERR;

	}

	def: {
		if (p.PFILE || p.PPRESET)
			return ERR;

		if (! p.file)
			p.file = stdin;
		return OK;
	}

}

int parse (FILE* file) {

	char* _fbuffer;
	char** __fbuffer;

	static unsigned int rbuffer;
	static bool magic = 1;

_buffer:
	_fbuffer = (char*)fbuffer;
	__fbuffer = &_fbuffer;
	rbuffer = fread (fbuffer, sizeof (char), __IO__, file);

	if (magic) {

		if (rbuffer < 5 || strncmp (fbuffer, "<?xml", 5)) {
			fputs ("Missing \"<?xml\" magic number\n", stderr);
			return 1;
		}

		magic = 0;
		_fbuffer = &_fbuffer[5];

		seek (__fbuffer, '>', XML_NULLC); // TODO: implement attribute seeking if needed
		_fbuffer++; // '>'
		goffset += _fbuffer - fbuffer;

	}

	callback __parse__ = _OK;

	if (rbuffer < __IO__)
		;

	__parse__ = feed (_fbuffer);

	switch (__parse__) {

	case _OK:
		goto _buffer;
		break;

	case _EOL:
		break;

	case _MORE:
		break;

	case _MORE_LIM:
		break;

	case _IGNORE:
		break;

	case _END:
		return 0;
		break;

	}

}

bool pload (char* preset_arg) {

	int i = 0;
	char* preset_string;

	while ((preset_string = (char*)presets[i].string) != NULL) {
		if (! strcmp ((const char*)preset_arg, (const char*)preset_string)) {
			p.preset = presets[i].preset;
			return true;
		}
		i++;
	}

	return false;

}

void list_presets (void) {

	char* preset;
	int i;

	puts ("presets available:");

	while ((preset = (char*)presets[i].string) != NULL) {
		printf (" * %s\n", preset);
		i++;
	}

}

callback feed (char* buffer) {

	Compound compound = XML_NULLC;

	static char* _buffer;
	static char* _match;

	static unsigned int offset;

	if (! _buffer)
		_buffer = buffer;

_feed_reset:
	callback __feed__ = _OK;

	if (parent == end)
		return _END;

	else if (matchg == PTAG_CLOSE) {
		matchg = PTAG_OPEN; // get data; store it; format it; print it
	}

_get_match:
	_match = matcher[matchstr];

	_buffer = strchr (_buffer, _match[matchch]);

	if (! _buffer)
		;

	offset = _buffer? (_buffer - buffer): 0;
	goffset += _buffer? (_buffer - buffer): 0;

	if (_buffer) {
		_buffer += offset; // <
		goffset += offset;
		matchg = PTAG_CLOSE;

_ensure_name:
		__feed__ = ensurename (&_buffer);

		switch (__feed__) {

		case _OK:
			compound = XML_NULLC;
			goto _seek;
			break;

		case _MORE:
			return _MORE;
			break;

		case _IGNORE:
_seek:
			matchstr = 1;
			if (seek (&_buffer, matcher[matchstr][matchch], compound) == _MORE)
				return _MORE;
			else {
				INC (_buffer); // <
				matchg = PTAG_CLOSE;
				matchstr = 0;
				goto _feed_reset;
			}
			break;

		}

		_close_group: goto _get_match;

	}

	goto _get_match;

}

void pcomp (void) {

	unsigned int n = 1;

	int i = 0, j = 0;

	while (p.preset.common[n].string != NULL) {
		i = p.preset.common[n].len;

		if (j < i)
			j = i;

		n++;
	}

	nbufferlen = (unsigned int)j;
	nbuffer = realloc (nbuffer, nbufferlen);
	parent = p.preset.common;
	end = &p.preset.common[n];

}

callback ensurename (char** _tbuffer) {

	unsigned int noffset;

	char* tbuffer = *_tbuffer;
	char* space_tbuffer = NULL;
	char* close_tbuffer = NULL;

	if (goffset + (nbufferlen - 1) >= __IO__)
		return _MORE;

	close_tbuffer = strchr (tbuffer, '>');

	if (! close_tbuffer)
		return _MORE;

	if (close_tbuffer - tbuffer > nbufferlen) { // overshot == there is a space after[1] / before[2]

		if (*tbuffer == ' ') { // [2] check if there is a space before ...
			char* tchar;
			for (unsigned int i = 0; tchar = &tbuffer[i]; i++) // ... then adjust the buffer for it
				if (*tchar != ' ') {
					*_tbuffer = tchar;
					goffset += i;
				}
		}

		space_tbuffer = strchr (tbuffer, ' '); // [1] check if there is a space after

		if (! space_tbuffer)
			;
		else if (space_tbuffer - tbuffer > nbufferlen) { // overshot == name is bigger than the preset's
			char* __tbuffer = tbuffer;
			tbuffer = close_tbuffer;
			goffset += tbuffer - __tbuffer;
			return _IGNORE;
		}

	}

	if (! space_tbuffer)
		noffset = close_tbuffer - tbuffer;

	else // calculate new offset
		noffset = space_tbuffer - tbuffer;

	if (! noffset)
		;

	strncpy (nbuffer, tbuffer, noffset);
	char* __tbuffer = tbuffer;
	*_tbuffer = space_tbuffer? space_tbuffer+1: close_tbuffer+1;
	goffset += space_tbuffer + 1 - __tbuffer;

	if (! strncmp ((const char*)nbuffer,
	               (const char*)(parent+1)->string,
	                            noffset)) {
		parent++;
		return _OK;
	}

	else
		return _IGNORE;

}

callback seek (char** _tbuffer, char __match, Compound compound) {

	char* seekee;
	char* tbuffer = *_tbuffer;

	if (! compound)
		;

	seekee = strchr (tbuffer, __match);

	if (! seekee)
		return _MORE;

	*_tbuffer = seekee;
}

int die (void) {
	fputs ("usage: csxp [-f <file>] [-p <preset>]\n", stderr);
	return ENO;
}

int main (int argc, char** argv) {

	arg ret = parseargs (argc, argv);

	if (ret == INFO)
		goto ok;

	else if (ret != OK)
		return die ();

	else {
		pcomp ();
		return parse (p.file);
	}

	ok: return 0;

}
