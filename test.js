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

var posix = require('fs'),
    sys = require('sys'),
    http = require("http"),
    maxmind = require("./maxmind");

pp = function(p){
  return sys.puts(sys.inspect(p));
};

db = new maxmind.DB();
db.opendb("../stuff/GeoLiteCity.dat");

rl = function(request,response){
  var body = ' ';
  if(request.url == '/geo' && request.method == 'GET'){
    sys.puts('Geo ....');
    // var ip = request.connection.remoteAddress;
    var ip = "72.14.204.17";
    var res = db.record_by_addr(ip);
    body = sys.inspect(res);
  }
  response.writeHeader(200, {
    "Content-Length": body.length,
    "Content-Type": "text/plain"
  });

  response.write(body);
  response.end();
  sys.puts('return ..');
};

var srv = http.createServer(rl);
srv.listen('9999', '0.0.0.0');
