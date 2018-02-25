#ifndef __DESMAG_ELEM_H__
#define __DESMAG_ELEM_H__

#include <system.h>
#include <connpoint.h>

#include "menv.h"
#include "melem.h"
#include "chromo.h"
#include "chromox.h"

namespace desmag {

    using namespace desa;
    using namespace std;

    /**
     * @brief Interface of context of Elem's states
     */
    class MElemContext
    {
	public:
	    virtual Chromo* getChromo() = 0;
    };


    /**
     * @brief Base class of agent in mutli-agents system
     */
    class Elem: public System, public MAMutable/*, public MAOwner*/, public MElemContext
    {
	public:
	    //Elem(const string& aName, MAOwner* aOwner, MEnv* aEnv);
	    Elem(const string& aName, MEnv* aEnv, bool aIsRoot = false);
	    virtual ~Elem();
	    // From MAMutable
	    virtual void AddElem(const TMut& aSpec, bool aRunTime = false);
	    // From MAOwner
	    //virtual MOwner* getOwner() {return mOwnerImpl;};
	    // From MElemContext
	    virtual Chromo* getChromo() { return mChromo;};
	protected:
	    inline MProvider* Provider() const { return mEnv ? mEnv->Provider(): nullptr; } ;
	protected:
	    MEnv* mEnv; // Environment
	    Chromo* mChromo; // Chromosome of element
	public:
	    // Inputs
	    ExtStateInp<ChromoX>* mInpBMut; // Input, Batch Mutation
	    ExtStateInp<pair<int, TMut>>* mInpMut; // Input, Mutation
	    ExtStateInp<TAChange>* mInpCompChg; // Input, Comp change proposed
	    ExtStateInp<bool>* mInpAppr; // Input, Approval from owner for comp change
	public:
	    // OUtputs
	    ExtStateOut<TAChange>* mOutChg; // Output, Change proposed
	    ExtStateOut<bool>* mOutChgAppr; // Output, Approval of component proposed change
    };


}

#endif // __DESMAG_ELEM_H__
