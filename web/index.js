mapInfo = JSON.parse(mapInfo); // mapInfo comes from mapInfo.json
var southWest = L.latLng(0, 0),
	northEast = L.latLng(Math.abs(mapInfo.maxx) + Math.abs(mapInfo.minx), Math.abs(mapInfo.maxy) + Math.abs(mapInfo.miny)),
	bounds = L.latLngBounds(southWest, northEast); 
var map = L.map('map').setView([0, 0], 0).setMaxBounds()
L.tileLayer('./images/0/nauvis/day/0/{x}/{y}.png', {
	attribution: '<a href="https://github.com/ProkopRandacek/FactorioFotograf">FactorioFotograf</a>',
	maxZoom: 3,
	minZoom: 0,
	maxNativeZoom: 3,
	minNativeZoom: 0,
	zoomOffset: 0,
	noWrap: true,
	tileSize: 128,
}).addTo(map)
L.TileLayer.prototype.getTileUrl = function(c) {
	console.log('./images/0/nauvis/day/0/' + c.x + '/' + c.y + '.png zoom ' + c.z)
	return      './images/0/nauvis/day/0/' + c.x + '/' + c.y + '.png'
}

