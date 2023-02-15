#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <cassert>

class AbstractBuilder;
class EmployeeBuilder;
class AddressBuilder;
class JobBuilder;
class AwardBuilder;

class Employee {
    // Name.
    std::string prefix_;
    std::string first_name_;
    std::string middle_name_;
    std::string last_name_;
    std::string suffix_;

    // Job.
    std::string title_;
    std::string office_;
    std::string manager_;
    std::string org_;
    std::string work_phone_;
    std::string email_;

    // Address.
    std::string house_;
    std::string street_;
    std::string apt_;
    std::string city_;
    std::string state_;
    std::string postal_code_;
    std::string country_;

    // Awards.
    std::vector<std::string> awards_;

    friend class AbstractBuilder;
    friend class EmployeeBuilder;
    friend class AddressBuilder;
    friend class JobBuilder;
    friend class AwardBuilder;

    Employee() = default;

    public:
    friend std::ostream& operator<<(std::ostream& out, const Employee& e) {
        auto print = [&](const auto& s) { out << s << " "; };
        auto print_or_skip = [&](const auto& s) { if (!s.empty()) out << s << " "; };

        print_or_skip(e.prefix_);
        print        (e.first_name_);
        print_or_skip(e.middle_name_);
        print        (e.last_name_);
        print_or_skip(e.suffix_);
        out << std::endl;

        out << "Work: ";
        print        (e.title_);
        print        (e.office_);
        print_or_skip(e.manager_);
        print        (e.org_);
        print        (e.work_phone_);
        print        (e.email_);
        out << std::endl;

        out << "Home: ";
        print        (e.house_);
        print        (e.street_);
        print_or_skip(e.apt_);
        print        (e.city_);
        print        (e.state_);
        print        (e.postal_code_);
        print        (e.country_);
        out << std::endl;

        if (!e.awards_.empty()) {
            out << "Awards: " << std::endl;
            for (const std::string& award : e.awards_) out << "  " << award << std::endl;
        }

        return out;
    }

};

class AbstractBuilder {
    protected:
    Employee& e_;
    public:
    explicit AbstractBuilder(Employee& e) : e_(e) {}
    operator Employee() { 
        assert(!e_.first_name_.empty() && !e_.last_name_.empty());
        return std::move(e_);
    }
    JobBuilder Job();
    AddressBuilder Address();
    AwardBuilder Awards();
};

class EmployeeBuilder : public AbstractBuilder {
    Employee employee_;
    public:
    EmployeeBuilder() : AbstractBuilder(employee_) {}
    EmployeeBuilder& SetPrefix    (std::string_view prefix     ) { e_.prefix_ = prefix; return *this; }
    EmployeeBuilder& SetFirstName (std::string_view first_name ) { e_.first_name_  = first_name ; return *this; }
    EmployeeBuilder& SetMiddleName(std::string_view middle_name) { e_.middle_name_ = middle_name; return *this; }
    EmployeeBuilder& SetLastName  (std::string_view last_name  ) { e_.last_name_   = last_name  ; return *this; }
    EmployeeBuilder& SetSuffix    (std::string_view suffix     ) { e_.suffix_      = suffix     ; return *this; }
};

class JobBuilder : public AbstractBuilder {
    public:
    explicit JobBuilder(Employee& e) : AbstractBuilder(e) {}
    JobBuilder& SetTitle     (std::string_view title      ) { e_.title_       = title      ; return *this; }
    JobBuilder& SetOffice    (std::string_view office     ) { e_.office_      = office     ; return *this; }
    JobBuilder& SetManager   (std::string_view manager    ) { e_.manager_     = manager    ; return *this; }
    JobBuilder& SetManager   (const Employee&  manager    ) { e_.manager_     = manager.first_name_ + " " + manager.last_name_; return *this; }
    JobBuilder& SetOrg       (std::string_view org        ) { e_.org_         = org        ; return *this; }
    JobBuilder& SetWorkPhone (std::string_view work_phone ) { e_.work_phone_  = work_phone ; return *this; }
    JobBuilder& SetEmail     (std::string_view email      ) { e_.email_       = email      ; return *this; }

    JobBuilder& CopyFrom     (const Employee& other) {
        e_.office_ = other.office_;
        e_.manager_ = other.manager_;
        e_.org_ = other.org_;
        return *this;
    }
};
JobBuilder AbstractBuilder::Job() { return JobBuilder(e_); }

class AddressBuilder : public AbstractBuilder {
    public:
    explicit AddressBuilder(Employee& e) : AbstractBuilder(e) {}
    AddressBuilder& SetHouse     (std::string_view house      ) { e_.house_       = house      ; return *this; }
    AddressBuilder& SetStreet    (std::string_view street     ) { e_.street_      = street     ; return *this; }
    AddressBuilder& SetApt       (std::string_view apt        ) { e_.apt_         = apt        ; return *this; }
    AddressBuilder& SetCity      (std::string_view city       ) { e_.city_        = city       ; return *this; }
    AddressBuilder& SetState     (std::string_view state      ) { e_.state_       = state      ; return *this; }
    AddressBuilder& SetPostalCode(std::string_view postal_code) { e_.postal_code_ = postal_code; return *this; }
    AddressBuilder& SetCountry   (std::string_view country    ) { e_.country_     = country    ; return *this; }
};
AddressBuilder AbstractBuilder::Address() { return AddressBuilder(e_); }

class AwardBuilder : public AbstractBuilder {
    public:
    explicit AwardBuilder(Employee& e) : AbstractBuilder(e) {}
    AwardBuilder& Add(std::string_view award) { e_.awards_.emplace_back(award); return *this; }
};
AwardBuilder AbstractBuilder::Awards() { return AwardBuilder(e_); }

int main() {
    Employee Barry = EmployeeBuilder()
        .SetFirstName("Barnabas")
        .SetLastName("Mackleberry")
        ;
    Employee Homer = EmployeeBuilder()
        .SetFirstName("Homer")
        .SetMiddleName("J")
        .SetLastName("Simpson")
        .Job()
            .SetTitle("Safety Inspector")
            .SetOffice("Sector 7G")
            .SetManager(Barry)
        .Address()
            .SetHouse("742")
            .SetStreet("Evergreen Terrace")
            .SetCity("Springfield")
        .Awards()
            .Add("Remorseless Eating Machine")
            .Add("Springfield Bowling Champion")
        ;
    Employee Lenny = EmployeeBuilder()
        .SetFirstName("Lenford")
        .SetLastName("Leonard")
        .Job()
            .CopyFrom(Homer)
        ;
    Employee Smithers = EmployeeBuilder()
        .SetFirstName("Waylon")
        .SetLastName("Smithers")
        .SetSuffix("Jr")
        .Job()
            .SetManager("Charles Montgomery Burns")
        ;

    std::cout << Homer << std::endl;
    std::cout << Barry << std::endl;
    std::cout << Smithers << std::endl;
    std::cout << Lenny << std::endl;
}
