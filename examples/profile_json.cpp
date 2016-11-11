
#include <iostream>
#include <fstream>
#include <rapidjson/document.h>

// print debug message
//#define ENABLE_SERIALIZATION_TRACE 1

#include <serialization/serialization.h>

struct Person
{
	std::string name;
	int32_t age;
	double money;

	SERIALIZATION_DEFINE(name, age, money)
};

struct AddressBook
{
	std::vector<Person> people;
	//std::vector<std::vector<std::string>> address;
	//std::map<std::string, int> test;
	std::optional<std::string> opt;
	//SERIALIZATION_DEFINE(people, address, test, opt)
	//

	SERIALIZATION_DEFINE(people, opt)
};


int main(int argc, char * argv[])
{
	if(argc < 3)
	  return 0;

	AddressBook ab = {
		{
			{"aaaaaaa", 12, 1123.123},
			{"qwasasdq", 22331, 1212.121},
			{"1234567890qwertyuiopasdfghjklzxcvbnm", 123123, 1212}
		},
		"opt string"
	};

	try
	{
		if(strcmp(argv[1], "serialize_sax") == 0)
		{
			size_t size = 0;
			for(int i = 0; i < atoi(argv[2]); ++i)
			{
				rapidjson::StringBuffer buffer;
				rapidjson::Writer<rapidjson::StringBuffer> writer{buffer};
				writer.StartObject();
				writer.Key("people");
				writer.StartArray();
				for(auto & i : ab.people)
				{
					writer.StartObject();
					writer.Key("name");
					writer.String(i.name.c_str(), i.name.length());
					writer.Key("age");
					writer.Int(i.age);
					writer.Key("money");
					writer.Double(i.money);
					writer.EndObject();
				}
				writer.EndArray();
				if(ab.opt)
				{
					writer.Key("opt");
					writer.String(ab.opt->c_str(), ab.opt->length());
				}
				writer.EndObject();
				size += buffer.GetSize();
				//std::cout << buffer.GetString() << std::endl;
			}
			std::cout << "serialize total size: " << size << std::endl;
		}
		else if(strcmp(argv[1], "serialize_dom") == 0)
		{
			size_t size = 0;
			for(int i = 0; i < atoi(argv[2]); ++i)
			{
				rapidjson::Document d;
				auto & a = d.GetAllocator();
				d.SetObject();
				rapidjson::Value p{rapidjson::kArrayType};
				for(auto & i : ab.people)
				{
					rapidjson::Value v{rapidjson::kObjectType};
					v.AddMember("name", rapidjson::Value().SetString(i.name.c_str(), i.name.size()), a);
					v.AddMember("age", rapidjson::Value().SetInt(i.age), a);
					v.AddMember("money", rapidjson::Value().SetDouble(i.money), a);
					p.PushBack(v, a);
				}
				d.AddMember("people", p, a);
				if(ab.opt)
				{
					d.AddMember("opt", rapidjson::Value().SetString(ab.opt->c_str(), ab.opt->size()), a);
				}
				rapidjson::StringBuffer buffer;
				rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
				d.Accept(writer);
				size += buffer.GetSize();
				//std::cout << buffer.GetString() << std::endl;
			}
			std::cout << "serialize total size: " << size << std::endl;
		}
		else if(strcmp(argv[1], "serialize") == 0)
		{
			size_t size = 0;
			for(int i = 0; i < atoi(argv[2]); ++i)
			{
				// serialize
				serialization::json_oarchive oa;
				serialization::serialize(oa, ab);
				//std::cout << oa.data() << std::endl;
				size += oa.size();
			}
			std::cout << "serialize total size: " << size << std::endl;
		}
		else
		{
			for(int i = 0; i < atoi(argv[2]); ++i)
			{
				// unserialize
				serialization::json_iarchive ia;
				ia.load_data(R"({"people":[{"name":"aaaaaaa","age":12,"money":1123.123},{"name":"qwasasdq","age":22331,"money":1212.121},{"name":"1234567890qwertyuiopasdfghjklzxcvbnm","age":123123,"money":1212.0}],"opt":"opt string"})");
				AddressBook ab2;
				serialization::unserialize(ia, ab2);
			}
		}
	}
	catch (serialization::serialization_error & err)
	{
		std::cout << err.where() << ":" << err.what() << std::endl;
	}
}

