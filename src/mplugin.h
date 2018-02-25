#ifndef __DESMAG_MPLUGIN_H
#define __DESMAG_MPLUGIN_H

#include <mprov.h>
#include <menv.h>

namespace desmag {

    // Each plugin has to export the function named "init" with this type

    typedef MProvider* plugin_init_func_t(MEnv* aEnv);

}


#endif
