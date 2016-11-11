
#include <iostream>
#include <fstream>

// print debug message
//#define ENABLE_SERIALIZATION_TRACE 1

#include <serialization/serialization.h>

struct Person
{
	std::string name;
	int32_t age;

	bool operator==(const Person & other) const
	{
		return (name == other.name) && (age == other.age);
	}

	SERIALIZATION_DEFINE(name, age)
};

struct AddressBook
{
	std::vector<Person> people;
	//std::vector<std::vector<std::string>> address;
	//std::map<std::string, int> test;
	std::optional<std::string> opt;
	//SERIALIZATION_DEFINE(people, address, test, opt)
	//
	
	bool operator==(const AddressBook & other) const
	{
		return (people == other.people) && (opt == other.opt);
	}

	SERIALIZATION_DEFINE(people, opt)
};

int main(int argc, char * argv[])
{
	try
	{
		AddressBook ab = {
			{
				{"aaaaaaa", 12},
				{"qwasasdq", 22331},
				{"12qweqweqweqwe", 123123}
			},
			"opt string"
		};

		// serialize
		serialization::json_oarchive oa;
		serialization::serialize(oa, ab);
		std::cout << oa.data() << std::endl;

		AddressBook ab2;
		// unserialize
		serialization::json_iarchive ia;
		ia.load_data(oa.data());
		serialization::unserialize(ia, ab2);

		if(ab == ab2)
		  std::cout << "ok" << std::endl;
		else
		  std::cout << "error" << std::endl; 
	}
	catch (serialization::serialization_error & err)
	{
		std::cout << err.where() << ":" << err.what() << std::endl;
	}
}

