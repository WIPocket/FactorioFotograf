ext = ".jpg"
time = "TIME"

var mapInfo = JSON.parse(mapInfo); // mapInfo comes from mapInfo.js
var permalink = link.getMapLocation(5, [0, 0]);
var map = L.map('map', {
	crs: L.CRS.Simple,
	center: permalink.center,
    zoom: permalink.zoom
})

link.setup(map);

L.tileLayer('./images/0/{z}/{x}_{y}' + ext, {
	attribution: `${time} | <a href="https://github.com/ProkopRandacek/FactorioFotograf">FactorioFotograf</a>`,
	maxZoom: 8,
	minZoom: 0,
	zoomOffset: 0,
	noWrap: true,
	tileSize: mapInfo.pixels_per_tile * 32,
}).addTo(map)

