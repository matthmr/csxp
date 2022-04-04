/**
 * preset for:
 *      https://www.archlinux.org/feeds/packages
 */

#ifndef LOCK_ARCHLINUXRSS
#  define LOCK_ARCHLINUXRSS

#  include "../csxp.h"

Common common[] = {
	BEGIN
	{ "rss", 4 },
	{ "channel", 8 },
	{ "item", 5 },
	END
};

Iter iter[] = {
	{
		.string = "title",
		.leq = 1
	},
	{
		.string = "description",
		.leq = 1
	},
};

const Preset archlinuxrss = {
	.type = ITER_VAL,
	.common = common,
	.iter = iter,
};

#endif
