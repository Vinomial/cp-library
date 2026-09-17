#pragma once

#include <algorithm>
#include <vector>

namespace z_algorithm_lib
{
    // z[i] := s と s[i..] の最長共通接頭辞(LCP)の長さ（z[0] = s.size()）
    template <class Container>
    std::vector<int> calc_z(const Container &s)
    {
        const int n = static_cast<int>(s.size());
        std::vector<int> z(n);
        if (n == 0)
            return z;

        z[0] = n;
        int j = 0; // [j, j + z[j]) : 今わかっている中で最も右まで LCP が伸びている区間
        for (int i = 1; i < n; ++i)
        {
            int k = (i < j + z[j]) ? std::min(z[i - j], j + z[j] - i) : 0;
            while (i + k < n && s[k] == s[i + k])
                ++k;
            z[i] = k;
            if (i + k > j + z[j])
                j = i;
        }
        return z;
    }
} // namespace z_algorithm_lib
