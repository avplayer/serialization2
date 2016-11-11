#pragma once

#include <cstdint>
#include <cstddef>
#include <sstream>

#define USE_LITTLE_ENDIAN

namespace serialization
{
  
  typedef uint32_t size_type;

#if(ENABLE_SERIALIZATION_TRACE)
  struct serialization_trace
  {
	  template<typename ...Args>
	  serialization_trace(Args&& ... args)
	  {
		  std::stringstream s;
		  s << ":";
		  build(s, std::forward<Args>(args)...);
		  str_ = s.str();
          std::cout << ">";
          print_level("   >");
          std::cout << str_ << std::endl;
		  ++level_;
	  }

	  template<typename T, typename ...Args>
	  void build(std::stringstream & s, T t, Args&& ... next)
	  {
		  s << t << ",";
		  build(s, next...);
	  }

	  void build(std::stringstream & s)
	  {
	  }

	  ~serialization_trace()
	  {
		  --level_;
          std::cout << "<";
          print_level("   <");
		  std::cout << str_ << std::endl;
	  }

      void print_level(const char * str)
      {
		  for(int i = 0; i < level_; ++i)
		  {
              std::cout << str;
		  }
      }

	  std::string str_;
	  static int level_;
  };
  int serialization_trace::level_ = 0;
#else
  struct serialization_trace
  {
	  template<typename ...Args>
	  serialization_trace(Args&& ... args)
	  {
	  }
  };
#endif

} // namespace serialization
