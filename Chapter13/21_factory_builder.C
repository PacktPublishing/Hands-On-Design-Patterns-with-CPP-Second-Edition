#include <vector>
#include <iostream>
#include <sstream>
#include <memory>

class SerializerBase {
  static size_t type_count;
  using Factory = SerializerBase* (*)(std::istream& s);
  static std::vector<Factory> registry;

  protected:
  virtual void Serialize(std::ostream& s) const = 0;

  public:
  virtual ~SerializerBase() {}
  static size_t RegisterType(Factory factory) {
    registry.push_back(factory);
    return type_count++;
  }

  static auto Deserialize(size_t type, std::istream& s) {
    Factory factory = registry[type];
    return std::unique_ptr<SerializerBase>(factory(s));
  }
};
std::vector<SerializerBase::Factory> SerializerBase::registry;
size_t SerializerBase::type_count = 0;

class DeserializerFactory {
  std::istream& s_;

  public:
  explicit DeserializerFactory(std::istream& s) : s_(s) {}
  template <typename It>
  void Deserialize(It iter) {
    while (true) {
      size_t type;
      s_ >> type;
      if (s_.eof()) return;
      iter = SerializerBase::Deserialize(type, s_);
    }
  }
};

class Derived1 : public SerializerBase {
  int i_;
  public:
  Derived1(int i) : i_(i) { std::cout << "Derived1(" << i << ")" << std::endl; }
  void Serialize(std::ostream& s) const override { s << type_tag << " " << i_ << std::endl; }
  static const size_t type_tag;
};
const size_t Derived1::type_tag = RegisterType([](std::istream& s)->SerializerBase* { int i; s >> i; return new Derived1(i); });

class Derived2 : public SerializerBase {
  double x_, y_;
  public:
  Derived2(double x, double y) : x_(x), y_(y) { std::cout << "Derived2(" << x << ", " << y << ")" << std::endl; }
  void Serialize(std::ostream& s) const override { s << type_tag << " " << x_ << " " << y_ << std::endl; }
  static const size_t type_tag;
};
const size_t Derived2::type_tag = RegisterType([](std::istream& s)->SerializerBase* { double x, y; s >> x >> y; return new Derived2(x, y); });

class Derived3 : public SerializerBase {
  bool integer_;
  int i_ {};
  double x_ {};
  public:
  Derived3(int i) : integer_(true), i_(i) { std::cout << "Derived3(" << i << ") - integer" << std::endl; }
  Derived3(double x) : integer_(false), x_(x) { std::cout << "Derived3(" << x << ") - double" << std::endl; }
  void Serialize(std::ostream& s) const override { s << type_tag << " " << integer_ << " "; if (integer_) s << i_; else s << x_; s << std::endl; }
  static const size_t type_tag;
};
const size_t Derived3::type_tag = RegisterType([](std::istream& s)->SerializerBase* {
  bool integer; s >> integer;
  if (integer) {
    int i; s >> i; return new Derived3(i);
  } else {
    double x; s >> x; return new Derived3(x);
  }
});

int main() {
  std::stringstream S;

  // Serialization.
  Derived1(42).Serialize(S);
  Derived2(3.1415, 2.71828).Serialize(S);
  Derived3(0.5772156649).Serialize(S);
  Derived3(144).Serialize(S);

  std::cout << S.str() << std::endl;

  // Deserialization.
  std::vector<std::unique_ptr<SerializerBase>> v;
  DeserializerFactory F(S);
  F.Deserialize(std::back_inserter(v));
}

