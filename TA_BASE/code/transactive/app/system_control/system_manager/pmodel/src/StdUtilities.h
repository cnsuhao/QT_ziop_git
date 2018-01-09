#pragma once
#include <set>
#include <vector>
#include <algorithm>

namespace StdUtilities
{
    template<typename T>
    std::set<size_t> getIndexesOf(const std::vector<T>& all, const std::vector<T>& selection)
    {
        std::set<size_t> indexes;

        for (size_t i = 0; i < all.size(); i++)
        {
            if (std::find(selection.begin(), selection.end(), all[i]) != selection.end())
            {
                indexes.insert(i);
            }
        }

        return indexes;
    }

    template<typename T>
    int findIndex(const std::vector<T>& list, const T& value)
    {
        std::vector<T>::const_iterator it = std::find(list.begin(), list.end(), value);

        if (it != list.end())
        {
            return std::distance(list.begin(), it);
        }

        return -1;
    }
}
