#pragma once

#include "enable_polymorphic_dispatch.h"
#include "detail/typelist.h"
#include <memory>
#include "detail/leftright.h"
#include <typeindex>
#include <unordered_map>
#include <utility>

//! The mpm namespace
namespace mpm {
//! \defgroup Concepts
//! Concept is a term that describes a named set of requirements for a type

//! \defgroup Event
//! \ingroup Concepts
//! \{
//!
//! An instance of any C++ object type. That is, anything but a function,
//! reference, or void type
//!
//! \par Requirements
//! Given:\n
//! E, an implementation of the Event concept
//!
//! |Expression                       | Requirements             | Return type        |
//! |:--------------------------------|:-------------------------|:-------------------|
//! |std::is_object<E>::value == true | E must be an object type | bool, must be true |
//! \}

//! \defgroup EventHandler
//! \ingroup Concepts
//! \{
//!
//! A Callable that can be invoked to handle an instance of Event.
//! Callable's INVOKE operation must be noexcept.
//!
//! \par Extends
//! Callable
//!
//! \par Requirements
//! Given:\n
//! E an implementation of the Event concept,
//! e and instance of E,
//! H an implementation of the EventHandler concept handling events of type E,
//! h an instance of H
//!
//! |Expression                      | Requirements           |
//! |:-------------------------------|:-----------------------|
//! |h(e)                            | well-formed            |
//! |noexcept(h(e)) == true          | h(e) must be noexcept  |
//! \}

namespace detail {
//! Adapts an instance of the Event concept into the
//! class hierarchy rooted at mpm::detail::event
template<typename Event>
class adapted_event : public detail::event {
public:
    using dispatch_as = detail::typelist<Event>;

    adapted_event(const Event &event) : m_event{event} {}

    operator const Event &() const { return m_event; }

private:
    const Event &m_event;
};

//! Holds the subscriber event type and the handler instance
//! in a type-erased manner so that they can be put into a
//! homogeneous container (e.g. the std::unordered_multimap as below)
class subscription {
public:
    using id_t = std::intptr_t;

    template<typename E, typename H, typename Alloc>
    subscription(const H &handler, const Alloc &alloc, E *)
        : m_self{std::allocate_shared<model<E, H>>(alloc, handler)}
    {}

    void deliver(const event &e) { m_self->deliver(e); }

    id_t id() const { return reinterpret_cast<std::intptr_t>(m_self.get()); }

private:
    struct concept
    {
        virtual ~concept(){};
        virtual void deliver(const event &e) = 0;
    };

    // base template for events that extend detail::event (i.e. for
    // polymorphic dispatchable events). These can be safely
    // static_cast to the subscribed event type
    template<typename E, typename H, typename Enable = void>
    struct model : concept {
        explicit model(H h) : handler(std::move(h)) {}

        void deliver(const event &e) final
        {
            handler(static_cast<const E &>(e));
        }

        H handler;
    };

    // Specialization for events that do not use
    // enable_polymorphic_dispatch. The diffence is that we static_cast
    // to adapted_event<E>.
    template<typename E, typename H>
    struct model<E,
                 H,
                 typename std::enable_if<
                     !std::is_base_of<detail::event, E>::value>::type>
        : concept {
        explicit model(H h) : handler(std::move(h)) {}

        void deliver(const event &e) final
        {
            handler(static_cast<const adapted_event<E> &>(e));
        }

        H handler;
    };

    std::shared_ptr<concept> m_self;
};

struct cookie {
    cookie() = default;

    cookie(subscription::id_t _id, std::type_index _ti) : id(_id), ti(_ti) {}

    subscription::id_t id = 0;
    std::type_index ti = typeid(std::nullptr_t);
};

//! Extract a dispatch typelist for an event supplying a dispatch_as
//! typedef. If the supplied event type is not in the head position of
//! the dispatch_as typelist then it is prepended to said list
template<typename E>
struct dispatch_typelist {
    // todo C++14 can be std::conditional_t
    // most derived type goes first
    using type = typename std::conditional<
        std::is_same<E, typename E::dispatch_as::head>::value,
        typename E::dispatch_as,
        detail::typelist<E, typename E::dispatch_as>>::type;
};

template<typename SubsMap>
struct deliver {
    deliver(const event &e, const SubsMap &subs) : m_subs{subs}, m_event{e} {}

    template<typename T>
    void operator()()
    {
        auto handlers = m_subs.equal_range(std::type_index(typeid(T)));
        for (auto pos = handlers.first; pos != handlers.second; ++pos) {
            const_cast<subscription &>(pos->second).deliver(m_event);
        }
    }

    const SubsMap &m_subs;
    const event &m_event;
};

// not meant to be used as a virtual base class - nothing should
// destruct via this type
class unsubscribable {
public:
    virtual void unsubscribe(const cookie &c) = 0;

protected:
    ~unsubscribable() {}
};
}// namespace detail

//! A small POD type representing a subscription
using cookie = detail::cookie;

//! Accepts events from publishers and delivers them to subscribers
template<typename Allocator>
class basic_eventbus : public detail::unsubscribable {
public:
    //! The type of the Allocator used by this eventbus
    using allocator_type = Allocator;

    //! Delegates to basic_eventbus(allocator_type) using
    //! default construction for the allocator_type instance.
    basic_eventbus();

    //! Constructs an instance using the supplied allocator_type.
    explicit basic_eventbus(allocator_type alloc);

#ifdef DOCS

    //! Publish an instance of E.
    //!
    //! The event instance will be delivered either as only type E or
    //! if E has been defined with mpm::enable_polymorphic_dispatch it
    //! will be delivered as every type in its inheritance chain.
    //!
    //! \tparam E An instance of the Event concept
    //! \param event The event to publish
    //! \returns void
    template<typename E>
    void publish(const E &event) noexcept;

#else

    template<typename E>
    typename std::enable_if<std::is_base_of<detail::event, E>::value>::type
    publish(const E &event) noexcept;

    template<typename E>
    typename std::enable_if<!std::is_base_of<detail::event, E>::value>::type
    publish(const E &event) noexcept;

#endif

    //! Subscribe to instances of Event
    //!
    //! The supplied handler will be invoked when events of type Event
    //! are published or when classes derived from Event that have chosen
    //! to enable polymorphic dispatch are published.
    //!
    //! \tparam Event The type for which to subscribe
    //! \tparam EventHandler an instance of the EventHandler concept
    //! \param handler An instance of EventHandler
    //! \returns A cookie which will allow for this handler to be
    //!         unsubscribed later via basic_eventbus::unsubscribe
    template<typename Event, typename EventHandler>
    cookie subscribe(const EventHandler &handler);

    //! Unsubscribes an event handler
    //!
    //! \param c A cookie obtained when basic_eventbus::subscribe was called
    //! \returns void
    void unsubscribe(const cookie &c) override;

private:
    using subscriptions =
        leftright<std::unordered_multimap<std::type_index,
                                          detail::subscription,
                                          std::hash<std::type_index>,
                                          std::equal_to<std::type_index>,
                                          allocator_type>>;
    allocator_type m_alloc;
    subscriptions m_subscriptions;
};

//! Default eventbus uses std::allocator<char>
using eventbus = basic_eventbus<std::allocator<char>>;

template<typename A>
basic_eventbus<A>::basic_eventbus() : basic_eventbus(allocator_type())
{}

template<typename A>
basic_eventbus<A>::basic_eventbus(allocator_type alloc)
    : m_alloc{std::move(alloc)},
      m_subscriptions{in_place, alloc}
{}

#ifndef DOCS

template<typename A>
template<typename Event>
typename std::enable_if<std::is_base_of<detail::event, Event>::value>::type
basic_eventbus<A>::publish(const Event &event) noexcept
{
    using types = typename detail::dispatch_typelist<Event>::type;
    m_subscriptions.observe([&](typename subscriptions::const_reference subs) {
        detail::for_each_type<types>(
            detail::deliver<decltype(subs)>{event, subs});
    });
}

template<typename A>
template<typename Event>
typename std::enable_if<!std::is_base_of<detail::event, Event>::value>::type
basic_eventbus<A>::publish(const Event &event) noexcept
{
    publish(detail::adapted_event<Event>{event});
}

#endif

template<typename A>
template<typename Event, typename EventHandler>
cookie
basic_eventbus<A>::subscribe(const EventHandler &handler)
{
    static_assert(std::is_object<Event>::value, "Events must be object types");
    static_assert(noexcept(handler(std::declval<const Event &>())),
                  "Need noexcept handler for Event");

    Event *ptr{};
    detail::subscription sub{handler, m_alloc, ptr};
    return m_subscriptions.modify(
        [&](typename subscriptions::reference subs) noexcept {
            auto idx = std::type_index(typeid(Event));
            subs.emplace(idx, sub);
            return cookie{sub.id(), idx};
        });
}

template<typename A>
void
basic_eventbus<A>::unsubscribe(const cookie &c)
{
    m_subscriptions.modify(
        [=](typename subscriptions::reference subs) noexcept {
            auto range = subs.equal_range(c.ti);
            for (auto pos = range.first; pos != range.second; ++pos) {
                if (pos->second.id() == c.id) {
                    subs.erase(pos);
                    return;
                }
            }
        });
}

//! An RAII type for eventbus subscriptions
//!
//! This type will ensure that the encapsulated eventbus subscription
//! is released when it goes out of scope.
//!
//! \tparam Event the type of event for which this subscription subscribes
template<typename Event>
class scoped_subscription {
public:
    using event_type = Event;

    //! Construct a scoped_subscription not managing any subscription
    scoped_subscription() = default;

    //! Move from another scoped_subscription.
    scoped_subscription(scoped_subscription &&other) noexcept
        : m_ebus{other.m_ebus},
          m_cookie{other.m_cookie}
    {
        other.m_ebus = nullptr;
    }

    //! Copying a scoped_subscription is prohibited
    scoped_subscription(const scoped_subscription &) = delete;

    //! Move-assign from another scoped_subscription
    scoped_subscription &operator=(scoped_subscription &&other) noexcept
    {
        reset();
        m_ebus = other.m_ebus;
        m_cookie = other.m_cookie;
        other.m_ebus = nullptr;
    }

    void swap(scoped_subscription &other) noexcept
    {
        using std::swap;
        swap(m_ebus, other.m_ebus);
        swap(m_cookie, other.m_cookie);
    }

    //! Initializes a subscription that will be usubscribed when
    //! this object goes out of scope.
    //!
    //! Subscription is constructed with ebus.subscribe<Event>(h)
    //!
    //! \tparam Alloc The allocator type of the event bus supplied
    //! \tparam Handler the type of the handler supplied
    //! \param ebus An instance of basic_eventbus to use for subscribing
    //! \param h An instance of an event handler
    template<typename Alloc, typename Handler>
    scoped_subscription(basic_eventbus<Alloc> &ebus, const Handler &h)
        : m_ebus{&ebus},
          m_cookie{ebus.template subscribe<event_type>(h)}
    {}

    //! Initializes a subscription that will be usubscribed when
    //! this object goes out of scope.
    //!
    //! Subscription is constructed with ebus.subscribe<Event>(h). If
    //! this object currently manages a subscription, that subscription
    //! will be cleared as though reset() had been called.
    //!
    //! \tparam Alloc The allocator type of the event bus supplied
    //! \tparam Handler the type of the handler supplied
    //! \param ebus An instance of basic_eventbus to use for subscribing
    //! \param h An instance of an event handler
    template<typename Alloc, typename Handler>
    void assign(basic_eventbus<Alloc> &ebus, const Handler &h)
    {
        reset();
        m_ebus = &ebus;
        m_cookie = ebus.template subscribe<event_type>(h);
    }

    //! Unsubscribes this objects managed subscription if one exists
    ~scoped_subscription() { reset(); }

    //! Unsubscribes this objects managed subscription if one exists
    //! \returns void
    void reset()
    {
        if (m_ebus) { m_ebus->unsubscribe(m_cookie); }
    }

private:
    friend void swap(scoped_subscription &lhs, scoped_subscription &rhs)
    {
        lhs.swap(rhs);
    }

    detail::unsubscribable *m_ebus = nullptr;//non-owning
    cookie m_cookie{};
};
}// namespace mpm

namespace ulib {
template<typename T, typename Base = mpm::detail::event>
using enable_polymorphic_dispatch = mpm::enable_polymorphic_dispatch<T, Base>;

// using eventbus = mpm::basic_eventbus<std::allocator<char>>;
using eventbus = mpm::basic_eventbus<std::allocator<
    std::pair<const std::type_index, mpm::detail::subscription>>>;

template<typename EventType>
using scoped_subscription = mpm::scoped_subscription<EventType>;
}// namespace ulib
