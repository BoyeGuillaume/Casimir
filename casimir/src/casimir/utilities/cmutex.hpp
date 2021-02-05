#ifndef CASIMIR_CMUTEX_HPP_
#define CASIMIR_CMUTEX_HPP_

#include <mutex>
#include <atomic>
#include <list>
#include <algorithm>
#include <thread>

#include "../casimir.hpp"

namespace Casimir {

        namespace utilities {

            /**
             * @brief Mutex class defines a complex mutex behavior that use queue for lock and release operations
             */
            class Mutex {
                CASIMIR_DISABLE_COPY_MOVE(Mutex);
            private:
                std::mutex m_internalOperation;
                std::list<std::thread::id> m_queue;

            public:
                /**
                 * @brief Default constructor for the Mutex
                 */
                CASIMIR_EXPORT Mutex();

                /**
                 * @brief acquire the lock if free otherwise do nothing
                 * @return the success of the operation describe above
                 */
                CASIMIR_EXPORT bool tryAquireLock();

                /**
                 * @brief release the lock owned by the current thread
                 * @throw Exception if the current thread doesn't actually own the lock
                 */
                CASIMIR_EXPORT void releaseLock();

                /**
                 * @brief acquire the lock. This operation block the thread until the lock is acquired
                 */
                CASIMIR_EXPORT void acquireLock();

                /**
                 * @brief Determine if the current thread own the lock
                 * @return the result of the above check
                 */
                CASIMIR_EXPORT bool ownLock();
            };

        };

}


#endif
