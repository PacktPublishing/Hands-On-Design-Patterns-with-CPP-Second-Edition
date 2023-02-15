#include <iostream>

class City {
    // ...
    public:
    class DayEvents {
        friend City;
        bool flood          = false;
        bool fire           = false;
        bool revolt         = false;
        bool exotic_caravan = false;
        bool holy_vision    = false;
        bool festival       = false;

        public:
        DayEvents() = default;
        DayEvents& SetFlood() { flood = true; return *this; }
        DayEvents& SetFire() { fire = true; return *this; }
        DayEvents& SetRevolt() { revolt = true; return *this; }
        DayEvents& SetExoticCaravan() { exotic_caravan = true; return *this; }
        DayEvents& SetHolyVision() { holy_vision = true; return *this; }
        DayEvents& SetFestival() { festival = true; return *this; }
    };
    void day(const DayEvents& events) {
        std::cout << "Day's events:" << std::endl;
        if (events.flood         ) std::cout << "It's not all bad, the palace on the hill is now beachfront property." << std::endl;
        if (events.fire          ) std::cout << "Good news: no more garbage in the streets - all burned down." << std::endl;
        if (events.revolt        ) std::cout << "There's a Mr Torch and a Miss Pitchfork at the door, they want a word." << std::endl;
        if (events.exotic_caravan) std::cout << "They say some of the spices from the distant langs have strange effects on people." << std::endl;
        if (events.holy_vision   ) std::cout << "Bad news: farmers left their fields and are queueing up at the temple." << std::endl;
        if (events.festival      ) std::cout << "The wine and beer tax revenue is through the roof!" << std::endl;
    }
};

int main() {
    City capital;
    capital.day(City::DayEvents().SetFire());
    capital.day(City::DayEvents().SetFestival().SetExoticCaravan());
}
