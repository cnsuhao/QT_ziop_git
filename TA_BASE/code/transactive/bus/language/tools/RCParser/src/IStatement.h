#pragma once

FORWARD_DECLARE_6(IVisitor, DIALOGresource, DIALOGEXresource, MENUresource, POPUPresource, TOOLBARresource);


struct IStatement
{
    virtual ~IStatement() {}
    virtual const char* my_type() = 0;

    virtual void accept( DIALOGresource* parent, IVisitor* visitor ) {};
    virtual void accept( MENUresource* parent, IVisitor* visitor ) {};
    virtual void accept( POPUPresource* parent, IVisitor* visitor ) {};
    virtual void accept( TOOLBARresource* parent, IVisitor* visitor ) {};
};

typedef boost::shared_ptr<IStatement> IStatementPtr;
typedef std::vector<IStatementPtr> IStatementPtrList;
