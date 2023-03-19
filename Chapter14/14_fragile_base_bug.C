#include <iostream>
#include <ostream>
#include <sstream>
#include <vector>
#include <string.h>

class FileWriter {
    std::ostream& file_;
    virtual void Preamble(const char* data) {}
    virtual void Postscript(const char* data) {}
    virtual void Preamble(std::vector<const char*> huge_data) {}
    virtual void Postscript(std::vector<const char*> huge_data) {}
    public:
    explicit FileWriter(std::ostream& file) : file_(file) {}
    void Write(const char* data) {
        Preamble(data);
        file_ << data;
        Postscript(data);
    }
    void Write(std::vector<const char*> huge_data) {
        Preamble(huge_data);
        for (auto data: huge_data) {
            Write(data);
        }
        Postscript(huge_data);
    }
    private:
};

class CountingFileWriter : public FileWriter {
    size_t count_ = 0;
    void Preamble(std::vector<const char*> huge_data) {
        for (auto data: huge_data) count_ += strlen(data);
    }
    void Preamble(const char* data) {
        count_ += strlen(data);
    }
    public:
    using FileWriter::FileWriter;
    size_t count() const { return count_; }
};

int main() {
    std::stringstream S;
    CountingFileWriter fw(S);
    std::vector<const char*> v { "It was a dark and stormy night; ", "the rain fell in torrents" };
    fw.Write(v);
    std::cout << fw.count() << ": " << S.str() << std::endl;
}
