// Made by mH (https://github.com/matthmr)

/// your config goes in this guard {{{
#ifndef LOCK_CONFIG
#  define LOCK_CONFIG

// load the actions first ...
#  include "presets/actions/archlinux-rss.h"

// ... then the main config ...
#  include "csxp.h"

// ... then the presets
#  include "presets/archlinux-rss.h"

// REMEBER: each preset file has an action file associated with it,
//          the return value and arguments of an action MUST be the
//          same for all presets

const Preset def_preset = archlinuxrss;

const PresetList presets[] = {
	{ archlinuxrss, "archlinux"},
	{ archlinuxrss, NULL }, // the first field here doesn't matter as long as it's valid
};

#endif
/// }}}

#ifndef LOCK_INFO
#  define LOCK_INFO

#  define HELP "\nMade by mH (https://github.com/matthmr)\n"\
               "\tcsxp\t\t  => Customizable & Simple XML Parser\n\n"\
               "Usage:\tcsxp -f <file> \t  => Read from <file> (not used = STDIN)\n"\
               "\tcsxp -p <preset>  => Use <preset> as preset\n"\
               "\nInfo:\tcsxp -h\t\t  => Displays this message and exits\n"\
               "\tcsxp -v\t\t  => Displays the version and exits\n"\
               "\tcsxp -l\t\t  => Lists available presets and exits\n\n"\
               "Note:\trunning csxp like \"command | csxp\" will cause it to choose the default preset\n"\
               "     \tcsxp is not SD compatible yet\n"

#  define VERSION "csxp v0.1.0"

#endif
