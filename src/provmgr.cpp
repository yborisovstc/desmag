#include "prov.h"
#include "provmgr.h"
#include "provdef.h"
#include "mplugin.h"
#include <dlfcn.h>
#include <dirent.h>

using namespace std;

int FilterPlgDirEntries(const struct dirent *aEntry)
{
    string name = aEntry->d_name;
    size_t ppos = name.find_first_of(".");
    string suff = name.substr(ppos + 1);
    int res = suff.compare("so"); 
    return (res == 0) ? 1 : 0;
}

using namespace desmag;

// Plugins default dir
string KPluginDir = "/usr/lib/desmag/plugins/";

ProvidersMgr::ProvidersMgr(const string& aName, MEnv* aEnv): MBase(aName), iEnv(aEnv)
{
    ProviderBase* baseprov = new ProvDef("ProvDef", iEnv);
    assert(baseprov != NULL);
    AddProvider(baseprov);
}

ProvidersMgr::~ProvidersMgr()
{
    while (!iProviders.empty()) {
	MProvider* prov = iProviders.begin()->second;
	delete prov;
	iProviders.erase(iProviders.begin());
    }
}

Elem* ProvidersMgr::CreateNode(const string& aType, const string& aName, MEnv* aEnv)
{
    Elem* res = NULL;
    for (map<string, MProvider*>::iterator it = iProviders.begin(); it != iProviders.end() && res == NULL; it++) {
	res = it->second->CreateNode(aType, aName, aEnv);
    }
    return res;
}

Elem* ProvidersMgr::GetNode(const string& aUri)
{
    Elem* res = NULL;
    for (TProviders::iterator it = iProviders.begin(); it != iProviders.end() && res == NULL; it++) {
	res = it->second->GetNode(aUri);
    }
    return res;
}

bool ProvidersMgr::IsProvided(const Elem* aElem) const
{
    bool res = false;
    for (TProviders::const_iterator it = iProviders.begin(); it != iProviders.end() && !res; it++) {
	res = it->second->IsProvided(aElem);
    }
    return res;

}

Chromo* ProvidersMgr::CreateChromo()
{
    Chromo* res = NULL;
    for (TProviders::iterator it = iProviders.begin(); it != iProviders.end() && res == NULL; it++) {
	res = it->second->CreateChromo();
    }
    return res;
}

bool ProvidersMgr::LoadPlugin(const string& aName)
{
    bool res = false;
    MProvider* prov = NULL;
    string plgpath = KPluginDir + aName;
    void* handle = dlopen(plgpath.c_str(), RTLD_NOW|RTLD_LOCAL|RTLD_DEEPBIND);
    if (handle != NULL) {
	dlerror();
	plugin_init_func_t* init = (plugin_init_func_t*) dlsym(handle, "init");
	char* str_error = dlerror();
	if (init!= NULL) {
	    prov = init(iEnv);
	    if (prov != NULL) {
		res = true;
		AddProvider(prov);
	    }
	}
	if (!res) {
	    dlclose(handle);
	}
    }
    return res;
}

void ProvidersMgr::LoadPlugins()
{
    // List plugins directory
    struct dirent **entlist;
    int n = scandir(KPluginDir.c_str(), &entlist, FilterPlgDirEntries, alphasort);
    // Load plugins
    for (int cnt = 0; cnt < n; ++cnt) {
	LoadPlugin(entlist[cnt]->d_name);
    }
}

void ProvidersMgr::AddProvider(MProvider* aProv)
{
    // TODO To support name
    TProviders::const_iterator res = iProviders.find(aProv->Name());
    assert(res == iProviders.end());
    iProviders.insert(TProvidersElem(aProv->Name(), aProv));
}

void ProvidersMgr::RemoveProvider(MProvider* aProv)
{
    assert(iProviders.count(aProv->Name()) > 0);
    iProviders.erase(aProv->Name());
}

void ProvidersMgr::AppendNodesInfo(vector<string>& aInfo)
{
    for (TProviders::iterator it = iProviders.begin(); it != iProviders.end(); it++) {
	it->second->AppendNodesInfo(aInfo);
    }
}

const string& ProvidersMgr::ModulesPath() const
{
    MProvider* defprov = iProviders.at("ProvDef");
    assert(defprov != NULL);
    return defprov->ModulesPath();
}

void ProvidersMgr::SetEnv(MEnv* aEnv)
{
    assert(iEnv == NULL || aEnv == NULL);
    iEnv = aEnv;
}
