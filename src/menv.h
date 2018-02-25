#ifndef __DESMAG_MENV_H__
#define __DESMAG_MENV_H__

#include <miface.h>
#include <mprov.h>

namespace desmag {

    using namespace desa;

    /**
     * @brief Interface of multi-agent environment
     */
    class MEnv: public MIface
    {
	public:
	    virtual MProvider* Provider() const = 0;
	    virtual void ConstructSystem() = 0;
	    virtual Elem* Root() = 0;
    };

}

#endif // __DESMAG_MENV_H__

