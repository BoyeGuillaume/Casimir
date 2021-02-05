#ifndef CASIMIR_PRIVATE_CONTEXT_HPP_
#define CASIMIR_PRIVATE_CONTEXT_HPP_

#include "../configuration.hpp"
#include "../utilities/uuid.hpp"

namespace Casimir {

    class PrivateCasimirContext {
    public:
        CASIMIR_EXPORT PrivateCasimirContext();
        CASIMIR_EXPORT ~PrivateCasimirContext();
    };

    namespace PrivateLogging {
        static const utilities::Uuid Warning = utilities::Uuid(17707170361395375139U, 4856449981596320257U);
        static const utilities::Uuid Error   = utilities::Uuid(9631149747179961252U, 4669447320871580547U);
        static const utilities::Uuid Note    = utilities::Uuid(2499535614363860142U, 15273922473455247291U);
        static const utilities::Uuid Info    = utilities::Uuid(5424077037978735843U, 1871455045818645681U);
    }

};

#endif
