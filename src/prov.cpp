#include "prov.h"
#include "elem.h"

using namespace desmag;
using namespace std;

const string KModPath;

ProviderBase::ProviderBase(const string& aName, MEnv* aEnv): iName(aName), iEnv(aEnv)
{
}

ProviderBase::~ProviderBase() 
{
    while (!iReg.empty()) {
	Elem* elem = iReg.begin()->second;
	delete elem;
	iReg.erase(iReg.begin());
    }
}

void ProviderBase::SetEnv(MEnv* aEnv)
{
    assert(iEnv == NULL || aEnv == NULL);
    iEnv = aEnv;
}

Elem* ProviderBase::CreateNode(const string& aType, const string& aName, MEnv* aEnv)
{
    return NULL;
}

Elem* ProviderBase::GetNode(const string& aUri)
{
    return NULL;
}

bool ProviderBase::IsProvided(const Elem* aElem) const
{
    bool res = false;
    for (TReg::const_iterator it = iReg.begin(); it != iReg.end() && !res; it++) {
	res = aElem == it->second;
    }
    return res;
}

Chromo* ProviderBase::CreateChromo()
{
    return NULL;
}

void ProviderBase::AppendNodesInfo(vector<string>& aInfo)
{
}

const string& ProviderBase::ModulesPath() const
{
    return KModPath;
}
