# CSXP

The Customizable & Simple XML Parser.

## What?

This program **monolithically** parses XML according to what the user changes in the `/config.h` file
and what preset is used.

## Why?

My main reason was to *beautify* archlinux's [rss package feed](https://www.archlinux.org/feeds/packages). So `csxp` is designed to
handle one file and one preset at a time. But feel free to fork this and add more!

## How?

Read the [manpage](/csxp.1) to learn about the preset syntax if you want to customize it.

## Compile

This repository uses a `Makefile` at root to compile.

Run:
- `make` or `make install` to install `csxp` to `$PREFIX/bin`.
- `make csxp` to just compile `csxp`.
- `make tags` to make ctags' tag file for the source tree.
- `make clean` to clean the compilation junk.
- `make man` to compress man pages.

Pass in the following evironment variables if needed:
- `CC`: the C compiler (gcc)
- `CCFLAG`: additional flags to pass to the compiler ()
- `CTAGS`: ctags program (ctags)
- `GZIP`: gunzip compressor (gzip)
- `PREFIX`: root prefix for install (/usr/local)
