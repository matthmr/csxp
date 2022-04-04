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
The method of parsing is *callback driven* the same way [SD](https://www.github.com/matthmr/sd) is.
