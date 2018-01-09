#include "StdAfx.h"
#include "MyFuture.h"

struct FutureTester
{
    void foo()
    {
    }

    static void bar(FutureTester&)
    {
    }
};


FutureTester* createByNew()
{
    return new FutureTester;
}

FutureTester createByValue()
{
    return FutureTester();
}

boost::shared_ptr<FutureTester> createByShared()
{
    return boost::shared_ptr<FutureTester>(new FutureTester);
}

void test_1()
{
    MyFuture<FutureTester> t;
    t.setCreator();
    t.setCreator(&createByNew);
    t.setSharedCreator(&createByShared);

    t->foo();
    FutureTester::bar(t);

    boost::shared_future< boost::shared_ptr<FutureTester> > f = t;
}
