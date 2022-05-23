/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file event_sink.ipp
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

namespace gwars {

template<auto T>
uint64_t StaticEventHandlerDelegateId<T>::s_Id{StaticEventHandlerDelegateIdGenerator::nextId()};

template<auto T>
uint64_t StaticEventHandlerDelegateId<T>::getId()
{
    return s_Id;
}

template<typename T>
template<auto HandlerMethodT, class HandlerClassT>
typename EventSink<T>::HandlerDelegate EventSink<T>::HandlerDelegate::wrapMethod(HandlerClassT& handler)
{
    return {std::bind(HandlerMethodT, &handler, std::placeholders::_1),
            StaticEventHandlerDelegateId<HandlerMethodT>::getId(),
            reinterpret_cast<void*>(&handler)};
}

template<typename T>
template<auto HandlerFunctionT>
typename EventSink<T>::HandlerDelegate EventSink<T>::HandlerDelegate::wrapFunction()
{
    return {std::bind(HandlerFunctionT, std::placeholders::_1),
            StaticEventHandlerDelegateId<HandlerFunctionT>::getId(),
            nullptr};
}

template<typename T>
bool EventSink<T>::HandlerDelegate::operator==(const HandlerDelegate& delegate) const
{
    return functionId == delegate.functionId && handler == delegate.handler;
}

template<typename T>
template<auto HandlerMethodT, class HandlerClassT>
void EventSink<T>::addHandler(HandlerClassT& handler)
{
    m_Handlers.emplace_back(HandlerDelegate::template wrapMethod<HandlerMethodT>(handler));
}

template<typename T>
template<auto HandlerFunctionT>
void EventSink<T>::addHandler()
{
    m_Handlers.emplace_back(EventSink<T>::HandlerDelegate::template wrapFunction<HandlerFunctionT>());
}

template<typename T>
template<auto HandlerMethodT, class HandlerClassT>
void EventSink<T>::removeHandler(HandlerClassT& handler)
{
    HandlerDelegate delegate{EventSink<T>::HandlerDelegate::template wrapMethod<HandlerMethodT>(handler)};

    for (auto it = m_Handlers.begin(); it != m_Handlers.end(); ++it)
    {
        if (delegate == *it)
        {
            m_Handlers.erase(it);
            break;
        }
    }
}

template<typename T>
template<auto HandlerFunctionT>
void EventSink<T>::removeHandler()
{
    HandlerDelegate delegate{EventSink<T>::HandlerDelegate::template wrapFunction<HandlerFunctionT>()};

    for (auto it = m_Handlers.begin(); it != m_Handlers.end(); ++it)
    {
        if (delegate == *it)
        {
            m_Handlers.erase(it);
            break;
        }
    }
}

template<typename T>
void EventSink<T>::fireEvent(const T& event)
{
    for (auto handler : m_Handlers)
    {
        handler.function(event);
    }
}

} // namespace gwars