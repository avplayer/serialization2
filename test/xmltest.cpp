#include "unittest.h"

#include <serialization/serialization.h>
#include <limits>

struct TestInt
{
	int value;
	bool operator==(const TestInt & other) const { return value == other.value; }
	SERIALIZATION_DEFINE(value);
};

struct TestUint
{
	uint value;
	bool operator==(const TestUint & other) const { return value == other.value; }
	SERIALIZATION_DEFINE(value);
};

struct TestInt32
{
	int32_t value;
	bool operator==(const TestInt32 & other) const { return value == other.value; }
	SERIALIZATION_DEFINE(value);
};

struct TestUint32
{
	uint32_t value;
	bool operator==(const TestUint32 & other) const { return value == other.value; }
	SERIALIZATION_DEFINE(value);
};

struct TestInt64
{
	int64_t value;
	bool operator==(const TestInt64 & other) const { return value == other.value; }
	SERIALIZATION_DEFINE(value);
};

struct TestUint64
{
	uint64_t value;
	bool operator==(const TestUint64 & other) const { return value == other.value; }
	SERIALIZATION_DEFINE(value);
};

struct TestDouble
{
	double value;
	SERIALIZATION_DEFINE(value);
};

struct TestString
{
	std::string value;
	bool operator==(const TestString & other) const { return value == other.value; }
	SERIALIZATION_DEFINE(value);
};

template<typename T>
void check_serialize(const T & v)
{
	T v2;
	try
	{
		// serialize
		serialization::xml_oarchive oa;
		serialization::serialize(oa, v);

		// unserialize
		std::string str = oa.data();
		serialization::xml_iarchive ia;
		ia.load_data(&str[0]);
		serialization::unserialize(ia, v2);
	}
	catch (serialization::serialization_error & err)
	{
		std::cout << "serialization error: " << err.where() << ":" << err.what() << std::endl;
	}

	EXPECT_EQ(v, v2);
}

template<typename T>
void test_integer()
{
	typedef decltype(T::value) int_type;
    int_type gap = std::numeric_limits<int_type>::max()/int_type{997};
	for(int_type i = std::numeric_limits<int_type>::min(); i < std::numeric_limits<int_type>::max() - gap; i += gap)
	{
		if(i > std::numeric_limits<int_type>::max() - gap)
		  i = std::numeric_limits<int_type>::max();
		
		T a{i};
		//std::cout << i << ",";
		check_serialize(a);
	}
	//std::cout << std::endl;
}

TEST(test_xml, test_int)
{
	test_integer<TestInt>();
}

TEST(test_xml, test_uint)
{
	test_integer<TestUint>();
}

TEST(test_xml, test_int32)
{
	test_integer<TestInt32>();
}

TEST(test_xml, test_uint32)
{
	test_integer<TestUint32>();
}

TEST(test_xml, test_int64)
{
	test_integer<TestInt64>();
}

TEST(test_xml, test_uint64)
{
	test_integer<TestUint64>();
}

TEST(test_xml, test_double)
{
	TestDouble a,b;
	// int can convert to double
	TestInt c;
	double values[10] = { 0, 2, 16, -4, 1.2, 4.3, 5.555, 6.3333, 1.3333333, -1212.122132111};
	for(int i = 0; i < 10; ++i)
	{
		try
		{
			// serialize
			serialization::xml_oarchive oa;
			if(i < 4)
			{
				c.value = values[i];
				serialization::serialize(oa, c);
			}
			else
			{
				a.value = values[i];
				serialization::serialize(oa, a);
			}
			// unserialize
			serialization::xml_iarchive ia;
			std::string str = oa.data();
			//std::cout << oa.data() << std::endl;
			ia.load_data(&str[0]);
			serialization::unserialize(ia, b);
		}
		catch (serialization::serialization_error & err)
		{
			std::cout << "serialization error: " << err.where() << ":" << err.what() << std::endl;
		}

		if(i < 4)
		{
			EXPECT_NEAR(c.value, b.value, 0.0);
		}
		else
		{
			EXPECT_NEAR(a.value, b.value, 0.0);
		}
	}
}

TEST(test_xml, test_string)
{
	TestString a,b;
	const char * values[] = { "", "asadasdas\"", R"(!@#$%&^*&*_++_**&^%$%#@#!~!{}:"?>><LL:<MNBVESRDXC)", "asdjhaiusdasdhiuahsiudhaoiuhsdiuoiuwqyoieqewryg18729384wjefbsadiufhasjdfnlk1o219uhhf10293489032j412j34ohew09f8qe-1i3ij4n"};
	for(auto i : values)
	{
		try
		{
			// serialize
			serialization::xml_oarchive oa;
			a.value = i;
			serialization::serialize(oa, a);

			// unserialize
			serialization::xml_iarchive ia;
			std::string str = oa.data();
			//std::cout << oa.data() << std::endl;
			ia.load_data(&str[0]);
			serialization::unserialize(ia, b);
		}
		catch (serialization::serialization_error & err)
		{
			std::cout << "serialization error: " << err.where() << ":" << err.what() << std::endl;
		}

		EXPECT_EQ(a.value, b.value);
	}
}


