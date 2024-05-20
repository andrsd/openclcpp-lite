#pragma once

#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/templ.h"
#include "openclcpp-lite/enums.h"

namespace openclcpp_lite {

class Queue;
class Context;

class Event {
public:
    Event();

    /// Increment the context reference count.
    void retain() const;

    /// Decrement the context reference count.
    void release() const;

    /// Return the command-queue associated with event. For user event objects, a NULL value is
    /// returned.
    Queue command_queue() const;

    /// Return the context associated with event.
    Context context() const;

    /// Return the execution status of the command identified by event.
    CommandExecutionStatus command_execution_status() const;

    /// Return the event reference count. The reference count returned should be considered
    /// immediately stale. It is unsuitable for general use in applications. This feature is
    /// provided for identifying memory leaks.
    unsigned int reference_count() const;

    /// Waits on the host thread for commands identified by event object to complete.
    void wait() const;

    operator cl_event() const;

private:
    Event(cl_event evt);

    template <typename T>
    T
    get_info(cl_event_info name) const
    {
        T val;
        get_info_helper(clGetEventInfo, this->evt, name, val);
        return val;
    }

    cl_event evt;
};

/// Waits on the host thread for commands identified by event object to complete.
void wait_for_event(const Event & event);

/// Waits on the host thread for commands identified by event objects to complete.
void wait_for_events(const std::vector<Event> & event);

} // namespace openclcpp_lite
