#include <iostream>
#include <ostream>
#include <sstream>

class FileWriter {
    std::ostream& file_;
    public:
    explicit FileWriter(std::ostream& file) : file_(file) {}
    void Write(const char* data) {
        Preamble(data);
        file_ << data;
        Postscript(data);
    }
    virtual void Preamble(const char* data) {}
    virtual void Postscript(const char* data) {}
};

class LoggingFileWriter : public FileWriter {
    public:
    using FileWriter::FileWriter;
    void Preamble(const char* data) override {
        std::cout << "Writing \"" << data << "\" to the file" << std::endl;
    }
    void Postscript(const char* data) override {
        std::cout << "Writing done" << std::endl;
    }
};

int main() {
    std::stringstream S;
    LoggingFileWriter fw(S);
    fw.Write("It was a dark and stormy night; ");
    fw.Write("the rain fell in torrents");
    std::cout << S.str() << std::endl;
}
