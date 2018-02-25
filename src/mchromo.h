#ifndef __DESMAG_MCHROMO_H
#define __DESMAG_MCHROMO_H

#include <string>

namespace desmag {

    using namespace std;

    enum TNodeType
    {
	ENt_Unknown = 0,
	ENt_Node = 1,       // Adding node
	ENt_Move = 3, 	// Moving node
	ENt_Rm = 4,   	// Removal node
	ENt_Rename = 5, 	// Change node name
	ENt_Cont = 6, 	// Change node content
	ENt_Import = 8, 	// Importing node
    };

    enum TNodeAttr
    {
	ENa_Unknown = 0,
	ENa_Id = 1,
	ENa_Parent = 2,
	ENa_Ref = 3,
	ENa_MutNode = 10,
	ENa_MutAttr = 11,
	ENa_MutVal = 12,
	ENa_Inactive = 13, // Sign of mutations being inactive, optimized out for instance
	ENa_Targ = 14, // Target node of mutation, is used in OSM mode, ref ds_mut_osm
	ENa_Comp = 15, // Component to be changed, for comps related muts only, is used in OSM mode, ref ds_mut_osm
    };

    // Interface of chromo model
    class MChromoMdl
    {
	public:
	    virtual TNodeType GetType(const void* aHandle) = 0;
	    virtual TNodeType GetType(const string& aId) = 0;
	    virtual void* Root(const void* aHandle) = 0;
	    virtual void* Parent(const void* aHandle) = 0;
	    virtual void* Next(const void* aHandle, TNodeType aType = ENt_Unknown) = 0;
	    virtual void* NextText(const void* aHandle) = 0;
	    virtual void* Prev(const void* aHandle, TNodeType aType = ENt_Unknown) = 0;
	    virtual void* GetFirstChild(const void* aHandle, TNodeType aType = ENt_Unknown) = 0;
	    virtual void* GetLastChild(const void* aHandle, TNodeType aType = ENt_Unknown) = 0;
	    virtual void* GetFirstTextChild(const void* aHandle) = 0;
	    virtual char* GetAttr(const void* aHandle, TNodeAttr aAttr) const = 0;
	    virtual void  GetAttr(const void* aNode, TNodeAttr aType, int& aVal) const = 0;
	    virtual char* GetContent(const void* aHandle) = 0;
	    virtual void  SetContent(const void* aHandle, const string& aContent) = 0;
	    virtual bool AttrExists(const void* aHandle, TNodeAttr aAttr) const  = 0;
	    virtual void* AddChild(void* aParent, TNodeType aType) = 0;
	    virtual void* AddChild(void* aParent, const void* aHandle, bool aCopy = true, bool aRecursively = true) = 0;
	    virtual void* AddNext(const void* aPrev, const void* aHandle, bool aCopy = true) = 0;
	    virtual void* AddNext(const void* aPrev, TNodeType aNode) = 0;
	    virtual void* AddPrev(const void* aNext, const void* aHandle, bool aCopy = true) = 0;
	    virtual void RmChild(void* aParent, void* aChild, bool aDeattachOnly = false) = 0;
	    virtual void Rm(void* aHandle) = 0;
	    virtual void MoveNextTo(void* aHandle, void* aDest) = 0;
	    virtual void MovePrevTo(void* aHandle, void* aDest) = 0;
	    virtual void MoveToEnd(void* aHandle) = 0;
	    virtual void SetAttr(void* aNode, TNodeAttr aType, const char* aVal) = 0;
	    virtual void SetAttr(void* aNode, TNodeAttr aType, int aVal) = 0;
	    virtual void RmAttr(void* aNode, TNodeAttr aType) = 0;
	    //virtual void Dump(void* aNode, MLogRec* aLogRec) = 0;
	    virtual bool ToString(void* aNode, string& aString) const = 0;
	    virtual void Save(const string& aFileName) const = 0;
	    virtual int GetLineId(void* aHandle) const = 0;
    };

    class ChromoNode;

    // Interface of chromosome
    class MChromo
    {
	public:
	    virtual ~MChromo() {};
	    virtual ChromoNode& Root() = 0;
	    virtual const ChromoNode& Root() const= 0;
	    virtual void Set(const char *aFileName) = 0;
	    virtual bool Set(const string& aUri) = 0;
	    virtual bool SetFromSpec(const string& aSpec) = 0;
	    virtual bool GetSpec(string& aSpec) = 0;
	    virtual void Set(const ChromoNode& aRoot) = 0;
	    virtual void Init(TNodeType aRootType) = 0;
	    virtual void Reset() = 0;
	    virtual void Save(const string& aFileName) const = 0;
	    virtual ChromoNode CreateNode(void* aHandle) = 0;
    };

}

#endif
