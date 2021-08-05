#!/bin/sh
if [ $# -eq 0 ]; then
	echo "No save name supplied"
	exit 1
fi

function editmodlist {
	# todo: this is unbelievably slow
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
	maxx=$(cat ./$1/mapInfo.json|jq '.maxx')
	maxy=$(cat ./$1/mapInfo.json|jq '.maxy')
	minx=$(cat ./$1/mapInfo.json|jq '.minx')
	miny=$(cat ./$1/mapInfo.json|jq '.miny')

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

factorio --load-game $1 1> /dev/null&
echo "If you misstyped the savename, load the game you want to capture manually."

echo "Waiting for the game to finish capturing..."
until [ -f ~/.factorio/script-output/done ]; do
	if [ -d ~/.factorio/script-output/images/ ]; then
		printf "\r%s/??? files created" $(find ~/.factorio/script-output/images/ -type f | wc -l)
	fi
	sleep 1
done
echo

sleep 3 # just to be sure
echo "Capturing done. Killing Factorio."
killall factorio

echo "Creating web folder"
mkdir $1
cp ./web/index.html $1/
cp ./web/script.js  $1/

echo "Moving images"
mv ~/.factorio/script-output/images/      $1/
mv ~/.factorio/script-output/mapInfo.json $1/

echo "Generating js scripts"
printf "mapInfo = '" > $1/mapInfo.js
cat $1/mapInfo.json >> $1/mapInfo.js
printf "'" >> $1/mapInfo.js

echo "Restoring mod settings"
./editmodlist ~/.factorio/mods/mod-list.json false

# generate a blank tile with the right size
imgres=$(cat $1/mapInfo.json | jq ".image_resolution")
convert blank.png -resize ${imgres}x${imgres} -colorspace RGB $1/images/blank.png

echo Compiling image merge tool
cd c && make && cd ..

zoomout $1 8 7 # lol maybe loop?
zoomout $1 7 6
zoomout $1 6 5
zoomout $1 5 4
zoomout $1 4 3
zoomout $1 3 2
zoomout $1 2 1
zoomout $1 1 0

echo Done
