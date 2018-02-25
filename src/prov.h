#ifndef __DESMAG_PROV_H
#define __DESMAG_PROV_H

#include <map>
#include <mdes.h>

#include "mprov.h"

namespace desmag {

    using namespace std;

    class Elem;

    /**
     * Base class for providers
     */
    class ProviderBase: public MProvider
    {
	public:
	    typedef pair<string, Elem*> TRegVal;
	    typedef map<string, Elem*> TReg;
	public:
	    ProviderBase(const string& aName, MEnv* aEnv);
	    virtual ~ProviderBase();
	    // From MProvider
	    virtual const string& Name() const { return iName;};
	    virtual Elem* CreateNode(const string& aType, const string& aName, MEnv* aEnv);
	    virtual Elem* GetNode(const string& aUri);
	    virtual bool IsProvided(const Elem* aElem) const;
	    virtual Chromo* CreateChromo();
	    virtual void AppendNodesInfo(vector<string>& aInfo);
	    virtual const string& ModulesPath() const;
	    virtual void SetEnv(MEnv* aEnv);
	protected:
	    MEnv* iEnv;
	    TReg iReg;
	    string iName;
    };

}

#endif
