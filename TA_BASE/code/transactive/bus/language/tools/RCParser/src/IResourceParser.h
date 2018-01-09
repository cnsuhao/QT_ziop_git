#pragma once

FORWARD_DECLARE(IResource);


struct IResourceParser
{
    virtual ~IResourceParser() {}
    virtual IResourcePtr parse() = 0;
};

typedef boost::shared_ptr<IResourceParser> IResourceParserPtr;
typedef std::vector<IResourceParserPtr> IResourceParserPtrList;
