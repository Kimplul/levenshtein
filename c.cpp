#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

template<class I>
auto prefix_length(
        I abegin, I aend,
        I bbegin, I bend
        ) {
    return std::distance(abegin, std::mismatch(abegin, aend, bbegin, bend).first);
}

static inline auto reduce(
        std::string_view &a,
        std::string_view &b
        ) -> void {
    auto prefix = prefix_length(a.begin(), a.end(), b.begin(), b.end());
    a.remove_prefix(prefix);
    b.remove_prefix(prefix);

    auto suffix = prefix_length(a.rbegin(), a.rend(), b.rbegin(), b.rend());
    a.remove_suffix(suffix);
    b.remove_suffix(suffix);
}

auto levenshtein_distance(
        std::string_view a,
        std::string_view b,
        size_t max_distance    
        ) -> size_t {

    reduce(a, b);

    if(a.empty() && b.empty()){
        return 0;
    }

    if(a.length() > b.length()){
        std::swap(a, b);
    }

    size_t buffer_length = b.length() + 1;
    std::vector<size_t> buffer_vector(buffer_length);

    auto buffer = buffer_vector.data();
    std::iota(buffer, buffer+buffer_length, 0);

    size_t offset = 0;
    size_t check_length = std::min(2*max_distance + 1, buffer_length);

    for(size_t i = 1; i < a.length() + 1; ++i){
        size_t minimum = std::numeric_limits<size_t>::max();
        size_t overflow = buffer_length - offset;
        
        size_t temp = buffer[offset] + 1;
        std::swap(buffer[offset], temp);
        
        for(size_t j = 1; j < std::min(overflow, check_length); ++j){

            size_t same = temp + (a[i - 1] == b[j - 1] ? 0 : 1);
            size_t min;

            if(j == check_length - 1){
                min = std::min(buffer[offset + j -1], max_distance) + 1;
            } else {
                min = std::min(buffer[offset + j], buffer[offset + j - 1]) + 1;
            }

            temp = std::min(min, same);

            if(temp < minimum){
                minimum = temp;
            }

            std::swap(buffer[offset + j], temp);
        }

        ++offset;

        if(minimum > max_distance){
            return std::numeric_limits<size_t>::max();
        }
        
    }

    return (buffer[buffer_length - 1] <= max_distance ? buffer[buffer_length - 1] : std::numeric_limits<size_t>::max());
}

int main(){
    std::string_view a = "wooff";
    std::string_view b = "wooff";
    size_t n = 2;
    std::cout << "Testing Levenshtein implementation with strings "
        << a << " " << b << " and n = " << n << std::endl;
    size_t l = levenshtein_distance(a, b, n);

    if(l != std::numeric_limits<size_t>::max()){
        std::cout << "Success! Edit distance: " << l << std::endl;
    } else {
        std::cout << "Failure!" << std::endl;
    }
}
