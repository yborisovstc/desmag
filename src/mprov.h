#ifndef __DESMAG_MPROV_H__
#define __DESMAG_MPROV_H__

#include <string>
#include <vector>

namespace desmag {

    using namespace desa;
    using namespace std;

    class Elem;
    class Chromo;
    class MEnv;

    class MProvider
    {
	public:
	    virtual ~MProvider() {};
	    virtual const string& Name() const = 0;
	    virtual Elem* CreateNode(const string& aType, const string& aName, MEnv* aEnv) = 0;
	    virtual Elem* GetNode(const string& aUri) = 0;
	    virtual bool IsProvided(const Elem* aElem) const = 0;
	    virtual Chromo* CreateChromo() = 0;
	    virtual void AppendNodesInfo(vector<string>& aInfo) = 0;
	    virtual const string& ModulesPath() const = 0;
	    virtual void SetEnv(MEnv* aEnv) = 0;
    };

}

#endif // __DESMAG_MPROV_H__
