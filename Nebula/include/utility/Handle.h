//
// Created by michal-swiatek on 03.12.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef HANDLE_H
#define HANDLE_H

#include <atomic>
#include <boost/bimap.hpp>
#include <boost/bimap/unordered_set_of.hpp>

#include "core/Assert.h"

namespace nebula { // NOLINT(*-dcl58-cpp)

    template <
        typename ObjectType,
        typename HashFunctor = boost::hash<ObjectType>,
        typename EqualKey = std::equal_to<ObjectType>
    >
    class HandleMapper
    {
    public:
        using HandleType = uint32_t;
        static constexpr HandleType NULL_HANDLE = 0;

    private:
        using boost_handle_set = boost::bimaps::unordered_set_of<HandleType>;
        using boost_object_set = boost::bimaps::unordered_set_of<ObjectType, HashFunctor, EqualKey>;
        using bimap_type = boost::bimap<boost_handle_set, boost_object_set>;
        using bimap_value_type = typename bimap_type::value_type;

    public:
        HandleType getHandle(const ObjectType& object)
        {
            if (contains(object))
                return m_bimap.right.at(object);

            m_bimap.insert(bimap_value_type(++m_counter, object));
            return m_counter;
        }

        const ObjectType& getObject(HandleType handle)
        {
            NB_CORE_ASSERT(contains(handle), std::format("No object with handle {}!", handle));
            return m_bimap.left.at(handle);
        }

        [[nodiscard]] bool contains(const ObjectType& object) const
        {
            return m_bimap.right.find(object) != m_bimap.right.end();
        }

        [[nodiscard]] bool contains(const HandleType& handle) const
        {
            return m_bimap.left.find(handle) != m_bimap.left.end();
        }

    private:
        bimap_type m_bimap{};
        std::atomic_uint32_t m_counter = NULL_HANDLE;
    };

}

#endif //HANDLE_H
