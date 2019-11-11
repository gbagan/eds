//
// Created by Guillaume on 21/08/2017.
//

#include <../include/Util.h>

// return the sublists of k elements of (0, ..., n-1)
std::vector<std::vector<int>> sublists(int n, int k) {
    std::vector<std::vector<int>> slists;
    slists.push_back(std::vector<int>());

    for (int i = 0; i < k; i++) {
        std::vector<std::vector<int>> slists2;

        int size = slists.size();
        for (auto slist : slists) {
            int size2 = slist.size();
            int begin = size2 == 0 ? 0 : slist[size2-1]+1;
            for (int ii = begin; ii <= n - k + i; ii++) {
                slists2.push_back(slist);
                slists2.back().push_back(ii);
            }
        }
        slists = std::move(slists2);

    }
    return slists;
}

std::vector<bool> intToBinary(int n, int size) {
    std::vector<bool> v (size);
    for (int i = 0; i < size; i++) {
        v[i] = n%2;
        n /= 2;
    }
    return v;
}