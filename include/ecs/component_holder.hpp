/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file component_holder.hpp
 * @date 2022-05-21
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

#include <inttypes.h>

namespace gwars {

using ComponentTypeId                          = uint64_t;
constexpr ComponentTypeId INVALID_COMPONENT_ID = 0;

class IComponentHolder
{
public:
    virtual ~IComponentHolder() = default;
};

template<typename T>
class ComponentHolder : public IComponentHolder
{
public:
    template<typename... Args>
    ComponentHolder(Args&&... args);

    ~ComponentHolder() override = default;

    T& get();
    static ComponentTypeId getTypeId();

protected:
    static ComponentTypeId s_TypeId;
    T m_Component;
};

} // namespace gwars

#include "ecs/component_holder.ipp"