#ifndef __DESMAG_PROVDEF_H
#define __DESMAG_PROVDEF_H

#include "prov.h"

namespace desmag {

    class ProvDef: public ProviderBase
    {
	public:
	    ProvDef(const string& aName, MEnv* aEnv);
	    virtual ~ProvDef();
	    // From MProvider
	    virtual Elem* CreateNode(const string& aType, const string& aName, MEnv* aEnv);
	    virtual Elem* GetNode(const string& aUri);
	    virtual Chromo* CreateChromo();
	    virtual void AppendNodesInfo(vector<string>& aInfo);
	    virtual const string& ModulesPath() const;
	private:
	    vector<string> iNodesInfo;
    };

}


#endif
