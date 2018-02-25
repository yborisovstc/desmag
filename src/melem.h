#ifndef __DESMAG_MELEM_H__
#define __DESMAG_MELEM_H__

#include <miface.h>

namespace desmag {

    using namespace desa;

    class TMut;

    /**
     * @brief States of agent's change
     */

    enum TAChange
    {
	EAc_None,
	EAc_Add, /**< Adding component */
	EAc_Rm,  /**< Removing component */
	EAc_Content, /**< Changing content via mutation */
	EAc_Change, /**< Changing content */
    };
     
    /**
     * @brief Interface of mutable elem of multi-agents system
     */
    class MAMutable
    {
	public:
	    virtual void AddElem(const TMut& aSpec, bool aRunTime = false) = 0;
    };


    /**
     * @brief Interface of owning node in agents native hierarchy
     */
    // TODO [YB] To avoid using this iface. getOwner doesn't satisfy async iface req (waits for responce)
    /*
    class MAOwner: public MIface
    {
	public:
	    virtual MOwner* getOwner() = 0;
    };
    */


}

#endif // __DESMAG_MELEM_H__

