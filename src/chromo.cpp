#include <stdlib.h>
#include <stdio.h>
#include <stdexcept> 
#include <assert.h>
#include <iostream>

#include "chromo.h"



using namespace desmag;



const string TMut::KCtrls = string(TMut::KSep, TMut::KAttrSep);

map<TNodeAttr, string> KNodeAttrsNames_Init()
{
    map<TNodeAttr, string> res;
    res[ENa_Id] = "id";
    res[ENa_Parent] = "parent";
    res[ENa_Ref] = "ref";
    res[ENa_MutNode] = "node";
    res[ENa_MutAttr] = "attr";
    res[ENa_MutVal] = "val";
    res[ENa_Inactive] = "na";
    res[ENa_Targ] = "targ";
    res[ENa_Comp] = "comp";
    return res;
}

map<TNodeType, string> KNodeTypesNames_Init()
{
    map<TNodeType, string> res;
    res[ENt_Node] = "node";
    res[ENt_Move] = "move";
    res[ENt_Rm] = "rm";
    res[ENt_Rename] = "rename";
    res[ENt_Cont] = "cont";
    res[ENt_Import] = "import";
    return res;
}

map<string, TNodeType> TMut::KNodeTypes_Init()
{
    map<string, TNodeType> res;
    for (map<TNodeType, string>::const_iterator it = KNodeTypesNames.begin(); it != KNodeTypesNames.end(); it++) {
	res[it->second] = it->first;
    }
    return res;
}

map<string, TNodeAttr> TMut::KNodeAttrs_Init()
{
    map<string, TNodeAttr> res;
    for (map<TNodeAttr, string>::const_iterator it = KNodeAttrsNames.begin(); it != KNodeAttrsNames.end(); it++) {
	res[it->second] = it->first;
    }
    return res;
}

map<TNodeType, string> TMut::KNodeTypesNames = KNodeTypesNames_Init();
map<TNodeAttr, string> TMut::KNodeAttrsNames = KNodeAttrsNames_Init();
map<string, TNodeType> TMut::KNodeTypes = TMut::KNodeTypes_Init();
map<string, TNodeAttr> TMut::KNodeAttrs = TMut::KNodeAttrs_Init();


const string& TMut::NodeAttrName(TNodeAttr aAttr)
{
    return KNodeAttrsNames[aAttr];
}

const string& TMut::NodeTypeName(TNodeType aType)
{
    return KNodeTypesNames[aType];
}

TNodeAttr TMut::NodeAttr(const string& aAttrName)
{
    return KNodeAttrs.count(aAttrName) > 0 ? KNodeAttrs[aAttrName] : ENa_Unknown;
}

TNodeType TMut::NodeType(const string& aTypeName)
{
    return KNodeTypes.count(aTypeName) > 0 ? KNodeTypes[aTypeName] : ENt_Unknown;
}

TMut::TMut(): mType(ENt_Unknown)
{
}

TMut::TMut(TNodeType aType): mType(aType)
{
}

TMut::TMut(TNodeType aType, TNodeAttr aAttr0, const string& aAttr0Val):
    mType(aType)
{
    mAttrs.insert(TElem(aAttr0, aAttr0Val));
}

TMut::TMut(TNodeType aType, TNodeAttr aAttr0, const string& aAttr0Val, TNodeAttr aAttr1, const string& aAttr1Val):
    mType(aType)
{
    mAttrs.insert(TElem(aAttr0, aAttr0Val));
    mAttrs.insert(TElem(aAttr1, aAttr1Val));
}

TMut::TMut(TNodeType aType, TNodeAttr aAttr0, const string& aAttr0Val, TNodeAttr aAttr1, const string& aAttr1Val,
	TNodeAttr aAttr2, const string& aAttr2Val): mType(aType)
{
    mAttrs.insert(TElem(aAttr0, aAttr0Val));
    mAttrs.insert(TElem(aAttr1, aAttr1Val));
    mAttrs.insert(TElem(aAttr2, aAttr2Val));
}

TMut::TMut(const ChromoNode& aCnode): mType(aCnode.Type())
{
    for (map<TNodeAttr, string>::const_iterator it = KNodeAttrsNames.begin(); it != KNodeAttrsNames.end(); it++) {
	TNodeAttr attr = it->first;
	if (aCnode.AttrExists(attr)) {
	    SetAttr(attr, aCnode.Attr(attr));
	}
    }
}

bool TMut::AttrExists(TNodeAttr aId) const
{
    return mAttrs.count(aId) > 0;
}

string TMut::Attr(TNodeAttr aId) const
{
    return mAttrs.count(aId) > 0 ? mAttrs.at(aId) : string();
}

void TMut::SetAttr(TNodeAttr aAttr, const string& aAttrVal)
{
    mAttrs[aAttr] = aAttrVal;
}

void TMut::RmAttr(TNodeAttr aAttr)
{
    mAttrs.erase(aAttr);
}

TMut::TMut(const string& aSpec)
{
    size_t type_beg = 0, type_end = 0;
    type_end = aSpec.find_first_of(KSep, type_beg); 
    string types = aSpec.substr(type_beg, (type_end == string::npos) ? string::npos : type_end - type_beg);
    mType = NodeType(types);
    if (mType == ENt_Unknown) {
	throw (runtime_error("Incorrect TMut type: " + types));
    }
    if (type_end != string::npos) {
	size_t attr_end = type_end;
	size_t attr_beg;
	do {
	    attr_beg = attr_end + 1;
	    size_t attr_mid = attr_beg;
	    // Find first non-escaped separator
	    do {
		attr_end = aSpec.find_first_of(KSep, attr_mid); 
		attr_mid = attr_end + 1;
	    } while (attr_end != string::npos && aSpec.at(attr_end - 1) == KEsc);
	    string attr = aSpec.substr(attr_beg, (attr_end == string::npos) ? string::npos : attr_end - attr_beg);
	    // Parse arg
		size_t attrtype_end = attr.find_first_of(KAttrSep); 
		if (attrtype_end != string::npos) {
		    string attrtype = attr.substr(0, attrtype_end);
		    TNodeAttr atype = NodeAttr(attrtype);
		    if (atype == ENa_Unknown) {
			throw (runtime_error("Incorrect TMut attr type: " + atype));
		    }
		    size_t attrval_beg = attrtype_end + 1;
		    string attrval = attr.substr(attrval_beg, string::npos);
		    SetAttr(atype, DeEscCtrls(attrval));
		} else {
		    throw (runtime_error("Incorrect TMut attr: " + attr));
		}
	} while (attr_end != string::npos);
    }
}

TMut::operator string() const
{
    string res(NodeTypeName(mType));
    for (TAttrs::const_iterator it = mAttrs.begin(); it != mAttrs.end(); it++) {
	res += KSep + NodeAttrName(it->first) + KAttrSep + EscapeCtrls(it->second);
    }
    return res;
}

/*
const TMut::TElem& TMut::AttrAt(TInt aInd) const
{
    __ASSERT(aInd < mAttrs.size());
    return mAttrs.at(aInd);
}
*/

string TMut::EscapeCtrls(const string& aInp)
{
    string res;
    for (string::const_iterator it = aInp.begin(); it != aInp.end(); it++) {
	const char cc = *it;
	if (cc == KSep || cc == KAttrSep || cc == KEsc) {
	    res.push_back(KEsc);
	}
	res.push_back(cc);
    }
    return res;
}

string TMut::DeEscCtrls(const string& aInp)
{
    string res;
    for (string::const_iterator it = aInp.begin(); it != aInp.end(); it++) {
	const char cc = *it;
	string::const_iterator itn = it + 1;
	if (itn != aInp.end()) {
	    const char cn = *itn;
	    if (cc == KEsc && IsCtrlSymb(cn)) {
		res.push_back(cn);
		it++;
		continue;
	    }
	}
	res.push_back(cc);
    }
    return res;
}

void TMut::Dump() const
{
    cout << "== TMut ==" << endl;
    for (TAttrs::const_iterator it = mAttrs.begin(); it != mAttrs.end(); it++) {
	cout << NodeAttrName(it->first) << " : " <<  EscapeCtrls(it->second) << endl;
    }
    /*
    for (auto attr: mAttrs) {
	cout << NodeAttrName(attr.first) << " : " << EscapeCtrls(attr.second) << endl;
    }
    */
}

// Chromo

Chromo::Chromo()
{
}

Chromo::Chromo(const Chromo& aSrc)
{
}

void Chromo::GetPath(const string& aUri, string& aPath)
{
    size_t scheme_end = aUri.find_first_of(':');
    size_t hier_beg = (scheme_end != string::npos) ? scheme_end+1 : 0; 
    size_t frag_beg = aUri.find_first_of("#");
    string hier = aUri.substr(hier_beg, frag_beg - hier_beg);
    size_t pos = hier.find("//"); 
    if (pos == 0) {
	// There is authority
    }
    else {
	aPath = hier;
    }
}

void Chromo::GetFrag(const string& aUri, string& aFrag)
{
    size_t frag_beg = aUri.find_first_of('#');
    if (frag_beg != string::npos) {
	aFrag = aUri.substr(frag_beg + 1);
    }
}


// Node of chromo spec

ChromoNode::ChromoNode(const ChromoNode& aNode): iMdl(aNode.iMdl), iHandle(aNode.iHandle)
{
}

ChromoNode& ChromoNode::operator=(const ChromoNode& aNode)
{
    // TODO [YB] Wrong to assign model because its base is not assignable (Base). To redesign.
//    iMdl = aNode.iMdl; iHandle = aNode.iHandle;
    iHandle = aNode.iHandle;
    return *this;
}

bool ChromoNode::operator==(const ChromoNode& aNode)
{
    return iHandle == aNode.iHandle;
}

bool ChromoNode::operator==(const ChromoNode& aNode) const
{ 
    return iHandle == aNode.iHandle;
}

const string ChromoNode::Attr(TNodeAttr aAttr) const
{
    char* sattr = iMdl.GetAttr(iHandle, aAttr); 
    string res; 
    if (sattr != NULL)
	res.assign(sattr);
    free(sattr); 
    return res; 
};

int ChromoNode::AttrInt(TNodeAttr aAttr) const 
{
    string attr = Attr(aAttr); 
    assert(!attr.empty()); 
    return atoi(attr.c_str());
};

bool ChromoNode::AttrBool(TNodeAttr aAttr) const 
{ 
    string attr = Attr(aAttr); 
    assert(!attr.empty() || attr == "yes" || attr == "no"); 
    return (attr == "yes"); 
};

string ChromoNode::GetName(const string& aTname)
{
    size_t tpos = aTname.find("%");
    return (tpos != string::npos) ? aTname.substr(tpos+1) : aTname;
}

ChromoNode::Iterator ChromoNode::Parent()
{
    void* parent = iMdl.Parent(iHandle);
    return  (parent == NULL) ?  End() : Iterator(ChromoNode(iMdl, parent));
}

ChromoNode::Iterator ChromoNode::Root()
{
    void* root = iMdl.Root(iHandle);
    return  (root == NULL) ?  End() : Iterator(ChromoNode(iMdl, root));
}

ChromoNode::Const_Iterator ChromoNode::Root() const 
{
    void* root = iMdl.Root(iHandle);
    return  (root == NULL) ?  End() : Const_Iterator(ChromoNode(iMdl, root));
}

ChromoNode::Const_Iterator ChromoNode::Parent() const
{
    void* parent = iMdl.Parent(iHandle);
    return  (parent == NULL) ?  End() : Const_Iterator(ChromoNode(iMdl, parent));
}

ChromoNode::Iterator ChromoNode::Find(TNodeType aType, const string& aName) 
{ 
    ChromoNode::Iterator res = End();
    size_t pos = aName.find("/");
    string name = aName.substr(0, pos);
    for (ChromoNode::Iterator it = Begin(); it != End(); it++) {
	if (((*it).Type() == aType) && (name.compare((*it).Name()) == 0)) {
	    res = it;  break;
	}
    }
    if ((res != End()) &&  (pos != string::npos)) {
	res = (*res).Find(aType, aName.substr(pos + 1));	
    }
    return res;
};

ChromoNode::Const_Iterator ChromoNode::Find(TNodeType aType, const string& aName) const
{ 
    ChromoNode::Const_Iterator res = End();
    for (ChromoNode::Const_Iterator it = Begin(); it != End(); it++) {
	if (((*it).Type() == aType) && (aName.compare((*it).Name()) == 0)) {
	    res = it;  break;
	}
    }
    return res;
};

ChromoNode::Iterator ChromoNode::Find(TNodeType aType, const string& aName, TNodeAttr aAttr, const string& aAttrVal)
{
    ChromoNode::Iterator res = End();
    for (ChromoNode::Iterator it = Begin(); it != End(); it++) {
	const ChromoNode& node = (*it);
	if ((node.Type() == aType)  && (aName.compare(node.Name()) == 0) && node.AttrExists(aAttr) && (aAttrVal.compare(node.Attr(aAttr)) == 0)) {
	    res = it;  break;
	}
    }
    return res;
};

ChromoNode ChromoNode::At(int aInd) const
{
    ChromoNode res = *End();
    Const_Iterator it = Begin();
    for (int ind = 0; it != End() && ind != aInd; it++, ind++);
    if (it != End()) {
	res = *it;
    }
    return res;
}

ChromoNode ChromoNode::AddChild(TNodeType aType) 
{ 
    return ChromoNode(iMdl, iMdl.AddChild(iHandle, aType)); 
}

ChromoNode ChromoNode::AddChild(const ChromoNode& aNode, bool aCopy, bool aRecursively) 
{ 
    return ChromoNode(iMdl, iMdl.AddChild(iHandle, aNode.Handle(), aCopy, aRecursively)); 
}

ChromoNode ChromoNode::AddChild(const TMut& aMut)
{
    ChromoNode res = AddChild(aMut.Type());
    for (auto attr: aMut.Attrs()) {
	res.SetAttr(attr.first, attr.second);
    }
    return res;
}

ChromoNode::Iterator ChromoNode::GetChildOwning(const ChromoNode& aNode) const
{
    ChromoNode res(aNode);
    ChromoNode prnt = *res.Parent();
    while (prnt != *this && prnt != ChromoNode()) {
	res = prnt;
    }
    return Iterator(res);
}

void ChromoNode::Activate()
{ 
    if (AttrExists(ENa_Inactive)) {
	RmAttr(ENa_Inactive);
    }
    if (Type() == ENt_Node) {
	Iterator it = Begin();
	while (it != End()) {
	    (*it).Activate();
	    it++;
	}
    }
};

ChromoNode::operator string() const
{
    string res;
    ToString(res);
    return res;
}
