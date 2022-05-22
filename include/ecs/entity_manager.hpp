/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file entity_manager.hpp
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

#include "ecs/component_holder.hpp"
#include <unordered_map>

namespace gwars {

using EntityId                       = uint32_t;
constexpr EntityId INVALID_ENTITY_ID = 0;

using EntityMap    = std::unordered_map<EntityId, IComponentHolder*>;
using ComponentMap = std::unordered_map<ComponentTypeId, IComponentHolder*>;

class EntityManager
{
public:
    EntityId createEntity();
    void     removeEntity(EntityId id);

    void clear();

    template<typename T, typename... Args>
    void createComponent(EntityId id, Args&&... args);

    template<typename T>
    void removeComponent(EntityId id);

    template<typename T>
    T& getComponent(EntityId id);

    template<typename T>
    bool hasComponent(EntityId id);

    template<typename T>
    EntityMap& getEntityMap();

private:
    std::unordered_map<EntityId, ComponentMap>     m_Entities;
    std::unordered_map<ComponentTypeId, EntityMap> m_Components;
    uint32_t                                       m_NextEntityId{1};
};

} // namespace gwars

#include "ecs/entity_manager.ipp"