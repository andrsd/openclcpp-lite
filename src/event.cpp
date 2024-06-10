// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#include "openclcpp-lite/event.h"
#include "openclcpp-lite/error.h"
#include "openclcpp-lite/queue.h"
#include "openclcpp-lite/context.h"

namespace openclcpp_lite {

Event::Event(cl_event evt) : evt(evt) {}

void
Event::retain() const
{
    OPENCL_CHECK(clRetainEvent(this->evt));
}

void
Event::release() const
{
    OPENCL_CHECK(clReleaseEvent(this->evt));
}

Queue
Event::command_queue() const
{
    auto q = get_info<cl_command_queue>(CL_EVENT_COMMAND_QUEUE);
    return Queue(q);
}

Context
Event::context() const
{
    auto ctx = get_info<cl_context>(CL_EVENT_CONTEXT);
    return Context(ctx);
}

CommandExecutionStatus
Event::command_execution_status() const
{
    auto status = get_info<cl_int>(CL_EVENT_COMMAND_EXECUTION_STATUS);
    if (status >= 0)
        return static_cast<CommandExecutionStatus>(status);
    else
        return ERROR;
}

unsigned int
Event::reference_count() const
{
    return get_info<cl_uint>(CL_EVENT_REFERENCE_COUNT);
}

void
Event::wait() const
{
    OPENCL_CHECK(clWaitForEvents(1, &this->evt));
}

Event::ProfilingInfo
Event::profiling_info() const
{
    ProfilingInfo info;
    info.queued = get_profiling_info<cl_ulong>(CL_PROFILING_COMMAND_QUEUED);
    info.submit = get_profiling_info<cl_ulong>(CL_PROFILING_COMMAND_SUBMIT);
    info.start = get_profiling_info<cl_ulong>(CL_PROFILING_COMMAND_START);
    info.end = get_profiling_info<cl_ulong>(CL_PROFILING_COMMAND_END);
    return info;
}

Event::operator cl_event() const
{
    return this->evt;
}

void
wait_for_event(const Event & event)
{
    event.wait();
}

void
wait_for_events(const std::vector<Event> & events)
{
    OPENCL_CHECK(
        clWaitForEvents(events.size(), events.empty() ? nullptr : (cl_event *) &events.front()));
}

} // namespace openclcpp_lite
