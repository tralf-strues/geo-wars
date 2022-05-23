/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file entity_manager.ipp
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
EventComponentConstruct<T>::EventComponentConstruct(T& component, EntityId entityId)
    : component(component), entityId(entityId)
{
}

template<typename T>
EventComponentRemove<T>::EventComponentRemove(T& component, EntityId entityId)
    : component(component), entityId(entityId)
{
}

template<typename T, typename... Args>
void EntityManager::createComponent(EntityId id, Args&&... args)
{
    assert(m_Entities.find(id) != m_Entities.end());

    const ComponentTypeId componentTypeId = ComponentHolder<T>::getTypeId();
    assert(m_Components[componentTypeId].find(id) == m_Components[componentTypeId].end());

    ComponentHolder<T>* componentHolder     = new ComponentHolder<T>(std::forward<Args>(args)...);
    IComponentHolder*   baseComponentHolder = componentHolder;

    m_Entities[id].emplace(componentTypeId, baseComponentHolder);
    m_Components[componentTypeId].emplace(id, baseComponentHolder);

    m_EventDispatcher.getSink<EventComponentConstruct<T>>().fireEvent(
        EventComponentConstruct<T>(componentHolder->get(), id));
}

template<typename T>
void EntityManager::removeComponent(EntityId id)
{
    assert(m_Entities.find(id) != m_Entities.end());

    const ComponentTypeId componentTypeId = ComponentHolder<T>::GetTypeId();
    assert(m_Components[componentTypeId].find(id) != m_Components[componentTypeId].end());

    IComponentHolder* holderToRemove = m_Entities[id].find(componentTypeId)->second;

    m_EventDispatcher.getSink<EventComponentRemove<T>>().fireEvent(
        EventComponentRemove<T>(dynamic_cast<ComponentHolder<T>*>(holderToRemove)->get(), id));

    m_Components[componentTypeId].erase(id);
    m_Entities[id].erase(componentTypeId);

    delete holderToRemove;
}

template<typename T>
T& EntityManager::getComponent(EntityId id)
{
    assert(m_Entities.find(id) != m_Entities.end());

    const ComponentTypeId componentTypeId = ComponentHolder<T>::getTypeId();

    auto holderIterator = m_Components[componentTypeId].find(id);
    assert(holderIterator != m_Components[componentTypeId].end());

    return reinterpret_cast<ComponentHolder<T>*>(holderIterator->second)->get();
}

template<typename T>
bool EntityManager::hasComponent(EntityId id)
{
    assert(m_Entities.find(id) != m_Entities.end());

    const ComponentTypeId componentTypeId = ComponentHolder<T>::getTypeId();
    return m_Components[componentTypeId].find(id) != m_Components[componentTypeId].end();
}

template<typename T>
EntityMap& EntityManager::getEntityMap()
{
    const ComponentTypeId componentTypeId = ComponentHolder<T>::getTypeId();
    return m_Components[componentTypeId];
}

template<typename T>
EventSink<EventComponentConstruct<T>>& EntityManager::onConstruct()
{
    return m_EventDispatcher.getSink<EventComponentConstruct<T>>();
}

} // namespace gwars