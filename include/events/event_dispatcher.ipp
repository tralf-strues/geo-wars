/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file event_dispatcher.ipp
 * @date 2022-05-23
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

#include <cassert>

namespace gwars {

template<typename T>
EventSink<T>& EventDispatcher::getSink()
{
    const EventType type = StaticEventTypeHolder<T>::getType();

    IEventSink* sink = nullptr;
    if (m_Sinks.find(type) == m_Sinks.end())
    {
        m_Sinks[type] = new EventSink<T>();
    }

    sink = m_Sinks[type];

    assert(sink != nullptr);
    return dynamic_cast<EventSink<T>&>(*sink);
}

template<typename T, typename... Args>
void EventDispatcher::fireEvent(Args&&... args)
{
    T event(std::forward<Args>(args)...);
    getSink<T>().fireEvent(event);
}

} // namespace gwars