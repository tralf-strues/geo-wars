/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file event_sink.hpp
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

#include "events/event.hpp"

namespace gwars {

struct StaticEventHandlerDelegateIdGenerator
{
    static uint64_t nextId();
};

template<auto T>
struct StaticEventHandlerDelegateId
{
public:
    static uint64_t getId();

private:
    static uint64_t s_Id;
};

class IEventSink
{
public:
    virtual ~IEventSink() = default;
};

template<typename T>
class EventSink : public IEventSink
{
public:
    using EventHandlerT = void(const T&);

    /**
     * @warning Currently adding several same handlers (with the same method and instance)
     * causes undefined behavior.
     *
     * @tparam HandlerMethodT
     * @tparam HandlerClassT
     * @param handler
     */
    template<auto HandlerMethodT, class HandlerClassT>
    void addHandler(HandlerClassT& handler);

    /**
     * @warning Currently adding several same handlers causes undefined behavior.
     *
     * @tparam HandlerMethodT
     * @tparam HandlerClassT
     * @param handler
     */
    template<auto HandlerFunctionT>
    void addHandler();

    template<auto HandlerMethodT, class HandlerClassT>
    void removeHandler(HandlerClassT& handler);

    template<auto HandlerFunctionT>
    void removeHandler();

    void fireEvent(const T& event);

private:
    struct HandlerDelegate
    {
        std::function<EventHandlerT> function;
        uint64_t                     functionId{0};
        void*                        handler{nullptr};

        template<auto HandlerMethodT, class HandlerClassT>
        static HandlerDelegate wrapMethod(HandlerClassT& handler);

        template<auto HandlerFunctionT>
        static HandlerDelegate wrapFunction();

        bool operator==(const HandlerDelegate& delegate) const;
    };

private:
    std::vector<HandlerDelegate> m_Handlers;
};

} // namespace gwars

#include "events/event_sink.ipp"