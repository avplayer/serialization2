## serialization2

一个神奇的序列化库。
消灭可恶的序列化代码。

## 示例

```cpp

struct Person
{
	std::string name;
	std::string number;

	SERIALIZATION_DEFINE(name, age)
};

struct AddressBook
{
	std::vector<Person> people;

	SERIALIZATION_DEFINE(people)
};

int main(int argc, char * argv[])
{
	AddressBook ab;
	
	// 序列化为json
	serialization::json_oarchive oa;
	serialization::serialize(oa, ab);
	
	// 打印json数据
	std::cout << oa.data() << std::endl;

	// 将json反序列化
	AddressBook ab2;
	serialization::json_iarchive ia;
	ia.load_data(oa.data());
	serialization::unserialize(ia, ab2);
}

```

详细使用方法参考examples文件夹

