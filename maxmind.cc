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

#include <GeoIP.h>
#include <GeoIPCity.h>
#include <node.h>
#include <node_events.h>
#include <assert.h>


using namespace v8;
using namespace node;
using namespace std;
static Persistent<String> result_symbol;
namespace maxmind {

  class DB : public EventEmitter {
    public:
      GeoIP *gi;
      static void Attach(Handle<Object> target){
        Local<FunctionTemplate> db_template = FunctionTemplate::New(New);
        db_template->Inherit(EventEmitter::constructor_template);
        db_template->InstanceTemplate()->SetInternalFieldCount(1);
        NODE_SET_PROTOTYPE_METHOD(db_template,"opendb",opendb);
        NODE_SET_PROTOTYPE_METHOD(db_template,"record_by_addr",record_by_addr);

        result_symbol = NODE_PSYMBOL("result");

        Persistent<FunctionTemplate> db_persistent = Persistent<FunctionTemplate>::New(db_template);
        target->Set(String::New("DB"),db_persistent->GetFunction());
      }
  
      static Handle<Value> New(const Arguments& args){
        HandleScope scope;
        DB *t = new DB();
        t->Wrap(args.Holder());
        return args.This();
      }
  
      static Handle<Value> opendb(const Arguments& args){
        HandleScope scope;
        if(args.Length() < 0){
          ThrowException(Exception::Error(String::New("You must provide path to the GeoLiteCity.dat database.")));
        }
        DB *db = ObjectWrap::Unwrap<DB>(args.This());
  
        String::Utf8Value db_path(args[0]->ToString());
  
        db->gi = GeoIP_open(*db_path, GEOIP_INDEX_CACHE);
        if (db->gi == NULL) {
          ThrowException(Exception::Error(String::New("Error opening database.")));
        }

        return String::New("ok");
      }

      static Handle<Value> record_by_addr(const Arguments& args){
        HandleScope scope;
        GeoIPRecord    *gir;
        if(args.Length() < 0){
          ThrowException(Exception::Error(String::New("You must provide IP address.")));
        }
        DB *db = ObjectWrap::Unwrap<DB>(args.This());
        if (db->gi == NULL) {
          ThrowException(Exception::Error(String::New("Must open the database first.")));
        }

        String::Utf8Value ipaddr(args[0]->ToString());
        gir = GeoIP_record_by_addr(db->gi, *ipaddr);
        Local<Object> result = Object::New();
        if(gir != NULL){
          result->Set(
            NODE_PSYMBOL("country_code"),
            String::New(_mk_NA(gir->country_code)));
          result->Set(
            NODE_PSYMBOL("region_code"),
            String::New(_mk_NA(gir->region)));
          result->Set(
            NODE_PSYMBOL("regon_name"),
            String::New(_mk_NA(GeoIP_region_name_by_code(gir->country_code, gir->region))));
          result->Set(
            NODE_PSYMBOL("city"),
            String::New(_mk_NA(gir->city)));
          result->Set(
            NODE_PSYMBOL("postal_code"),
            String::New(_mk_NA(gir->postal_code)));
          result->Set(
            NODE_PSYMBOL("latitude"),
            Number::New(gir->latitude));
          result->Set(
            NODE_PSYMBOL("longitude"),
            Number::New(gir->longitude));
          result->Set(
            NODE_PSYMBOL("metro_code"),
            Number::New(gir->metro_code));
          result->Set(
            NODE_PSYMBOL("area_code"),
            Number::New(gir->area_code));
          GeoIPRecord_delete(gir);
        }
        return scope.Close(result);
      }
    private:
      static const char * _mk_NA( const char * p ){
       return p ? p : "N/A";
      }

  };

  class Geo : public EventEmitter {
    public:

    // This is the object binding. Things are bound onto the target.
    static void Initialize (Handle<Object> target) {
      HandleScope scope;
      DB::Attach(target);
    }
  };
};

// This is our bind in.
extern "C" void init (Handle<Object> target){
  HandleScope scope;
  maxmind::Geo::Initialize(target);
}
