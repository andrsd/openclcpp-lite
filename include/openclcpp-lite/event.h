// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/templ.h"
#include "openclcpp-lite/enums.h"

namespace openclcpp_lite {

class Queue;
class Context;

/// OpenCL event
class Event {
public:
    struct ProfilingInfo {
        /// Value that describes the current device time counter in nanoseconds when the command
        /// was enqueued in a command-queue by the host.
        uint64_t queued;
        /// Value that describes the current device time counter in nanoseconds when the command
        /// that has been enqueued is submitted by the host to the device associated with the
        /// command-queue.
        uint64_t submit;
        /// Value that describes the current device time counter in nanoseconds when the command
        /// starts execution on the device.
        uint64_t start;
        /// Value that describes the current device time counter in nanoseconds when the command
        /// has finished execution on the device.
        uint64_t end;
    };

    /// Create an event from OpenCL event
    explicit Event(cl_event evt);

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

    /// Returns profiling information for the command associated with event if profiling is enabled.
    ProfilingInfo profiling_info() const;

    operator cl_event() const;

private:
    template <typename T>
    T
    get_info(cl_event_info name) const
    {
        T val;
        get_info_helper(clGetEventInfo, this->evt, name, val);
        return val;
    }

    template <typename T>
    T
    get_profiling_info(cl_profiling_info name) const
    {
        T val;
        get_info_helper(clGetEventProfilingInfo, this->evt, name, val);
        return val;
    }

    /// Underlying OpenCL event
    cl_event evt;
};

/// Waits on the host thread for commands identified by event object to complete.
void wait_for_event(const Event & event);

/// Waits on the host thread for commands identified by event objects to complete.
void wait_for_events(const std::vector<Event> & event);

} // namespace openclcpp_lite
