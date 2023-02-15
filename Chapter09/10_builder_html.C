#include <string>
#include <string_view>
#include <vector>
#include <iostream>
#include <initializer_list>

class HTMLElement {
    public:
    const std::string name_;
    const std::string text_;
    const std::vector<HTMLElement> children_;
    HTMLElement(std::string_view name, std::string_view text) : name_(name), text_(text) {}
    HTMLElement(std::string_view name, std::string_view text, std::vector<HTMLElement>&& children) : name_(name), text_(text), children_(std::move(children)) {}
    using IndentedHTMLElement = std::pair<const HTMLElement&, size_t>;
    friend std::ostream& operator<<(std::ostream& out, IndentedHTMLElement indented_element) {
        const auto& [ element, indent_value ] = indented_element;
        const std::string indent(indent_value, ' ');
        out << indent << "<" << element.name_ << ">\n";
        if (!element.text_.empty()) out << indent << "  " << element.text_ << "\n";
        for (const auto& e : element.children_) {
            out << IndentedHTMLElement(e, indent_value + 2);
        }
        out << indent << "</" << element.name_ << ">" << std::endl;
        return out;
    }
    friend std::ostream& operator<<(std::ostream& out, const HTMLElement& element) {
        out << IndentedHTMLElement(element, 0);
        return out;
    }
};

class HTML : public HTMLElement {
    public:
    HTML() : HTMLElement("html", "") {}
    HTML(std::initializer_list<HTMLElement> children) : HTMLElement("html", "", children) {};
};

class Head : public HTMLElement {
    public:
    Head() : HTMLElement("head", "") {}
    Head(std::initializer_list<HTMLElement> children) : HTMLElement("head", "", children) {};
};

class Body : public HTMLElement {
    public:
    Body() : HTMLElement("body", "") {}
    Body(std::initializer_list<HTMLElement> children) : HTMLElement("body", "", children) {};
};

class Title : public HTMLElement {
    public:
    Title(std::string_view title) : HTMLElement("title", title) {}
};

class OL : public HTMLElement {
    public:
    OL() : HTMLElement("ol", "") {}
    OL(std::initializer_list<HTMLElement> children) : HTMLElement("ol", "", children) {};
};

class UL : public HTMLElement {
    public:
    UL() : HTMLElement("ul", "") {}
    UL(std::initializer_list<HTMLElement> children) : HTMLElement("ul", "", children) {};
};

class P : public HTMLElement {
    public:
    P(std::string_view text) : HTMLElement("p", text) {}
};

class LI : public HTMLElement {
    public:
    explicit LI(std::string_view text) : HTMLElement("li", text) {}
    LI(std::string_view text, std::initializer_list<HTMLElement> children) : HTMLElement("li", text, children) {}
};

int main() {
    auto doc = HTML{
        Head{
            Title{"Mary Had a Little Lamb"}
        },
        Body{
            P{"Mary Had a Little Lamb"}, 
            OL{
                LI{"Its fleece was white as snow"},
                LI{"And everywhere that Mary went"},
                LI{"The lamb was sure to go"}
            },
            UL{
                LI{"It followed her to school one day"},
                LI{"Which was against the rules",
                    {UL{
                        LI{"It made the children laugh and play"},
                        LI{"To see a lamb at school"}
                    }}
                },
                LI{"And so the teacher turned it out"}
            }
        }
    };
    std::cout << doc << std::endl;
}
