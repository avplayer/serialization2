
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
    std::optional<std::string> opt;
    std::map<std::string, Person > mapTest;
    std::vector<std::vector<std::vector<int> > > vectorTest;

    bool operator==(const AddressBook & other) const
    {
        return (people == other.people)
                && (opt == other.opt)
                && (mapTest == other.mapTest)
                && (vectorTest == other.vectorTest);
    }

    SERIALIZATION_DEFINE(people, opt, mapTest, vectorTest)
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
            "opt string",
            {
                {"a", {"a", 1} },
                {"b", {"b", 2} },
                {"c", {"c", 3} },
            },
            {
                { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} },
                { {11, 12, 13}, {14, 15, 16} },
                { {21, 22, 23} },
            }
        };

        // json serialize
        serialization::json_oarchive json_oa;
        serialization::serialize(json_oa, ab);
        std::cout << "json serilize result: " << json_oa.data() << std::endl;

        // json unserialize
        AddressBook ab2;
        serialization::json_iarchive json_ia;
        json_ia.load_data(json_oa.data());
        serialization::unserialize(json_ia, ab2);
        std::cout << "json unserilize result: " << (ab == ab2 ? "success" : "fail") << std::endl;

        // xml serialize
        serialization::xml_oarchive xml_oa;
        serialization::serialize(xml_oa, ab);
        std::string xml_str = xml_oa.data();
        std::cout << "xml serilize result: " << xml_str << std::endl;

        // xml unserialize
        AddressBook ab3;
        serialization::xml_iarchive xml_ia;
        xml_ia.load_data(&xml_str[0]);
        serialization::unserialize(xml_ia, ab3);
        std::cout << "xml unserilize result: " << (ab == ab3 ? "success" : "fail") << std::endl;

    }
    catch (serialization::serialization_error & err)
    {
        std::cout << err.where() << ":" << err.what() << std::endl;
    }
}

