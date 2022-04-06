// Made by mH (https://github.com/matthmr)

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "config.h"

#define exclude_token(x) x++; goffset++
#define calc_goffset(x) goffset=x-fbuffer

#define matchg resolve.group // enum promise -- group from `matcher`
#define matchstr resolve.str // unsigned int -- string from `@matcher`
#define matchch resolve.ch   // unsigned int -- char from `matcher/@string`

#define matcher ((char**)(matches[matchg]))

promise p = {
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
static callback seek (char**, char);
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

	callback __parse__ = _OK;

_buffer:
	_fbuffer = (char*)fbuffer;
	__fbuffer = &_fbuffer;
	rbuffer = fread (fbuffer, sizeof (char), __IO__, file);

	if (goffset)
		goffset == 0;

	if (magic) {

		if (rbuffer < 5 || strncmp (fbuffer, "<?xml", 5)) {
			fputs ("Missing \"<?xml\" magic number\n", stderr);
			__parse__ = _ERR;
			goto _fclose;
		}

		magic = 0;
		_fbuffer = &_fbuffer[5];
		calc_goffset (_fbuffer);

		if (rbuffer < __IO__)
			;

		seek (__fbuffer, '>'); // TODO: implement attribute seeking if needed
		exclude_token (_fbuffer);

	}

	if (rbuffer < __IO__)
		;

	__parse__ = feed (_fbuffer);

	switch (__parse__) {

	case _ERR:
		goto _fclose;
		break;

	case _OK:
		goto _buffer;
		break;

	case _EOL:
		break;

	case _MORE:
		// allocate a separate buffer for the leading sub-array
		goto _buffer;
		break;

	case _IGNORE:
		break;

	case _END:
_fclose:
		fclose (p.file);
		return __parse__;
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

	callback __feed__ = _OK;

	static char* _buffer;
	static char* _match;

	if (! _buffer)
		_buffer = buffer;

_get_match:
	if (parent == end)
		return _END;
	else if (_common || parent + 1 == end)
		_common = 1;

	_match = matcher[matchstr]; // ""
	
	if (*_buffer != '<') {

		char** __buffer = &_buffer;

		_buffer = strchr (_buffer, _match[matchch]); // match with the current (opening) delimiter token
		calc_goffset (_buffer); // calculate current offset to the delimiter token

		if (_common)
			p.preset.action_load (_buffer, __buffer);

	}

_after_common:
	if (! _buffer)
		return _MORE;

	exclude_token (_buffer); // advance current buffer to AFTER the delimiter token

	if (*_buffer == '/') { // if found a '/' character, set the tag as a closing tag ...
		exclude_token (_buffer); // ... and also exclude it
		_close = 1;
	}

_ensure_name:
	__feed__ = ensurename (&_buffer);

	switch (__feed__) {

	case _ERR:
		return _ERR;
		break;

	case _OK:
		goto _seek;
		break;

	case _MORE:
		return _MORE;
		break;

	case _IGNORE:
_seek:

		if (*_buffer == '>')
			goto _skip_seek;

		matchstr = 1;
		if (seek (&_buffer, matcher[matchstr][matchch]) == _MORE)
			return _MORE;
		else _skip_seek: {

			if (*(_buffer - 1) == '/') // self-closing tag
				_close = 0;

			exclude_token (_buffer);
			matchstr = 0;
			goto _get_match;

		}
		break;

	}

	goto _get_match;

}

void pcomp (void) {

	unsigned int n = 1;
	unsigned int _n = 0;

	int i = 0, j = 0;

	while (p.preset.common[n].string != NULL) {
		i = p.preset.common[n].len;

		if (j < i)
			j = i;

		n++;
	}

	parent = p.preset.common;
	end = &p.preset.common[n-1];

	n = 0;

	for (n = p.preset.iter->len; n >= 1; n--) {
		i = p.preset.iter[n].iter.len;
		if (j < i)
			j = i;
	}

	nbufferlen = (unsigned int)j;

	nbuffer = realloc (nbuffer, nbufferlen);
	cnbuffer = realloc (cnbuffer, nbufferlen);

}

callback ensurename (char** _tbuffer) {

	unsigned int noffset;
	unsigned int gindex;

	char* tbuffer = *_tbuffer;
	char* space_tbuffer = NULL;
	char* close_tbuffer = NULL;

	gindex = goffset + nbufferlen;

	if (gindex >= __IO__)
		return _MORE;

	close_tbuffer = strchr (tbuffer, '>'); // find closing tag

	if (! close_tbuffer)
		return _MORE;

	if (*(close_tbuffer - 1) == '/') // self-closing tag
		_close = 0;

	if (close_tbuffer - tbuffer > nbufferlen) { // overshot == there is a space after[1] / before[2]

		if (*tbuffer == ' ') { // [2] check if there is a space before ... (xml syntax error [ignore])

			unsigned int i = 0;
			char t;

			while (gindex + i < __IO__) {
				t = tbuffer[i];
				if (! WHITESPACE (t)) {
					tbuffer += i; // skip all whitespace ...
					goffset += i; // ... add it to the global offset
					break;
				}
				else
					i++;
			}

		}

		space_tbuffer = strchr (tbuffer, ' '); // [1] check if there is a space after (xml attribute [ignored as of v0.1.0])

		if (! space_tbuffer)
			;
		else if (space_tbuffer - tbuffer > nbufferlen) { // overshot == name is bigger than the preset's
			tbuffer = close_tbuffer;
			calc_goffset (tbuffer);

			if (_close)
				_close = 0;

			return _IGNORE;
		}

	}

	if (! space_tbuffer)
		noffset = close_tbuffer - tbuffer;

	else // calculate new offset
		noffset = space_tbuffer - tbuffer;

	if (! noffset)
		;

	strncpy (nbuffer, tbuffer, noffset); // copy name to `nbuffer`

	*_tbuffer = space_tbuffer? (space_tbuffer): (close_tbuffer);
	calc_goffset (*_tbuffer);

	if (_common) {

			if (! _close)
				p.preset.action_enter (noffset);

			else {
				_close = 0;
				p.preset.action_exit (noffset);
			}

		return _OK;
	}

	else if (_close &&
		!strncmp (
			(const char*)nbuffer,
			(const char*)(parent)->string,
			noffset)
	) { // && ! _common
		parent--;
		_close = 0;
		return _OK;
	}

	else if (_close) { // this parser is monolithic; XML syntax errors outside targets are irrelevant
		_close = 0;
		return _IGNORE;
	}

	else if (
		!strncmp (
			(const char*)nbuffer,
			(const char*)(parent+1)->string,
			noffset)
	) {

		if (_close) {
			parent--;
			_close = 0;
			return _OK;
		}

		parent++;
		return _OK;

	}

	else
		return _IGNORE;

}

callback seek (char** _tbuffer, char __match) {

	char* seekee;
	char* tbuffer = *_tbuffer;

	seekee = strchr (tbuffer, __match);

	if (! seekee)
		return _MORE;

	calc_goffset (seekee);
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
		return parse (p.file); // <-- file is closed here
	}

	ok: return 0;

}
