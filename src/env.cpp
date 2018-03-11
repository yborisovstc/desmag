#include "env.h"
#include "elem.h"
#include "provmgr.h"

using namespace desmag;


const string KRootName = "Root";

Env::Env(const string& aChromoPath, const string& aLogFilePath): MBase("Env"),
    mChromoOutHandler(*this),
    mChromoPath(aChromoPath), mProvider(nullptr), mOutChromo(nullptr)
{
    mProvider = new ProvidersMgr("Provider", this);
    mOutChromo = new TConnPoint<MStateNotifier, MStateObserver<ChromoX>>("OutChromo", nullptr, MConnPoint::EOutput, mChromoOutHandler);
}

Env::~Env()
{
    if (mProvider) {
	delete mProvider;
    }
    if (mOutChromo) {
	delete mOutChromo;
    }
}

MProvider* Env::Provider() const
{
    return mProvider;
}

void Env::ConstructSystem()
{
    // Create root system
    Chromo *spec = mProvider->CreateChromo();
    if (mChromoPath.empty()) {
	mRoot = new Elem(KRootName, this);
    } else {
	spec->Set(mChromoPath.c_str());
	const ChromoNode& root = spec->Root();
	string sparent = root.Attr(ENa_Parent);
	mRoot = new Elem("Root", this, true);
	Connect(*mOutChromo, *mRoot->mInpBMut);
	//Logger()->Write(EInfo, iRoot, "Started of creating system, spec [%s]", iSpecFile.c_str());
	/*
	TConnPoint<MStateNotifier, MStateObserver<ChromoX>>& inpMut = dynamic_cast<TConnPoint<MStateNotifier, MStateObserver<ChromoX>>&>(mRoot->mInpBMut->Orig());
	MIface* iface = inpMut.Required().begin()->second;
	MStateObserver<ChromoX>* obs = dynamic_cast<MStateObserver<ChromoX>*>(iface);
	ChromoX* specx = dynamic_cast<ChromoX*>(spec);
	obs->OnStateChanged(nullptr, *specx);
	*/
	ChromoX* specx = dynamic_cast<ChromoX*>(spec);
	for (auto& iobs : mOutChromo->Required()) {
	    MStateObserver<ChromoX>* obs = *iobs.second;
	    obs->OnStateChanged(&mChromoOutHandler, *specx);
	}
    }
}

void Env::HandleChromoOutStateHandled(MIface* aObserver)
{
}

