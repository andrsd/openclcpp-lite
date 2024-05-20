#pragma once

#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/templ.h"
#include "openclcpp-lite/error.h"

namespace openclcpp_lite {

class Context;
class Device;
class Buffer;
class Kernel;

class Queue {
public:
    Queue(const Context & context, bool enable_profiling = false);

    /// Increments the command_queue reference count
    void retain() const;

    /// Decrements the command_queue reference count.
    void release() const;

    /// Return the context specified when the command-queue is created.
    Context context() const;

    /// Return the device specified when the command-queue is created.
    Device device() const;

    /// Return the command-queue reference count.
    ///
    /// The reference count returned should be considered immediately stale. It is unsuitable for
    /// general use in applications. This feature is provided for identifying memory leaks.
    unsigned int reference_count() const;

    /// Enqueue commands to read from a buffer object to host memory.
    ///
    /// @param buffer Buffer to read from
    /// @param blocking Indicates if the read operations are blocking or non-blocking.
    /// @param offset The offset in bytes in the buffer object to read from.
    /// @param size The size in bytes of data being read.
    /// @param ptr The pointer to buffer in host memory where data is to be read into.
    void enqueue_read_buffer(const Buffer & buffer,
                             bool blocking,
                             size_t offset,
                             size_t size,
                             void * ptr) const;

    /// Enqueue commands to write to a buffer object from host memory.
    ///
    /// @param buffer Buffer to write into
    /// @param blocking Indicates if the write operations are blocking or nonblocking.
    /// @param offset The offset in bytes in the buffer object to write to.
    /// @param size The size in bytes of data being written.
    /// @param ptr The pointer to buffer in host memory where data is to be written from.
    void enqueue_write_buffer(const Buffer & buffer,
                              bool blocking,
                              size_t offset,
                              size_t size,
                              const void * ptr) const;

    /// Enqueues a command to copy from one buffer object to another.
    ///
    /// @param src Source buffer
    /// @param dest Destination buffer
    /// @param src_offset The offset where to begin copying data from `src`.
    /// @param dest_offset The offset where to begin copying data into `dst`.
    /// @param size Refers to the size in bytes to copy.
    void enqueue_copy_buffer(const Buffer & src,
                             const Buffer & dest,
                             size_t src_offset,
                             size_t dest_offset,
                             size_t size);

    template <typename... ARGS>
    void enqueue_kernel(const Kernel & kernel);

    /// Issues all previously queued OpenCL commands in a command-queue to the device associated
    /// with the command-queue.
    void flush() const;

    /// Blocks until all previously queued OpenCL commands in a command-queue are issued to the
    /// associated device and have completed.
    void finish() const;

private:
    template <typename T>
    T
    get_info(cl_command_queue_info name) const
    {
        T val;
        get_info_helper(clGetCommandQueueInfo, this->q, name, val);
        return val;
    }

    cl_command_queue q;
};

} // namespace openclcpp_lite
