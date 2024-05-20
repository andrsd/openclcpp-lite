#include "openclcpp-lite/event.h"
#include "openclcpp-lite/error.h"
#include "openclcpp-lite/queue.h"
#include "openclcpp-lite/context.h"

namespace openclcpp_lite {

Event::Event() {}

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
    if (status > 0)
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
wait_for_events(const std::vector<Event> & event)
{
    std::vector<cl_event> evts;
    evts.reserve(event.size());
    for (auto & e : event)
        evts.emplace_back(e);
    OPENCL_CHECK(clWaitForEvents(evts.size(), evts.data()));
}

} // namespace openclcpp_lite
