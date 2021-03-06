

#include <assert.h>
#include <sstream>

//#include "mlog.h"
#include "chromox.h"

using namespace desmag;

const char* KChromoSystemId = "/usr/share/grayb/conf/objspec.dtd";

ChromoMdlX::ChromoMdlX(): MChromoMdl(), iDoc(NULL), iDocOwned(false)
{
};

ChromoMdlX::~ChromoMdlX()
{
    Reset(); 
}

void* ChromoMdlX::Init(TNodeType aRootType)
{
    iDocOwned = true;
    iDoc = xmlNewDoc((const xmlChar*) "1.0");
    string sroottype = TMut::NodeTypeName(aRootType);
    xmlNodePtr root = xmlNewNode(NULL, (const xmlChar *) sroottype.c_str());
    xmlDocSetRootElement(iDoc, root);
    //xmlDtdPtr dtd = xmlParseDTD(NULL, (const xmlChar*) KChromoSystemId );
    //iDoc->extSubset = dtd;
    return root;
}

void ChromoMdlX::Reset()
{
    if (iDocOwned) {
	xmlFreeDoc(iDoc);
    }
    iDoc = NULL;
}

void* ChromoMdlX::Set(const char *aFileName)
{
    xmlNode *sEnv = NULL; // Node of environment element
    xmlNode *sRoot = NULL; // Node of root element
    // Read and parse the CAE spec file
    Reset();
    iDoc = xmlReadFile(aFileName, NULL, XML_PARSE_DTDLOAD | XML_PARSE_DTDVALID);
    assert(iDoc != NULL);
    // Get the node 
    sRoot = (xmlNodePtr) GetFirstChild((void *) iDoc, ENt_Node);
    iDocOwned = false;
    return sRoot;
}

void* ChromoMdlX::SetFromSpec(const string& aSpec)
{
    xmlNode *sEnv = NULL; // Node of environment element
    xmlNode *sRoot = NULL; // Node of root element
    // Read and parse the CAE spec file
    Reset();
    iDoc = xmlReadMemory(aSpec.c_str(), aSpec.size(), "noname.xml", NULL, XML_PARSE_DTDLOAD | XML_PARSE_DTDVALID);
    assert(iDoc != NULL);
    // Get any node, not necessary ENt_Node, ref ds_mut_osmlc_impl
    sRoot = (xmlNodePtr) GetFirstChild((void *) iDoc);
    iDocOwned = false;
    return sRoot;
}

void* ChromoMdlX::Set(const string& aUri)
{
    xmlNodePtr res = NULL;
    xmlNode *sEnv = NULL; // Node of environment element
    xmlNode *sRoot = NULL; // Node of root element
    // Read and parse the CAE spec file
    Reset();
    string path;
    Chromo::GetPath(aUri, path);
    iDoc = xmlReadFile(path.c_str(), NULL, XML_PARSE_DTDLOAD | XML_PARSE_DTDVALID);
    assert(iDoc != NULL);
    // Get the node 
    string desuri;
    Chromo::GetFrag(aUri, desuri);
    sRoot = (xmlNodePtr) GetFirstChild((void *) iDoc, ENt_Node);
    res = sRoot;
    iDocOwned = false;
    return res;
}

void* ChromoMdlX::Set(const void* aHandle)
{
    if (iDoc == NULL) {
	iDocOwned = true;
	iDoc = xmlNewDoc((const xmlChar*) "1.0");
	xmlDtdPtr dtd = xmlParseDTD(NULL, (const xmlChar*) KChromoSystemId );
	iDoc->extSubset = dtd;
    }
    xmlNodePtr node = xmlDocCopyNode((xmlNodePtr) aHandle, iDoc, 1);
    xmlDocSetRootElement(iDoc, node);
    return node;
}

TNodeType ChromoMdlX::GetType(const string& aId)
{
    return TMut::NodeType(aId);
}

TNodeType ChromoMdlX::GetType(const void* aHandle)
{
    TNodeType res = ENt_Unknown;
    xmlNodePtr node = (xmlNodePtr) aHandle;
    if (node->name != NULL) {
	const char* type_name = (const char*) node->name;
	res = TMut::NodeType(type_name);
    }
    return res;
}

void *ChromoMdlX::GetFirstChild(const void *aHandle, TNodeType aType)
{
    xmlNodePtr node = (xmlNodePtr) aHandle;
    assert(node != NULL);
    xmlNodePtr res = node->children;
    if (res != NULL) {
	TNodeType type = GetType((void*) res);
	if ((res->type != XML_ELEMENT_NODE) || ((aType != ENt_Unknown) ? (type != aType) : (type == ENt_Unknown)))
	    res = (xmlNodePtr) Next(res, aType);
    }
    return res;
}

void *ChromoMdlX::GetLastChild(const void *aHandle, TNodeType aType)
{
    xmlNodePtr node = (xmlNodePtr) aHandle;
    assert(node != NULL);
    xmlNodePtr res = node->last;
    if (res != NULL) {
	TNodeType type = GetType((void*) res);
	if ((res->type != XML_ELEMENT_NODE) || ((aType != ENt_Unknown) ? (type != aType) : (type == ENt_Unknown)))
	    res = (xmlNodePtr) Prev(res, aType);
    }
    return res;
}

void* ChromoMdlX::GetFirstTextChild(const void* aHandle)
{
    xmlNodePtr node = (xmlNodePtr) aHandle;
    assert(node != NULL);
    xmlNodePtr res = node->children;
    if (res != NULL) {
	if (res->type != XML_TEXT_NODE)
	    res = (xmlNodePtr) NextText(res);
    }
    return res;
}

void* ChromoMdlX::Root(const void* aHandle)
{
    void *res = NULL;
    xmlNodePtr parent = (xmlNodePtr) Parent(aHandle);
    if (parent == NULL) {
	res = (void*) aHandle;
    }
    else {
	res = Root(parent);
    }
    return res;
}

void* ChromoMdlX::Parent(const void* aHandle)
{
    xmlNodePtr parent = ((xmlNodePtr) aHandle)->parent;
    return (parent != NULL && parent->type == XML_ELEMENT_NODE) ? parent : NULL;
}

void *ChromoMdlX::Next(const void *aHandle, TNodeType aType)
{
    assert(aHandle!= NULL);
    xmlNodePtr res = ((xmlNodePtr) aHandle)->next;
    if (res != NULL) {
	TNodeType type = GetType((void*) res);
	while ((res != NULL) && ((res->type != XML_ELEMENT_NODE) || ((aType != ENt_Unknown) ? (type != aType) : (type == ENt_Unknown)))) {
	    res = res->next;
	    if (res != NULL)
		type = GetType((void*) res);
	}
    }
    return res;
}

void* ChromoMdlX::NextText(const void* aHandle)
{
    assert(aHandle!= NULL);
    xmlNodePtr res = ((xmlNodePtr) aHandle)->next;
    if (res != NULL) {
	while ((res != NULL) && (res->type != XML_TEXT_NODE)) {
	    res = res->next;
	}
    }
    return res;
}

void *ChromoMdlX::Prev(const void *aHandle, TNodeType aType)
{
    assert(aHandle!= NULL);
    xmlNodePtr res = ((xmlNodePtr) aHandle)->prev;
    if (res != NULL) {
	TNodeType type = GetType((void*) res);
	while ((res != NULL) && ((res->type != XML_ELEMENT_NODE) || ((aType != ENt_Unknown) ? (type != aType) : (type == ENt_Unknown)))) {
	    res = res->prev;
	    type = GetType((void*) res);
	}
    }
    return res;
}

char *ChromoMdlX::GetAttr(const void* aHandle, TNodeAttr aAttr) const
{
    assert(aHandle != NULL);
    xmlNodePtr node = (xmlNodePtr) aHandle;
    xmlChar *attr = xmlGetProp(node, (const xmlChar *) TMut::NodeAttrName(aAttr).c_str());
    return (char *) attr;
}

void ChromoMdlX::GetAttr(const void* aHandle, TNodeAttr aAttr, int& aVal) const
{
    string ss(GetAttr(aHandle, aAttr)); 
    istringstream sstr(ss);
    sstr >> aVal;
}

void ChromoMdlX::SetAttr(void* aHandle, TNodeAttr aAttr, int aVal)
{
    stringstream sval;
    sval << aVal;
    SetAttr(aHandle, aAttr, sval.str().c_str());
}

bool ChromoMdlX::AttrExists(const void* aHandle, TNodeAttr aAttr) const
{
    assert(aHandle != NULL);
    bool res = false;
    xmlNodePtr node = (xmlNodePtr) aHandle;
    xmlChar *attr = xmlGetProp(node, (const xmlChar *) TMut::NodeAttrName(aAttr).c_str());
    res = (attr != NULL);
    free (attr);
    return res;
}

char* ChromoMdlX::GetContent(const void* aHandle)
{
    assert(aHandle != NULL);
    xmlNodePtr node = (xmlNodePtr) aHandle;
    xmlChar *cont = xmlNodeGetContent(node);
    return (char *) cont;
}

void  ChromoMdlX::SetContent(const void* aHandle, const string& aContent)
{
    assert(aHandle != NULL);
    xmlNodePtr node = (xmlNodePtr) aHandle;
    xmlNodeSetContent(node, (const xmlChar*) aContent.c_str());
}

int ChromoMdlX::GetAttrInt(void *aHandle, const char *aName)
{
    int res = -1;
    assert(aHandle!= NULL);
    xmlNodePtr node = (xmlNodePtr) aHandle;
    xmlChar *attr = xmlGetProp(node, (const xmlChar *) aName);
    assert(attr != NULL);
    res = atoi((const char *) attr);
    free(attr);
    return res;
}

void* ChromoMdlX::AddChild(void* aParent, TNodeType aNode)
{
    string name = TMut::NodeTypeName(aNode);
    xmlNodePtr node = xmlNewNode(NULL, (const xmlChar*) name.c_str());
    return xmlAddChild((xmlNodePtr) aParent, node);
}

void* ChromoMdlX::AddChild(void* aParent, const void* aHandle, bool aCopy, bool aRecursively)
{
    void* root = Root(aParent);
    /*
    int rorder = GetOrder(root, true);
    int order = GetOrder((void*) aHandle);
    if (order == 0) {
	order = rorder + 1;
	SetOrder((void*) aHandle, order);
    }
    if (order > rorder) {
	SetOrder(root, order, true);
    }
    */
    xmlNodePtr node = aCopy ? xmlCopyNode((xmlNodePtr) aHandle, aRecursively ? 1:2) : (xmlNodePtr) aHandle;
    return xmlAddChild((xmlNodePtr) aParent, node);
}

void* ChromoMdlX::AddNext(const void* aPrev, const void* aHandle, bool aCopy)
{
    xmlNodePtr node = aCopy ? xmlCopyNode((xmlNodePtr) aHandle, 1) : (xmlNodePtr) aHandle;
    return xmlAddNextSibling((xmlNodePtr) aPrev, node);
}

void* ChromoMdlX::AddNext(const void* aPrev, TNodeType aNode)
{
    string name = TMut::NodeTypeName(aNode);
    xmlNodePtr node = xmlNewNode(NULL, (const xmlChar*) name.c_str());
    return xmlAddNextSibling((xmlNodePtr) aPrev, node);
}

void* ChromoMdlX::AddPrev(const void* aNext, const void* aHandle, bool aCopy)
{
    xmlNodePtr node = aCopy ? xmlCopyNode((xmlNodePtr) aHandle, 1) : (xmlNodePtr) aHandle;
    return xmlAddPrevSibling((xmlNodePtr) aNext, node);
}

void ChromoMdlX::SetAttr(void* aNode, TNodeAttr aType, const char* aVal)
{
    string name = TMut::NodeAttrName(aType);
    if (AttrExists(aNode, aType)) {
	xmlSetProp((xmlNodePtr) aNode, (const xmlChar*) name.c_str(), (const xmlChar*) aVal);
    }
    else {
	xmlNewProp((xmlNodePtr) aNode, (const xmlChar*) name.c_str(), (const xmlChar*) aVal);
    }
}

void ChromoMdlX::RmAttr(void* aNode, TNodeAttr aType)
{
    xmlAttrPtr attr = xmlHasProp((xmlNodePtr) aNode, (const xmlChar *) TMut::NodeAttrName(aType).c_str());
    if (attr != NULL) {
	xmlRemoveProp(attr);
    }
}

void ChromoMdlX::RmChild(void* aParent, void* aChild, bool aDeattachOnly)
{
    xmlNodePtr child = (xmlNodePtr) aChild;
    xmlUnlinkNode(child);
    if (!aDeattachOnly) {
	xmlFreeNode(child);
    }
}

void ChromoMdlX::Rm(void* aNode)
{
    xmlNodePtr node = (xmlNodePtr) aNode;
    xmlUnlinkNode(node);
    xmlFreeNode(node);
}

/*
void ChromoMdlX::Dump(void* aNode, MLogRec* aLogRec)
{
    xmlBufferPtr bufp = xmlBufferCreate();	
    int	res = xmlNodeDump(bufp, iDoc, (xmlNodePtr) aNode, 0, 0);
    aLogRec->WriteFormat("%s", xmlBufferContent(bufp));
}
*/

bool ChromoMdlX::ToString(void* aNode, string& aString) const
{
    xmlBufferPtr bufp = xmlBufferCreate();	
    int	res = xmlNodeDump(bufp, iDoc, (xmlNodePtr) aNode, 0, 0);
    aString.assign((const char*)(xmlBufferContent(bufp)));
    return (res != 0);
}

void ChromoMdlX::Save(const string& aFileName) const
{
    int res = xmlSaveFormatFile(aFileName.c_str(), iDoc, 4);
}

void ChromoMdlX::MoveNextTo(void* aHandle, void* aDest)
{
    xmlNodePtr src = (xmlNodePtr) aHandle;
    xmlNodePtr dest = (xmlNodePtr) aDest;
    xmlAddNextSibling(dest, src);
}

void ChromoMdlX::MovePrevTo(void* aHandle, void* aDest)
{
    xmlNodePtr src = (xmlNodePtr) aHandle;
    xmlNodePtr dest = (xmlNodePtr) aDest;
    xmlAddPrevSibling(dest, src);
}

void ChromoMdlX::MoveToEnd(void* aHandle)
{
    xmlNodePtr src = (xmlNodePtr) aHandle;
    xmlNodePtr dest = ((xmlNodePtr) aHandle)->next;
    if (dest != NULL) {
	xmlAddSibling(dest, src);
    }
}

int ChromoMdlX::GetLineId(void* aHandle) const
{
    xmlNodePtr src = (xmlNodePtr) aHandle;
    return src->line;
}


ChromoX::ChromoX(): iRootNode(iMdl, NULL)
{
}

void ChromoX::Set(const char *aFileName)
{
    void *root = iMdl.Set(aFileName);
    iRootNode = ChromoNode(iMdl, root);
}

bool ChromoX::Set(const string& aUri)
{
    bool res = false;
    void *root = iMdl.Set(aUri);
    if (root != NULL) {
	iRootNode = ChromoNode(iMdl, root);
	res = true;
    }
    return res;
}

bool ChromoX::SetFromSpec(const string& aSpec)
{
    bool res = false;
    void *root = iMdl.SetFromSpec(aSpec);
    if (root != NULL) {
	iRootNode = ChromoNode(iMdl, root);
	res = true;
    }
    return res;

}

void ChromoX::Set(const ChromoNode& aRoot)
{
    void *root = iMdl.Set(aRoot.Handle());
    iRootNode = ChromoNode(iMdl, root);
}

ChromoX::~ChromoX()
{
}

ChromoNode& ChromoX::Root()
{
    return iRootNode;
}

const ChromoNode& ChromoX::Root() const
{
    return iRootNode;
}

void ChromoX::Reset()
{
    iMdl.Reset();
}

void ChromoX::Init(TNodeType aRootType)
{
    void *root = iMdl.Init(aRootType);
    iRootNode = ChromoNode(iMdl, root);
}

void ChromoX::Save(const string& aFileName) const
{
    // We cannot simply save the doc (iMdl.iDoc) because it will save not only root node but
    // also adjacent nodes. So we need to create new model and add to doc only one separated root
    ChromoMdlX mdl;
    mdl.Set(iRootNode.Handle());
    mdl.Save(aFileName);
    //iMdl.Save(aFileName);
}

ChromoNode ChromoX::CreateNode(void* aHandle)
{
    return ChromoNode(iMdl, aHandle);
}

bool ChromoX::GetSpec(string& aSpec)
{
    return iMdl.ToString(iRootNode.Handle(), aSpec);
}

