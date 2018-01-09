#pragma once

FORWARD_DECLARE_3(IVisitor, DIALOGresource, DIALOGEXresource);


struct IControl
{
    virtual ~IControl() {}
    virtual const char* my_type() = 0;

    virtual void accept( DIALOGresource* parent, IVisitor* visitor ) {};
};

typedef boost::shared_ptr<IControl> IControlPtr;
typedef std::vector<IControlPtr> IControlPtrList;
