#include "cmutex.hpp"
#include "exception.hpp"

namespace Casimir {

    CASIMIR_EXPORT utilities::Mutex::Mutex()
    {

    }

    CASIMIR_EXPORT bool utilities::Mutex::tryAquireLock() {
        // Retrieve the id of the thread
        const std::thread::id id = std::this_thread::get_id();

        // We lock because we do not expect the m_queue to change while performing the search
        // could lead to undefined behavior
        m_internalOperation.lock();
        const bool success = m_queue.empty() || *m_queue.begin() == id;
        if(success && m_queue.empty()) {
            m_queue.insert(m_queue.begin(), id);
        }
        m_internalOperation.unlock();

        // Then simply return the success value
        return success;
    }

    CASIMIR_EXPORT void utilities::Mutex::releaseLock() {
        // Retrieve the id of the thread
        const std::thread::id id = std::this_thread::get_id();

        // We lock because we do not expect the m_queue to change while performing the search
        // could lead to undefined behavior
        m_internalOperation.lock();
        const bool success = !m_queue.empty() && *m_queue.begin() == id;
        if(success) {
            m_queue.pop_front();
        }
        m_internalOperation.unlock();

        // If no success throw exception
        if(!success) {
            CASIMIR_THROW_EXCEPTION("InvalidRequest", "Cannot release the Mutex since you don't actually own the lock");
        }
    }

    CASIMIR_EXPORT void utilities::Mutex::acquireLock() {
        // Retrieve the id of the thread
        const std::thread::id id = std::this_thread::get_id();

        while(true) {
            // We lock because we do not expect the m_queue to change while performing the search
            // could lead to undefined behavior
            m_internalOperation.lock();
            auto it = std::find_if(m_queue.begin(), m_queue.end(), [id](const std::thread::id& cid) { return cid == id; });

            // Determine the position
            cuint position = std::distance(it, m_queue.begin());

            // If the object has not been found
            if(it == m_queue.end()) {
                m_queue.insert(it, id);
            }

            // Release the lock as we do not longer require to have the lock
            m_internalOperation.unlock();

            // Evaluate the position in the list
            if(position == 0) {
                break;
            }
        }
    }

    CASIMIR_EXPORT bool utilities::Mutex::ownLock() {
        // Retrieve the id of the thread
        const std::thread::id id = std::this_thread::get_id();

        // We lock because we do not expect the m_queue to change while performing the search
        // could lead to undefined behavior
        m_internalOperation.lock();
        const bool success = !m_queue.empty() && *m_queue.begin() == id;
        m_internalOperation.unlock();

        return success;
    }


};
