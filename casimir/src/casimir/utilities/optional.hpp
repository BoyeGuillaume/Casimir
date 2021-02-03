#ifndef CASIMIR_OPTIONAL_HPP_
#define CASIMIR_OPTIONAL_HPP_

#include <cstddef>
#include <type_traits>
#include <functional>
#include <iostream>
#include <memory>

#include "../casimir.hpp"
#include "exception.hpp"

namespace Casimir {

    namespace utilities {

        /**
         * @brief Optional class is a modern way to defines an nullable argument object
         * @tparam T the type of the given object
         */
        template<typename T>
        class Optional {
        private:
            typedef std::bool_constant<std::is_move_constructible<T>::value && std::is_copy_constructible<T>::value> 
                _isTCopyMove;

            std::shared_ptr<T> m_ref;

            constexpr explicit Optional(std::shared_ptr<T> data) : m_ref(data) {}

        public:
            /**
             * @brief Default constructor of the Optional object. By default the Optional is empty
             */
            Optional() : m_ref(nullptr) {}

            /**
             * @brief Create a new instance of Optional based on an \link std::shared_ptr<T>
             * @param data the data to be used to construct the instance of Optional<>
             * @return The newly created instance of Optional containing the given data
             */
            inline static Optional<T> of(std::shared_ptr<T> data) {
                return Optional<T>(data);
            }

            /**
             * @brief Create a new instance of Optional based on a pointer onto data of type T
             * @param data A pointer to the data to be used to create the Optional
             * @warning The optional won't delete or clean the data if created with this method
             * @return The newly created instance of Optional containing the given data
             */
            inline static Optional<T> of(T* data) {
                return Optional<T>(std::shared_ptr<T>(data, [](T*) {}));
            }

            /**
             * @brief Create a new instance of Optional based on raw data (doesn't required pointer to operate)
             * @tparam U the type of the create Optional (same as T)
             * @param data Raw data used to create the optional
             * @return The newly created instance of Optional containing the given data
             */
            template<typename U = T, typename = std::enable_if_t<_isTCopyMove::value, bool>>
            inline static Optional<T> of(T data) {
                return Optional<T>(std::shared_ptr<T>(new T(data)));
            }

            /**
             * @brief Create a new instance of Optional based on parameters
             * @tparam argsTypes variable template argument for each of the argument used to create the new instance of T
             * @param args the arguments used to instantiate T
             * @return The newly created instance of Optional containing the given data
             */
            template<class... argsTypes>
            inline static Optional<T> of(argsTypes&&... args) {
                return Optional<>(std::make_shared<>(args));
            }

            /**
             * @brief Return a new instance of \link Optional<> that contain no data
             * @return Empty instance of \link Optional<>
             */
            inline static Optional<T> empty() {
                return Optional<T>();
            }

            /**
             * @brief whether or not the current instance of \link Optional<> hold any value
             * @return whether or not a value is present in the current \link Optional<>
             */
            inline bool isPresent() const {
                return (bool) m_ref;
            }

            /**
             * @brief whether or not the current instance of \link Optional<> hold no value
             * @return whether or not the current \link Optional<> is empty
             */
            inline bool isEmpty() const {
                return ! ((bool) m_ref);
            }

            /**
             * @brief Get the pointer hold by the instance of \link Optional<>
             * @throw \link utilities::Exception if no value is present in the current Optional
             * @return A pointer to the data hold by the current optional.
             * @note The given pointer is safe to use as long as the Optional is allocate. In the case where
             * the \link Optional<> has been instantiate using directly a pointer the pointer is safe-to-use even after.
             */
            inline T* getPtr() const {
#ifdef CASIMIR_SAFE_CHECK
                if (isEmpty()) CASIMIR_THROW_EXCEPTION("NoSuchValueException", "No value was present in the given optional");
#endif
                return m_ref.operator->();
            }

            /**
             * @brief Return the pointer hold by the optional if present or else the replacement value
             * @param replacement the replacement value to be used if the optional is empty
             * @return the pointer hold by the optional if present or else the replacement value
             */
            inline T* orElsePtr(T* replacement) const {
                return (isEmpty()) ?
                    (replacement) :
                    (getPtr());
            }

            /**
             * @brief Get the date hold by the instance of \link Optional<>
             * @throw \link utilities::Exception if no value is present in the current Optional
             * @tparam U the type of the create Optional (same as T)
             * @return the data hold by the optional
             */
            template<typename U = T, typename = std::enable_if_t<_isTCopyMove::value, bool>>
                inline T get() const {
                return *getPtr();
            }

            /**
             * @brief Return the value  hold by the optional if present or else the replacement value
             * @tparam U the type of the create Optional (same as T)
             * @param replacement the replacement value to be used if the optional is empty
             * @return the value hold by the optional if present or else the replacement value
             */
            template<typename U = T, typename = std::enable_if_t<_isTCopyMove::value, bool>>
            inline T orElse(T replacement) const {
                return *orElsePtr(&replacement);
            }

            /**
             * @brief Same as \link utilities::Optional<T>::getPtr
             * @throw \link utilities::Exception if no value is present in the current Optional
             * @return A pointer to the data hold by the current optional.
             * @note The given pointer is safe to use as long as the Optional is allocate. In the case where
             * the \link Optional<> has been instantiate using directly a pointer the pointer is safe-to-use even after.
             */
            inline T* operator->() const {
                return getPtr();
            }

            /**
             * @brief Return a reference of the data hold by the \link utilities::Optional<T>
             * @throw \link utilities::Exception if no value is present in the current Optional
             * @return A reference to the data hold by the optional
             */
            inline T& operator*() const {
                return *getPtr();
            }

            /**
             * @brief Set the data hold by the optional
             * @param ref the new data hold by the optional
             * @return self-reference
             */
            inline Optional<T>& set(T* ref) {
                m_ref = std::shared_ptr<T>(ref, [](T*) {});
                return *this;
            }

            /**
             * @brief Reset any data present in the optional
             * @return self-reference
             */
            inline Optional<T> reset() {
                return set(nullptr);
            }

            /**
             * @brief map the \link Optional<> to another \link Optional<> of another type
             * @tparam U the destination type (type of the Optional after the mapping)
             * @param mapper Function used to map from type T to type U
             * @return new instance of Optional that is empty if the current \link Optional<> is empty
             * otherwise the mapped version of the current data
             */
            template<typename U, typename = std::enable_if_t<_isTCopyMove::value &&
                std::is_move_constructible<U>::value && std::is_copy_constructible<U>::value, bool>>
            Optional<U> map(std::function<U(T)> mapper) {
                return isPresent() ?
                    Optional<U>::of(mapper(get())) :
                    Optional<U>::empty();
            }
        };

    };

};

#endif
