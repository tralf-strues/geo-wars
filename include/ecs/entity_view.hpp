/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file entity_view.hpp
 * @date 2022-05-22
 *
 * The MIT License (MIT)
 * Copyright (c) 2022 Nikita Mochalov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include "ecs/entity.hpp"

namespace gwars {

template<typename T>
class EntityView
{
public:
    class Iterator
    {
    private:
        using EntityMapIterator = typename std::unordered_map<EntityId, IComponentHolder*>::iterator;

    public:
        Iterator(EntityMapIterator iterator, EntityManager& manager);

        Iterator& operator++();
        Iterator  operator++(int);

        T&     getComponent() const;
        Entity getEntity() const;

    public:
        friend bool operator==(const Iterator& lhs, const Iterator& rhs) { return lhs.m_Iterator == rhs.m_Iterator; }

        friend bool operator!=(const Iterator& lhs, const Iterator& rhs) { return lhs.m_Iterator != rhs.m_Iterator; }

        friend std::pair<Entity, T&> operator*(const Iterator& it)
        {
            return std::pair<Entity, T&>(it.getEntity(), it.getComponent());
        }

    private:
        EntityMapIterator m_Iterator;
        EntityManager&    m_EntityManager;
    };

public:
    EntityView(EntityMap& entityMap, EntityManager& manager);

    Iterator begin();
    Iterator end();

private:
    EntityMap&     m_EntityMap;
    EntityManager& m_EntityManager;
};

template<typename T>
EntityView<T> getView(EntityManager& manager);

} // namespace gwars

#include "ecs/entity_view.ipp"