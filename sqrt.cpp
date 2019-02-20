#include <stdexcept>

namespace nithin_michael {
    int sqrt(double x) {
        return x + x;
    }
    
    int jek_time() {
        throw std::runtime_error("not now jek!");        
    }
};

