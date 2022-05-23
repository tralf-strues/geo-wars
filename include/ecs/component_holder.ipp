/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file component_holder.ipp
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

#include <utility>

namespace gwars {

extern uint32_t s_NextComponentTypeId;

template<typename T>
ComponentTypeId ComponentHolder<T>::s_TypeId{s_NextComponentTypeId++};

template<typename T>
template<typename... Args>
ComponentHolder<T>::ComponentHolder(Args&&... args) : m_Component(std::forward<Args>(args)...)
{
}

template<typename T>
T& ComponentHolder<T>::get()
{
    return m_Component;
}

template<typename T>
ComponentTypeId ComponentHolder<T>::getTypeId()
{
    return s_TypeId;
}

} // namespace gwars