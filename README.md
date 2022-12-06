Creb - Tom Schouten's abstractions & externals library for Pure Data.

This repository will be a merge of Tom Schouten's darcs version from http://zwizwa.be/darcs/creb/ and the pd-extended version from https://git.puredata.info/cgit/svn2git/libraries/creb.git/. As both have seen some independend development, this is an attenpt to merge both. The zwizwa.be version has no commit history (at least in a git readable format) and the git.puredata.info instance does, so the latter is used as the basis. The changes at the zwizwa.be seem rather minor.

To reflect the changes, the version number will be changed to 0.9.3.
The 2022 change for the Makefile.pdlibbuilder directory, modifies the version number to the more standard format 0.10.0.

Compilation (Linux MacOSX) example:
<pre>make PDDIR=../pure-data/</pre>

Compilation (Windows MinGW2)) example:
<pre>make PDDIR=../pure-data/ PDBINDIR=../pdbin64/</pre>

Installation example:
<pre>make PDDIR=../pure-data/ install pkglibdir=~/Documents/Pd/externals/</pre>

Fred Jan Kraan, fjkraan@electrickery.nl, 2022-12-06
