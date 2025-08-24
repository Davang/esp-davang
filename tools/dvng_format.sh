#!/bin/bash

if [[ -z "${DAVANG_ESP}" ]]; then
  echo this script shall be run with the davan esp env variable set
  exit 1
fi

list_dirs()
{
	echo $(find $1 -mindepth 1 -maxdepth 1 -type d )
}

list_files()
{
	echo $(find $1 -not -path "$1/build/*" -regex '.*\.\(c\|cpp\|h\|hpp\)$' )
}

top_dirs="$DAVANG_ESP/examples $DAVANG_ESP/components"

for top_dir in $top_dirs;
do
	for dir in $(list_dirs $top_dir);
	do
		files+=' '
		files+=$(list_files $dir );
	done
done

uncrustify -q -l CPP -c $DAVANG_ESP/tools/etc/uncrustify.cfg --replace --no-backup $files > /dev/null