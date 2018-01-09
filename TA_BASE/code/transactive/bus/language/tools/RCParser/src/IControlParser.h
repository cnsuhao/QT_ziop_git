#pragma once

FORWARD_DECLARE(IControl);


class IControlParser
{
public:
    virtual ~IControlParser() {}
    virtual IControlPtr parse() = 0;
};

typedef boost::shared_ptr<IControlParser> IControlParserPtr;
typedef std::vector<IControlParserPtr> IControlParserPtrList;
