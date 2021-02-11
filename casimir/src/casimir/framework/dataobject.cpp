#include "dataobject.hpp"

#include <utility>

#include "../utilities/exception.hpp"
#include "../core/private-context.hpp"

namespace Casimir::framework {
    
    using namespace literals;
    using namespace utilities;
    
    CASIMIR_EXPORT DataObject::DataObject(CasimirContext ctx, std::vector<DataChunk> chunks)
    : ContextualObject(ctx),
    m_chunkObject(std::move(chunks)) {
#ifdef CASIMIR_SAFE_CHECK
        // First check the chunk share the same context
        for(const DataChunk& chunk : chunks) {
            if (chunk.ctx() != ctx) {
                CASIMIR_THROW_EXCEPTION("ContextError",
                                        "The different data of a same dataObject MUST share the same context"
                                        "as the data object");
            }
        }
        
        // Assert there's at least one chunks
        if(chunks.empty()) {
            CASIMIR_THROW_EXCEPTION("InvalidArgument", "Any DataObject must have at least one chunk");
        }
        
        // Assert all the chunks has the same length
        for(const DataChunk& chunk : chunks) {
            if(chunk.length() != chunks[0].length()) {
                CASIMIR_THROW_EXCEPTION("InvalidArgument", "All chunks must have the same length");
            }
            if(chunk.dtype() != chunks[0].dtype()) {
                CASIMIR_THROW_EXCEPTION("InvalidArgument", "Incompatible types. Each chunks of a same DataObject must have"
                                                           "the same types");
            }
        }
#endif
        for(const DataChunk& chunk : chunks) {
            m_chunkById.insert(std::make_pair(chunk.rawData()->allocator()->uuid(), chunk));
        }
    }
    
    CASIMIR_EXPORT DataObject::~DataObject() = default;
    
    CASIMIR_EXPORT void DataObject::copy(const utilities::Uuid& destination, const utilities::Uuid& source) {
        const auto& dest = m_chunkById.find(destination);
        const auto& src = m_chunkById.find(source);
#ifdef CASIMIR_EXPORT
        if(dest == m_chunkById.end() || src == m_chunkById.end()) {
            CASIMIR_THROW_EXCEPTION("InvalidArgument", "The current DataObject don't hold any chunk of the given interface");
        }
#endif
        // Perform the copy from one interface to another
        DataChunk::copy(&dest->second, &src->second);
    }
    
    CASIMIR_EXPORT bool DataObject::has(const utilities::Uuid& interface) const {
        const auto& it = m_chunkById.find(interface);
        return it != m_chunkById.end();
    }
    
    CASIMIR_EXPORT DataChunk DataObject::at(const utilities::Uuid& interface) const {
        const auto& it = m_chunkById.find(interface);
#ifdef CASIMIR_SAFE_CHECK
        if(it == m_chunkById.end()) {
            CASIMIR_THROW_EXCEPTION("InvalidArgument", "Cannot found the given uuid as parameters of the structure");
        }
#endif
        return it->second;
    }
    
    CASIMIR_EXPORT utilities::String DataObject::toString() const {
        return "DataObject(size=" + String::toString((cint) m_chunkObject.size()) + ")";
    }
    
    
    CASIMIR_EXPORT DataObjectBuilder::~DataObjectBuilder() {
        for(const DataChunk& chunks : *m_chunks) {
            delete chunks.rawData();
        }
        m_chunks->clear();
    }
    
    CASIMIR_EXPORT DataObjectBuilder& DataObjectBuilder::registerInterface(const Uuid& interface) {
        // Then retrieve the interface with the given `uuid`
        const auto& it = ctx()->interfacesByUuid.find(interface);
#ifdef CASIMIR_SAFE_CHECK
        if(it == ctx()->interfacesByUuid.end()) {
            CASIMIR_THROW_EXCEPTION("InvalidArgument", "The given interface uuid doesn't correspond to anything in the current context");
        }
#endif
        // Then allocator the rawData
        const cuint requiredSize = m_length * m_dataType.sizeOf();
        RawData* rawData = it->second->allocator()->allocate(requiredSize);
        
        // Create the corresponding Chunk
        DataChunk chunk(ctx(), rawData, m_dataType, 0, m_length);
        m_chunks->push_back(chunk);
        
        // Return self-reference
        return *this;
    }
    
    CASIMIR_EXPORT DataObject* DataObjectBuilder::create() {
        auto* dataObject = new DataObject(ctx(), *m_chunks);
        m_chunks->clear();
        return dataObject;
    }
}
