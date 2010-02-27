process.mixin(GLOBAL, require("sys"));
var geoip = require("./maxmind");

var c = geoip.openDB("./stuff/GeoLiteCity.dat");
