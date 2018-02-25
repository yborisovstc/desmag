#ifndef __DESMAG_ENV_H__
#define __DESMAG_ENV_H__

#include <mdes.h>
#include <connpoint.h>

#include "menv.h"
#include "mprov.h"

namespace desmag {

    using namespace std;

    class Elem;

    /**
     * @brief Mutliagets system environment
     */
    class Env: public MBase, public MEnv
    {
	friend class ChromoOutHandler;
	public:
	    class ChromoOutHandler: public MStateNotifier {
		public:
		    ChromoOutHandler(Env& aHost): mHost(aHost) {};
		    // From MStateNotifier
		    virtual void OnStateChangeHandled(MIface* aObserver) { mHost.HandleChromoOutStateHandled(aObserver);};
		private:
		    Env& mHost;
	    };

	public:
	    Env(const string& aChromoPath, const string& aLogFilePath = string());
	    virtual ~Env();
	    // From MEnv
	    virtual MProvider* Provider() const;
	    virtual void ConstructSystem();
	    virtual Elem* Root() { return mRoot;};
	private:
	    void HandleChromoOutStateHandled(MIface* aObserver);
	private:
	    ChromoOutHandler mChromoOutHandler;
	    MProvider* mProvider;
	    Elem* mRoot;
	    string mChromoPath;
	    ConnPoint* mOutChromo;
    };

}

#endif
