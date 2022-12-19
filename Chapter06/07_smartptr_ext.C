#include "07_smartptr.h"

int* get_raw_ptr() {
    return new int(0);
}

std::unique_ptr<int, deleter> get_unique_ptr() {
    return std::unique_ptr<int, deleter>(new int(0), d);
}

std::shared_ptr<int> get_shared_ptr() {
    return std::shared_ptr<int>(new int(0), d);
}

std::shared_ptr<int> make_shared_ptr() {
    return std::make_shared<int>(0);
}

smartptr<int, deleter> get_smartptr() {
    return smartptr<int, deleter>(new int(0), d);
}

smartptr_te<int> get_smartptr_te() {
    return smartptr_te<int>(new int(0), d);
}

smartptr_te_lb<int> get_smartptr_te_lb() {
    return smartptr_te_lb<int>(new int(0), d);
}

smartptr_te_lb0<int> get_smartptr_te_lb0() {
    return smartptr_te_lb0<int>(new int(0), d);
}

smartptr_te_static<int> get_smartptr_te_static() {
    return smartptr_te_static<int>(new int(0), d);
}

smartptr_te_vtable<int> get_smartptr_te_vtable() {
    return smartptr_te_vtable<int>(new int(0), d);
}
