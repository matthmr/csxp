// Made by mH (https://github.com/matthmr)

#include "csxp.h"

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

/// your config goes in this guard {{{
#ifndef LOCK_CONFIG
#  define LOCK_CONFIG

#  include "presets/archlinux-rss.h"

const Preset def_preset = archlinuxrss;
const PresetList presets[] = {
	{ archlinuxrss, "archlinux"},
	{ archlinuxrss, NULL }, // the first field doesn't matter
};

#endif
/// }}}
