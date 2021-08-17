ext = ".jpg"

var mapInfo = JSON.parse(mapInfo); // mapInfo comes from mapInfo.js (generated at runtime)
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
	zoomReverse: true,
	zoomOffset: 0,
	noWrap: true,
	tileSize: mapInfo.ppt * 32,
}).addTo(map)

