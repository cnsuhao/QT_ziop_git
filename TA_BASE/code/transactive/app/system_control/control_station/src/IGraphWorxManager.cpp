#pragma once
#include "StdAfx.h"
#include "IGraphWorxManager.h"
#include "GraphWorxManager.h"

IGraphWorxManager& IGraphWorxManager::getInstance()
{
    return Singleton<GraphWorxManager>::getInstance();
}
