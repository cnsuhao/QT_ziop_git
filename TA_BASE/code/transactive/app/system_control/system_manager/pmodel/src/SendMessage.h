#pragma once
#include "IMessage.h"
#include "ForeachCall.h"

namespace TA_Base_App
{
    template <typename F>
    static void sendMessage(F function)
    {
        foreachCall(IMessage::subscribers(), function);
    }

    template <typename F, typename P1>
    static void sendMessage(F function, const P1& p1)
    {
        foreachCall(IMessage::subscribers(), function, p1);
    }

    template <typename F, typename P1, typename P2>
    static void sendMessage(F function, const P1& p1, const P2& p2)
    {
        foreachCall(IMessage::subscribers(), function, p1, p2);
    }

    template <typename F, typename P1, typename P2, typename P3>
    static void sendMessage(F function, const P1& p1, const P2& p2, const P3& p3)
    {
        foreachCall(IMessage::subscribers(), function, p1, p2, p3);
    }

    template <typename F, typename P1, typename P2, typename P3, typename P4>
    static void sendMessage(F function, const P1& p1, const P2& p2, const P3& p3, const P4& p4)
    {
        foreachCall(IMessage::subscribers(), function, p1, p2, p3, p4);
    }

    template <typename F, typename P1, typename P2, typename P3, typename P4, typename P5>
    static void sendMessage(F function, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5)
    {
        foreachCall(IMessage::subscribers(), function, p1, p2, p3, p4, p5);
    }
}
