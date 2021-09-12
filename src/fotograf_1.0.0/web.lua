web_html = [[
<!DOCTYPE html>
<html lang="en" style="margin:0px;height:100%">
	<head>
		<title>FactorioFotograf</title>
		<meta http-equiv="content-type" content="text/html; charset=utf-8"/>
		<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1">
		<link rel="stylesheet" href="https://unpkg.com/leaflet@1.7.1/dist/leaflet.css"/>
		<script src="https://unpkg.com/leaflet@1.7.1/dist/leaflet.js"></script>
		<script src="https://rdck.dev/dl/leaflet.permalink.min.js"></script>
		<script src="./mapInfo.js"></script>
	</head>
	<body style="margin:0px;height:100%">
		<div id="map" style="height:100%;background:#222831"></div>
		<script src="./script.js"></script>
	</body>
</html>
]]

web_js = [[
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
	maxZoom: 5,
	minZoom: 0,
	zoomReverse: true,
	zoomOffset: 1,
	noWrap: true,
	tileSize: mapInfo.ppt * 32,
}).addTo(map)
]]

