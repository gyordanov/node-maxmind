/*
Copyright 2010 Galin Yordanov

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

pp = function(p){
  return sys.puts(sys.inspect(p));
};

var test = require("./maxmind");
var sys = require("sys");
var db = new test.DB();
pp(db.opendb("../stuff/GeoLiteCity.dat"));
pp(db.record_by_addr("72.14.204.17"));
pp(db.record_by_addr("sdfsdsdfsd"));
