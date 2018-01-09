#pragma once

struct StriCmp
{
    bool operator()(const std::string& lhs, const std::string& rhs) const
    {
        return boost::ilexicographical_compare(lhs, rhs);
    }
};
