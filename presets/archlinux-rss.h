/**
 * preset for:
 *      https://www.archlinux.org/feeds/packages
 */

#ifndef LOCK_PRESET__ARCHLINUX_RSS
#  define LOCK_PRESET__ARCHLINUX_RSS

/********************
 * <pkg>: <version> *
 *    <description> *
 ********************/

// locals
#  define FIELDS 3

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
			.len = 6,
		}
	},
	{
		.iter = {
			.string = "description",
			.leq = 1,
			.len = 12,
		}
	},
};

unsigned int* _iter = NULL;
unsigned int* _iter_len = 0;

char* lbuffer[FIELDS];
unsigned int lbufferlen[FIELDS];

__action_enter_ret__ action_enter (unsigned int offset) {

	if (! _iter) {

		_iter = (unsigned int*)
		calloc (
			(*_iter_len = p.preset.iter->len),
			sizeof (unsigned int)
		);

	}

	Iter* iter = p.preset.iter;

	for (unsigned int i = *_iter_len - 1; i >= 0; i--) {
		if (iter[i].iter.len == offset) {

			if (_iter[i] < iter[i].iter.leq &&
			!strncmp (
				(const char*)nbuffer,
				(const char*)iter[i].iter.string,
				offset)
			) {
				_iter[i]++;
			}
			else
				return;

		}
	}
	

}

__action_exit_ret__ action_exit (unsigned int offset) {
	_iter[0]--;
}

void arrange_f2f (void) {

}

__action_load_ret__ action_load (char* bbuffer, char** cbuffer) {
	if (_iter[0] == 1) { // arrange first two fields
		arrange_f2f ();
	}

	else { // arrange last field

		// print
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
