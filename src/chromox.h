#ifndef __DESMAG_CHROMOX_H
#define __DESMAG_CHROMOX_H

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "chromo.h"

namespace desmag {

// Model of XML based chromo
class ChromoMdlX: public MChromoMdl
{
    public:
	ChromoMdlX();
	virtual ~ChromoMdlX();
    public:
	virtual TNodeType GetType(const string& aId);
	virtual TNodeType GetType(const void* aHandle);
	virtual void* Root(const void* aHandle);
	virtual void* Parent(const void* aHandle);
	virtual void* Next(const void* aHandle, TNodeType aType = ENt_Unknown);
	virtual void* NextText(const void* aHandle);
	virtual void* Prev(const void* aHandle, TNodeType aType = ENt_Unknown);
	virtual void* GetFirstChild(const void* aHandle, TNodeType aType = ENt_Unknown);
	virtual void* GetLastChild(const void* aHandle, TNodeType aType = ENt_Unknown);
	virtual void* GetFirstTextChild(const void* aHandle);
	virtual char *GetAttr(const void* aHandle, TNodeAttr aAttr) const;
	virtual void  GetAttr(const void* aHandle, TNodeAttr aAttr, int& aVal) const;
	virtual char* GetContent(const void* aHandle);
	virtual void  SetContent(const void* aHandle, const string& aContent);
	virtual bool AttrExists(const void* aHandle, TNodeAttr aAttr) const;
	virtual void* AddChild(void* aParent, TNodeType aNode);
	virtual void* AddChild(void* aParent, const void* aHandle, bool aCopy = true, bool aRecursively = true);
	virtual void* AddNext(const void* aPrev, const void* aHandle, bool aCopy = true);
	virtual void* AddNext(const void* aPrev, TNodeType aNode);
	virtual void* AddPrev(const void* aNext, const void* aHandle, bool aCopy = true);
	virtual void RmChild(void* aParent, void* aChild, bool aDeattachOnly = false);
	virtual void Rm(void* aNode);
	virtual void MoveNextTo(void* aHandle, void* aDest);
	virtual void MovePrevTo(void* aHandle, void* aDest);
	virtual void MoveToEnd(void* aHandle);
	virtual void SetAttr(void* aNode, TNodeAttr aType, const char* aVal);
	virtual void SetAttr(void* aHandle, TNodeAttr aAttr, int aVal);
	virtual void RmAttr(void* aNode, TNodeAttr aType);
	//virtual void Dump(void* aNode, MLogRec* aLogRec);
	virtual bool ToString(void* aNode, string& aString) const;
	virtual void Save(const string& aFileName) const;
	virtual int GetLineId(void* aHandle) const;
    public:
	int GetAttrInt(void *aHandle, const char *aName);
	void* Set(const char* aFileName);
	void* Set(const string& aUri);
	void* SetFromSpec(const string& aSpec);
	void* Set(const void* aHandle);
	xmlDoc* Doc() { return iDoc;};
	static inline const char *Type() { return "ChromoMdlX";}; 
	virtual void* Init(TNodeType aRootType);
	void Reset();
    private:
	xmlDoc *iDoc;	// XML document
	bool iDocOwned;
};


// XML based chromo
class ChromoX: public Chromo
{
    public:
	ChromoX();
	ChromoX(const char *aFileName);
	virtual ~ChromoX();
    public:
	// From MChromo
	virtual ChromoNode& Root();
	virtual const ChromoNode& Root() const;
	virtual void Set(const char *aFileName);
	virtual bool Set(const string& aUri);
	virtual bool SetFromSpec(const string& aSpec);
	virtual bool GetSpec(string& aSpec);
	virtual void Set(const ChromoNode& aRoot);
	virtual void Init(TNodeType aRootType);
	virtual void Reset();
	virtual void Save(const string& aFileName) const;
	virtual ChromoNode CreateNode(void* aHandle);
	virtual MChromoMdl& GetModel() { return iMdl;};
    private:
	ChromoMdlX iMdl;
	ChromoNode iRootNode;
};

}

#endif
