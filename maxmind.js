pp = function(p){
  return sys.puts(sys.inspect(p));
};

var test = require("./build/default/binding");
var sys = require("sys");
var db = new test.DB();
pp(db.opendb("stuff/GeoLiteCity.dat"));
pp(db.record_by_addr("74.72.38.97"));
pp(db.record_by_addr("sdfsdsdfsd"));