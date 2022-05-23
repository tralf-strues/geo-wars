/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file entity_manager.cpp
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

#include <cassert>
#include "ecs/entity_manager.hpp"

using namespace gwars;

EntityId EntityManager::createEntity()
{
    EntityId id = m_NextEntityId++;
    m_Entities.emplace(id, ComponentMap{});
    return id;
}

void EntityManager::removeEntity(EntityId id)
{
    assert(m_Entities.find(id) != m_Entities.end());

    for (auto [componentTypeId, holder] : m_Entities[id])
    {
        // IComponentHolder* holderToRemove = &entityMap[id];
        m_Components[componentTypeId].erase(id);
        delete holder;
    }

    m_Entities.erase(id);
}

void EntityManager::clear()
{
    for (auto [entityId, componentMap] : m_Entities)
    {
        removeEntity(entityId);
    }
}