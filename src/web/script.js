ext = ".png"

var mapInfo = JSON.parse(mapInfo); // mapInfo comes from mapInfo.js

var permalink = L.Permalink.getMapLocation(5, [0, 0]);

var map = L.map('map', {
	crs: L.CRS.Simple,
	center: permalink.center,
    zoom: permalink.zoom
})

L.Permalink.setup(map);

L.tileLayer('./images/0/{z}/{x}_{y}' + ext, {
	attribution: '<a href="https://github.com/ProkopRandacek/FactorioFotograf">FactorioFotograf</a>',
	maxZoom: 8,
	minZoom: 0,
	zoomOffset: 0,
	noWrap: true,
	tileSize: mapInfo.block_size * mapInfo.pixels_per_tile,
}).addTo(map)

