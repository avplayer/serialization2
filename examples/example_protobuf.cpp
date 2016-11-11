
#include <iostream>
#include <string>
#include "addressbook.pb.h"
#include <google/protobuf/util/json_util.h>

using namespace std;

void test(const AddressBook & ab)
{
      std::string str = ab.SerializeAsString();
      AddressBook tmp;
      if(!tmp.ParseFromString(str))
      {
          std::cout << "parse from string error" << std::endl;
      }
}

int main(int argc, char* argv[]) 
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (argc != 2) 
    return -1;

  AddressBook ab;
  for(int i = 0; i < 10; ++i)
  {
	  Person* p = ab.add_people();
	  p->set_name("zbcdefghijklmnopqrstuvwxyz0123456789");
	  p->set_age(18);
	  p->set_money(123123.123123);
  }

  std::string str;
  google::protobuf::util::MessageToJsonString(ab, &str);
  std::cout << str << std::endl;
  for(int i = 0; i < atoi(argv[1]); ++i)
  {
	  google::protobuf::util::MessageToJsonString(ab, &str);
	  str.clear();
  }
	//str = ab.SerializeAsString();
	  //test(ab);

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
