#ifndef CASIMIR_CONTEXTABLEOBJECT_HPP_
#define CASIMIR_CONTEXTABLEOBJECT_HPP_

#include "../casimir.hpp"
#include "../core/context.hpp"

namespace Casimir::framework {
 
    class ContextualObject {
    private:
        CasimirContext m_ctx;

    public:
        inline explicit ContextualObject(CasimirContext ctx) : m_ctx(ctx) {}

    protected:
        inline PrivateCasimirContext* ctx() const {
            return m_ctx;
        }
    };
    
}

#endif
