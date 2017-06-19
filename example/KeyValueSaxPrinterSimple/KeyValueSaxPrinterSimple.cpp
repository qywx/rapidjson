// Reading a message JSON with Reader (SAX-style API).
// The JSON should be an object with key-string pairs.

#include "rapidjson/reader.h"
#include "rapidjson/pointer.h"
#include "rapidjson/error/en.h"
#include "rapidjson/PointedValueHandler.h"
#include "rapidjson/ToStdStream.h"
#include <iostream>


using namespace std;
using namespace rapidjson;


class KeyValuePrinter 
		//: public KeyValueSaxHandler<KeyValuePrinter>
{
public:
	//using Super = KeyValueSaxHandler<KeyValuePrinter>;
	//using typename Super::Ch;
	using Ch = char;
public:
	bool Default(Pointer const& p)                                               { (void)p; return true; }
	bool Null( Pointer const& p )                                                { cout << p<<":"<<"null"<<endl; return true; }
	bool Bool(Pointer const& p, bool b)                                          { cout << p<<":"<<b <<endl; return true;}
	bool Int (Pointer const& p, int i)                                           { cout << p<<":"<<i <<endl; return true;}
	bool Uint(Pointer const& p, unsigned i)                                      { cout << p<<":"<<i <<endl; return true;}
	bool Int64(Pointer const& p, int64_t i)                                      { cout << p<<":"<<i <<endl; return true;}
	bool Uint64(Pointer const& p, uint64_t i)                                    { cout << p<<":"<<i <<endl; return true;}
	bool Double(Pointer const& p, double d)                                      { cout << p<<":"<<d <<endl; return true;}
	bool RawNumber(Pointer const& p, const Ch* str, SizeType length, bool copy)  { (void)copy; cout << p<<":"<<string(str,length) <<endl; return true;}
	bool String(Pointer const& p, const Ch* str, SizeType length, bool copy)     { (void)copy; cout << p<<":"<<string(str,length) <<endl; return true;}
	bool StartObject(Pointer const& p)                                           { (void)p; cout << p<<":"<<"obj" <<endl; return true;}
	//bool Key(const Ch* str, SizeType length, bool copy)                        { (void)p; cout << p<<":"<<; return true;}
	bool EndObject(Pointer const& p, SizeType memberCount)                       { (void)memberCount; cout << p<<":"<<"endobj"<<endl; return true;}
	bool StartArray(Pointer const& p)                                            { cout << p<<":"<<"arr"<<endl; return true;}
	bool EndArray(Pointer const& p, SizeType elementCount)                       { (void)elementCount; cout << p<<":"<<"endarr"<<endl; return true;}
};



int main()
{
	//const char* json1 = "{ \"greeting\" : \"Hello!\", \"farewell\" : \"bye-bye!\" }";
	//const char* json = "{ \"greeting\" : \"Hello!\", \"farewell\" : \"bye-bye!\", \"foo\" : {} }";
	char json[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null,"
			"\"i\":123, \"pi\": 3.1416, \"a\":[-1, 2, 3, 4, \"array\", []], \"skipArrays\":[1, 2, [[[3]]]], "
			"\"skipObject\":{ \"i\":0, \"t\":true, \"n\":null, \"d\":123.45 }, "
			"\"skipNested\":[[[[{\"\":0}, {\"\":[-9.87]}]]], [], []], "
			"\"skipString\":\"zzz\", \"reachedEnd\":null, \"t\":true }";
	
	Reader reader;
	KeyValueSaxHandler<KeyValuePrinter> handler{};
	StringStream ss(json);
	bool success = reader.Parse(ss, handler);
	if( !success ){
		ParseErrorCode e = reader.GetParseErrorCode();
		size_t o = reader.GetErrorOffset();
		cout << "Error: " << GetParseError_En(e) << endl;;
		cout << " at offset " << o << " near '" << string(json).substr(o, 10) << "...'" << endl;
	}
	
}
