#!/bin/sh
if [ $# -eq 0 ]; then
	echo "No save name supplied"
	exit 1
fi

function editmodlist { # todo: this is unbelievably slow
	modlist=$1
	newval=$2
	tmp=$(mktemp)

	len="$(cat $modlist | jq ".mods | length")"
	done=false

	for i in $(seq 0 $(( len - 1 ))); do
		mod="$(cat $modlist | jq -r ".mods[$i].name")"
		if [ "$mod" = "fotograf" ]; then
			cat $modlist | jq ".mods[$i].enabled = $newval" > $tmp
			mv $modlist $modlist.orig
			mv $tmp $modlist
			done=true
		fi
	done

	if [ "$done" = false ]; then
		cat $modlist | jq ".mods[$len] = {\"name\": \"fotograf\", \"enabled\": $newval}" > $tmp
		mv $modlist $modlist.orig
		mv $tmp $modlist
	fi
}

function zoomout {
	maxx=$(cat ./$1/mapInfo.json | jq '.maxx')
	maxy=$(cat ./$1/mapInfo.json | jq '.maxy')
	minx=$(cat ./$1/mapInfo.json | jq '.minx')
	miny=$(cat ./$1/mapInfo.json | jq '.miny')
	mkdir -p $1/images/0/$3/
	./imgmerge $1 $2 $3 $maxx $maxy $minx $miny
}

echo "Start"

echo "Injecting FactorioFotograf into the game"
ln -sf $(pwd)/fotograf_1.0.0 ~/.factorio/mods/

echo "Enabling FactorioFotograf mod"
editmodlist ~/.factorio/mods/mod-list.json true

echo "Clearing script output"
rm ~/.factorio/script-output/images/ -rf
rm ~/.factorio/script-output/mapInfo.json -f
rm ~/.factorio/script-output/done -f
rm ./$1 -rf
mkdir -p $1

factorio --load-game $1 1> /dev/null&
fac_pid="$!"

echo "Waiting for the game to finish capturing..."
until [ -f ~/.factorio/script-output/done ]; do
	if [ -d ~/.factorio/script-output/images/ ]; then
		printf "\r%s screenshots taken" $(find ~/.factorio/script-output/images/ -type f | wc -l)
	fi
	sleep 0.2
done
echo

echo "Capturing done. Closing Factorio."
kill $fac_pid

echo "Moving images"
mv ~/.factorio/script-output/images/      $1/
mv ~/.factorio/script-output/mapInfo.json $1/
imgres=$(cat $1/mapInfo.json | jq ".image_resolution") #  generate blank tile
convert blank.png -resize ${imgres}x${imgres} -colorspace RGB $1/images/blank.png

echo "Creating web files"
cp ./web/* $1/ # copy the html and js script
printf "mapInfo = '" > $1/mapInfo.js # generate js script with map info to read from
cat $1/mapInfo.json >> $1/mapInfo.js
printf "'" >> $1/mapInfo.js

echo "Restoring mod settings"
editmodlist ~/.factorio/mods/mod-list.json false

echo Compiling image merge tool
cd c
	make
cd ..

for n in $(seq 8 -1 1); do
	zoomout $1 $n $(expr $n - 1)
done

echo Done

