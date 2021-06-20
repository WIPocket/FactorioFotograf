mapInfo = JSON.parse(mapInfo); // mapInfo comes from mapInfo.json

width  = Math.abs(mapInfo.minx) + Math.abs(mapInfo.maxx)
height = Math.abs(mapInfo.miny) + Math.abs(mapInfo.maxy)

var map = L.map('map', {
	crs: L.CRS.Simple,
	center: [0, 0],
	zoom: 0,
})


L.tileLayer('./images/0/nauvis/day/0/{x}/{y}.png', {
	attribution: '<a href="https://github.com/ProkopRandacek/FactorioFotograf">FactorioFotograf</a>',
	maxZoom: 0,
	minZoom: 0,
	maxNativeZoom: 0,
	minNativeZoom: 0,
	zoomOffset: 0,
	noWrap: true,
	tileSize: mapInfo.block_size * mapInfo.pixels_per_tile / 4,
}).addTo(map)

L.TileLayer.prototype.getTileUrl = function(c) {
	if (c.x >= 0 && c.x <= width && c.y >= 0 && c.y <= height) {
		return './images/0/nauvis/day/0/' + c.x + '/' + c.y + '.png'
	} else { // tile is out of bounds
		return ""
	}
}

