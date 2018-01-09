#pragma once

FORWARD_DECLARE(IStatement);


class IStatementParser
{
public:
    virtual ~IStatementParser() {}
    virtual IStatementPtr parse() = 0;
};

typedef boost::shared_ptr<IStatementParser> IStatementParserPtr;
typedef std::vector<IStatementParserPtr> IStatementParserPtrList;
