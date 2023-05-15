#pragma ocne

#include <vector>


template<class Iter, class Cmp = std::less<typename std::iterator_traits<Iter>::value_type>>
size_t merge(Iter begin, Iter mid, Iter end, const Cmp& cmp = Cmp())
{
    auto lhs_len = std::distance(begin,mid);
    size_t count = 0;
    
    // reserve space for sort-bed
    std::vector<typename std::iterator_traits<Iter>::value_type> sorted;
    
    Iter lhs = begin, rhs = mid;
    while (lhs != mid && rhs != end)
    {
        if (cmp(*lhs,*rhs) || !cmp(*rhs,*lhs))
            sorted.emplace_back(*lhs++);
        else
        {   // bump count with rhs moves
            sorted.emplace_back(*rhs++);
            count += (lhs_len - std::distance(begin,lhs));
        }
    }
    
    // finish missing segment
    while (lhs != mid)
        sorted.emplace_back(*lhs++);
    while (rhs != end)
        sorted.emplace_back(*rhs++);

    // move sorted data back into place
    std::copy(sorted.begin(), sorted.end(), begin);
    
    return count;
}

// mergesort algorithm
template<class Iter, class Cmp = std::less<typename std::iterator_traits<Iter>::value_type>>
size_t mergesort(Iter begin, Iter end, const Cmp& cmp = Cmp())
{
    auto len = std::distance(begin, end);
    if (len < 2)
        return 0;
    
    Iter mid = std::next(begin, len/2);
    
    return mergesort(begin, mid, cmp) +
           mergesort(mid, end, cmp) +
           merge(begin, mid, end, cmp);
}