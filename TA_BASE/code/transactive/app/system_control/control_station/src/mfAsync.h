#pragma once

template <typename MF, typename T>
void mfAsync(MF mf, T* t)
{
    boost::async(boost::bind(mf, t));
}

template <typename MF, typename T, typename P1>
void mfAsync(MF mf, T* t, const P1& p1)
{
    boost::async(boost::bind(mf, t, p1));
}
