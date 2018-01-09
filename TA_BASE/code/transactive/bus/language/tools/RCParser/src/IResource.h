#pragma once

FORWARD_DECLARE_4(IVisitor, MENUresource, POPUPresource, STRINGTABLEresource);


struct IResource
{
    virtual ~IResource() {}
    virtual const char* my_type() = 0;

    virtual void accept( IVisitor* visitor ) {};
    virtual void accept( MENUresource* parent, IVisitor* visitor ) {}
    virtual void accept( POPUPresource* parent, IVisitor* visitor ) {}
    virtual void accept( STRINGTABLEresource* parent, IVisitor* visitor ) {}
};

typedef boost::shared_ptr<IResource> IResourcePtr;
typedef std::vector<IResourcePtr> IResourcePtrList;
