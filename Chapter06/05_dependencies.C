#include <functional>
#include <iostream>
#include <string.h>

class Network {
  static const char* default_processor(const char* data) {
    std::cout << "Default processing" << std::endl;
    return data;
  }
  std::function<const char*(const char*)> processor =
    default_processor;
  public:
  void send(const char* data) {
    std::cout << "Send: " << processor(data) << std::endl;
  }
  public:
  template <typename F>
  void set_processor(F&& f) { processor = f; }
};

int main() {
  Network N;
  N.send("abc");
  N.set_processor([](const char* c) {
      std::cout << "Special processing" << std::endl;
      const size_t l = strlen(c);
      char* s = new char[l + 3];
      s[0] = '"';
      memcpy(s + 1, c, l);
      s[l + 1] = '"';
      s[l + 2] = '\0';
      return c;
    });
  N.send("abc");
}
