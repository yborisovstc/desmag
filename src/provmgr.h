#ifndef __DESMAG_PROVMGR_H
#define __DESMAG_PROVMGR_H

#include <map>

#include <mdes.h>
#include "mprov.h"

namespace desmag {

    using namespace std;

    class ProvidersMgr: public MBase, public MProvider
    {
	public:
	    typedef map<string, MProvider*> TProviders;
	    typedef pair<string, MProvider*> TProvidersElem;
	public:
	    static const char* Type() { return "ProvidersMgr";};
	    ProvidersMgr(const string& aName, MEnv* aEnv);
	    virtual ~ProvidersMgr();
	    bool LoadPlugin(const string& aName);
	    void LoadPlugins();
	    void AddProvider(MProvider* aProv);
	    void RemoveProvider(MProvider* aProv);
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
//	    static int FilterPlgDirEntries(const struct dirent* aEntry);
	protected:
	    TProviders iProviders;
	    MEnv* iEnv;
	    string iName;
    };

}


#endif
