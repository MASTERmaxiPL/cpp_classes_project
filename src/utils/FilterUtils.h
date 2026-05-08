#ifndef ROSTER_UTILS_H
#define ROSTER_UTILS_H

#include <vector>

namespace  utils
{
    template <typename T, typename Predicate>
    static std::vector<T*> filter(const std::vector<T*>& roster, Predicate pred) {
        std::vector<T*> results;
        for (T* member : roster) {
            if (member && pred(member)) {
                results.push_back(member);
            }
        }
        return results;
    }
}

#endif