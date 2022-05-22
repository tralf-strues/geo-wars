/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file entity_Entityview.ipp
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

namespace gwars {

template<typename T>
EntityView<T>::Iterator::Iterator(EntityMapIterator iterator, EntityManager& manager)
    : m_Iterator(iterator), m_EntityManager(manager)
{
}

template<typename T>
typename EntityView<T>::Iterator& EntityView<T>::Iterator::operator++()
{
    ++m_Iterator;
    return *this;
}

template<typename T>
typename EntityView<T>::Iterator EntityView<T>::Iterator::operator++(int)
{
    Iterator temp{*this};
    ++(*this);
    return temp;
}

template<typename T>
T& EntityView<T>::Iterator::getComponent() const
{
    ComponentHolder<T>* holder = reinterpret_cast<ComponentHolder<T>*>(m_Iterator->second);
    assert(holder != nullptr);
    return holder->get();
}

template<typename T>
Entity EntityView<T>::Iterator::getEntity() const
{
    return Entity{m_Iterator->first, m_EntityManager};
}

template<typename T>
EntityView<T>::EntityView(EntityMap& entityMap, EntityManager& manager) : m_EntityMap(entityMap), m_EntityManager(manager)
{
}

template<typename T>
typename EntityView<T>::Iterator EntityView<T>::begin()
{
    return Iterator{m_EntityMap.begin(), m_EntityManager};
}

template<typename T>
typename EntityView<T>::Iterator EntityView<T>::end()
{
    return Iterator{m_EntityMap.end(), m_EntityManager};
}

template<typename T>
EntityView<T> getView(EntityManager& manager)
{
    return EntityView<T>{manager.getEntityMap<T>(), manager};
}

} // namespace gwars