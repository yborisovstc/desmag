#include <connpoint.h>
#include <state.h>

#include "elem.h"
#include "chromo.h"
#include "uri.h"

using namespace desmag;
using namespace desa;

/**
 * @brief Change status state
 *
 * Generates status of change from proposed mutation
 */
class StateChangeSt: public TState<TAChange>
{
    public:
	StateChangeSt(const string& aName): TState<TAChange>(aName, nullptr, EAc_None), mInpMut("Inp_mut", mSobs) {}
    protected:
	virtual void Trans() {
	    auto &inp = mInpMut.Data().begin()->second;
	    const TMut& mut = inp.second;
	    if (mut.Type() == ENt_Node) {
		mUpd = EAc_Add;
	    } else if (mut.Type() == ENt_Rm) {
		mUpd = EAc_Rm;
	    } else if (mut.Type() == ENt_Cont) {
		mUpd = EAc_Content;
	    }
	};
    public:
	StateInput<pair<int, TMut>> mInpMut;
};


/**
 * @brief Controller state of system's chromo
 *
 * Controler state has an access to internal system's interface in order
 * to change the system structure.
 */
class StateChromo: public TState<TMut>
{
    public:
	StateChromo(const string& aName, MAMutable& aMutable): TState<TMut>(aName, nullptr, TMut()), mInpMut("Inp_mut", mSobs), mMutable(aMutable) {
	}
    protected:
	virtual void Trans() {
	    auto &inp = mInpMut.Data().begin()->second;
	    mUpd = inp;
	    mMutable.AddElem(mUpd, false);

	};
    public:
	StateInput<TMut> mInpMut;
	MAMutable& mMutable;
};

/**
 * @brief State of batch mutation
 * Batch mutation gets as input of ChromoX type
 * State data is the counter of muations applied
 * Count value -1 indicates that all mutations are applied
 * TODO We need to use combined state here - pair of counter and mutation. Otherwise we would need two states:
 * one for counter and second for current mutation. But in that case we would have two inputs of type ChromoX, that
 * would cause big overhead. To consider implementing shared input in desa and use it here, ref desa::uc_007 
 * Then two separate states will follow proper design and  not cause the overhead.
 */
class StateBmut: public TState<pair<int, TMut>>
{
    public:
	StateBmut(const string& aName): TState<pair<int, TMut>>(aName, nullptr, pair<int, TMut>(-1, TMut())), mInpBmut("Inp_bmut", mSobs) {
	}
    protected:
	virtual void Trans() {
	    auto &muts = mInpBmut.Data().begin()->second;
	    if (mConf.first == -1) {
		mUpd.first = 0;
	    } else {
		mUpd.first = mConf.first + 1;
	    }
	    ChromoNode mut = muts.Root().At(mUpd.first);
	    if (mut.Handle() == nullptr) {
		mUpd.first = -1;
	    } else {
		mUpd.second = mut;
	    }
	};
    public:
	StateInput<ChromoX> mInpBmut;
};


/**
 * @brief State: approval of component's proposed change
 */
class StateChangeAppr: public TState<bool>
{
    public:
	StateChangeAppr(const string& aName): TState<bool>(aName, nullptr, false), mInpChange("Inp_change", mSobs) {}
    protected:
	virtual void Trans() {
	    auto &inp = mInpChange.Data().begin()->second;
	    mUpd = false;
	};
    public:
	StateInput<TAChange> mInpChange; //!< Input: component proposed change
};

/**
 * @brief State: mutation applied
 *
 * Applied mutation from mutation proposed and approval from owner
 * Acts as controller using internal system function for mutating
 */
class StateChangeAppl: public TState<Uri>
{
    public:
	StateChangeAppl(const string& aName, MAMutable& aMutable, MElemContext& aEctx): TState<Uri>(aName, nullptr,
		Uri()), mInpMut("Inp_Mut", mSobs), mInpAppr("Inp_Appr", mSobs), mMutable(aMutable), mEctx(aEctx)  {}
    protected:
	virtual void Trans() {
	    bool isOwned = mInpAppr.IsConnected();
	    auto &approved = mInpAppr.Data().begin()->second;
	    auto &mut = mInpMut.Data().begin()->second.second;
	    // Assume that any change is approved if there is no owner
	    if (approved || !isOwned) {
		if (mut.Type() == ENt_Node) {
		    Uri puri = mut.Attr(ENa_Parent);
		    mUpd = puri;
		    //mMutable.AddElem(mut, false);
		}
	    }
	    // Append the mutation to chromo
	    mEctx.getChromo()->Root().AddChild(mut);
	};
    public:
	StateInput<pair<int, TMut>> mInpMut; //!< Input: mutation proposed
	StateInput<bool> mInpAppr; //!< Input: Approval from owner
    private:
	MAMutable& mMutable;
	MElemContext& mEctx;
};


//Elem::Elem(const string& aName, MAOwner* aOwner, MEnv* aEnv): desa::System(aName, (aOwner != nullptr) ? aOwner->getOwner():nullptr), mEnv(aEnv)
Elem::Elem(const string& aName, MEnv* aEnv, bool aIsRoot): desa::System(aName, nullptr), mEnv(aEnv)
{
    // Create context (data that doen't take part in elem movement
    // Chromosome of element
    mChromo = Provider()->CreateChromo();
    mChromo->Init(ENt_Node);
    // State change from proposed mut
    StateChangeSt* st_chg = new StateChangeSt("St_Chg");
    st_chg->EnableDanglingOutput();
    AddComp(st_chg);
    // State mut applied
    StateChangeAppl* st_appl = new StateChangeAppl("St_Appl", *this, *this);
    st_appl->EnableDanglingOutput();
    AddComp(st_appl);
    // State: comp change approver
    StateChangeAppr* st_appr = new StateChangeAppr("St_Appr");
    st_appr->EnableDanglingOutput();
    AddComp(st_appr);
    // State batch mut
    StateBmut* st_bmut = new StateBmut("St_Bmut");
    AddComp(st_bmut);
    // Bool constant "true"
    TState<bool>* st_true = new TState<bool>("St_True", *this, true);
    AddComp(st_true);
    st_true->EnableDanglingOutput();
    // Inputs
    mInpBMut = new ExtStateInp<ChromoX>("Inp_BMut");
    mInpMut = new ExtStateInp<pair<int, TMut>>("Inp_Mut");
    mInpCompChg = new ExtStateInp<TAChange>("Inp_CompChg");
    mInpAppr = new ExtStateInp<bool>("Inp_Appr");
    // Outputs
    mOutChg = new ExtStateOut<TAChange>("Out_Chg");
    mOutChgAppr = new ExtStateOut<bool>("Out_Chg_Appr");
    // Connections
    // Inputs
//    assert(Connect(mInpMut->Orig(), st_chg->mInpMut));
//    assert(Connect(mInpMut->Orig(), st_appl->mInpMut));
    assert(Connect(mInpCompChg->Orig(), st_appr->mInpChange)); // Inp#Comp_change_proposed -- State#approval_of_component's_proposed_change/Inp#Change  
    assert(Connect(mInpAppr->Orig(), st_appl->mInpAppr));
    assert(Connect(mInpBMut->Orig(), st_bmut->mInpBmut));
    // Outputs
    assert(Connect(st_chg->Output(), mOutChg->Orig()));
    assert(Connect(st_appr->Output(), mOutChgAppr->Orig()));
    if (aIsRoot) {
	// In case of root, connect chromo directly to mut applier
	assert(Connect(st_bmut->Output(), st_appl->mInpMut));
	// assert(Connect(st_true->Output(), st_appl->mInpAppr)); // No need, the state checks approver existence by itself
    } else {
	assert(Connect(st_bmut->Output(), st_chg->mInpMut));
    }
}

Elem::~Elem()
{
    delete mInpMut;
    delete mInpCompChg;
    delete mInpAppr;
    delete mOutChg;
    delete mOutChgAppr;
    if (mChromo != nullptr) {
	delete mChromo;
    }
}

void Elem::AddElem(const TMut& aSpec, bool aRunTime)
{
    assert(aSpec.Type() == ENt_Node);
    string name = aSpec.Attr(ENa_Id);
    Elem* comp = new Elem(name, mEnv);
    AddComp(comp);
    assert(Connect(*comp->mOutChg, *mInpCompChg)); // Comp.outp_change_proposed -- Inp_comp_change_proposed 
    assert(Connect(*mOutChgAppr, *comp->mInpAppr));
}
