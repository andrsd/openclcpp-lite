// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/templ.h"
#include "openclcpp-lite/enums.h"
#include "openclcpp-lite/error.h"
#include "openclcpp-lite/event.h"
#include <mutex>

namespace openclcpp_lite {

class Context;
class Device;
class Kernel;
template <int N>
class Range;
class Memory;
template <typename T, int D>
class Buffer;

/// OpenCL command queue
class Queue {
public:
    /// Create a null queue
    Queue();

    /// Create a queue from a OpenCL command queue
    explicit Queue(cl_command_queue q);

    /// Create a queue in a Context
    ///
    /// @param context OpenCL context of the queue
    /// @param enable_profiling Enable profiling
    explicit Queue(const Context & context, bool enable_profiling = false);

    /// Create a queue in a Context
    ///
    /// @param context OpenCL context of the queue
    /// @param device OpenCL device of the queue
    /// @param enable_profiling Enable profiling
    Queue(const Context & context, const Device & device, bool enable_profiling = false);

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

    /// Enqueue commands to read from a buffer object to host memory in blocking mode
    ///
    /// @param buffer Buffer to read from
    /// @param ptr The pointer to buffer in host memory where data is to be read into.
    /// @param wait_list Specify events that need to complete before this particular command can be
    ///        executed
    template <typename T, int D>
    void
    enqueue_read(const Buffer<T, D> & buffer,
                 const Range<D> & range,
                 void * ptr,
                 const std::vector<Event> & wait_list = std::vector<Event>()) const
    {
        enqueue_read_raw(buffer, 0 * sizeof(T), range.size() * sizeof(T), ptr, wait_list);
    }

    /// Enqueue commands to read from a buffer object to host memory in non-blocking mode
    ///
    /// @param buffer Buffer to read from
    /// @param ptr The pointer to buffer in host memory where data is to be read into.
    /// @param wait_list Specify events that need to complete before this particular command can be
    ///        executed
    /// @return Event object that identifies this particular kernel execution instance
    template <typename T, int D>
    Event
    enqueue_iread(const Buffer<T, 1> & buffer,
                  const Range<D> & range,
                  void * ptr,
                  const std::vector<Event> & wait_list = std::vector<Event>()) const
    {
        return enqueue_iread_raw(buffer, 0 * sizeof(T), range.size() * sizeof(T), ptr, wait_list);
    }

    /// Enqueue commands to write to a buffer object from host memory in blocking mode
    ///
    /// @param buffer Buffer to write into
    /// @param ptr The pointer to buffer in host memory where data is to be written from.
    /// @param wait_list Specify events that need to complete before this particular command can be
    ///        executed
    template <typename T, int D>
    void
    enqueue_write(const Buffer<T, D> & buffer,
                  const Range<D> & range,
                  const void * ptr,
                  const std::vector<Event> & wait_list = std::vector<Event>()) const
    {
        enqueue_write_raw(buffer, 0 * sizeof(T), range.size() * sizeof(T), ptr, wait_list);
    }

    /// Enqueue commands to write to a buffer object from host memory in non-blocking mode
    ///
    /// @param buffer Buffer to write into
    /// @param ptr The pointer to buffer in host memory where data is to be written from.
    /// @param wait_list Specify events that need to complete before this particular command can be
    ///        executed
    template <typename T, int D>
    Event
    enqueue_iwrite(const Buffer<T, D> & buffer,
                   const Range<D> & range,
                   const void * ptr,
                   const std::vector<Event> & wait_list = std::vector<Event>()) const
    {
        return enqueue_iwrite_raw(buffer, 0 * sizeof(T), range.size() * sizeof(T), ptr, wait_list);
    }

    /// Enqueues a command to copy from one buffer object to another.
    ///
    /// @param src Source buffer
    /// @param dest Destination buffer
    /// @param range Range.
    /// @param wait_list Specify events that need to complete before this particular command can be
    ///        executed
    /// @return Event object that identifies this particular operation
    template <typename T, int D>
    Event
    enqueue_copy(const Buffer<T, D> & src,
                 const Buffer<T, D> & dest,
                 const Range<D> & range,
                 const std::vector<Event> & wait_list = std::vector<Event>()) const
    {
        assert(src.byte_size() == dest.byte_size());
        return enqueue_copy_raw(src,
                                dest,
                                0 * sizeof(T),
                                0 * sizeof(T),
                                range.size() * sizeof(T),
                                wait_list);
    }

    /// Enqueues a command to map a region of the buffer object given by `buffer` into the host
    /// address space and returns a pointer to this mapped region.
    ///
    /// @param buffer Buffer to map
    /// @param blocking Indicates if the map operation is blocking or non-blocking.
    /// @param flags A bit-field with the following supported values:
    ///   - `READ` This flag specifies that the region being mapped in the memory object is being
    ///      mapped for reading.
    ///   - `WRITE` This flag specifies that the region being mapped in the memory object is being
    ///      mapped for writing.
    ///   - `WRITE_INVALIDATE_REGION` This flag specifies that the region being mapped in the memory
    ///      object is being mapped for writing.
    ///
    ///   `READ` or `WRITE` and `WRITE_INVALIDATE_REGION` are mutually exclusive.
    /// @param range The range
    /// @return The returned pointer maps a region starting at `offset` and is at least `size` bytes
    ///         in size. The result of a memory access outside this region is undefined.
    template <typename T, int D>
    T *
    enqueue_map_buffer(const Buffer<T, D> & buffer,
                       bool blocking,
                       MapFlags flags,
                       const Range<D> & range) const
    {
        return static_cast<T *>(enqueue_map_buffer_raw(buffer,
                                                       blocking,
                                                       flags,
                                                       0 * sizeof(T),
                                                       range.size() * sizeof(T)));
    }

    /// Enqueues a command to unmap a previously mapped region of a memory object
    ///
    /// @param mem Memory object to unmap
    /// @param mapped_ptr Mapped memory
    /// @return Event object that identifies this particular operation
    Event enqueue_unmap_mem_object(const Memory & mem, void * mapped_ptr) const;

    /// A synchronization point that enqueues a barrier operation.
    ///
    /// @param wait_list Specify events that need to complete before
    /// @return Event object the identified this barrier
    Event enqueue_barrier(const std::vector<Event> & wait_list = std::vector<Event>()) const;

    /// Enqueues a marker command which waits for either a list of events to complete, or all
    /// previously enqueued commands to complete.
    ///
    /// @param wait_list Events that need to complete before this particular command can be
    ///        executed.
    /// @return Event object that identifies this particular marker
    Event enqueue_marker(const std::vector<Event> & wait_list = std::vector<Event>()) const;

    /// Enqueues a command to execute a kernel on a device.
    ///
    /// @param kernel Kernel to execute
    /// @param global Global range
    /// @param wait_list Specify events that need to complete before this particular command can be
    ///        executed
    /// @return Event object that identifies this particular kernel execution instance
    template <int N>
    Event
    enqueue_kernel(const Kernel & kernel,
                   const Range<N> & global,
                   const std::vector<Event> & wait_list = std::vector<Event>()) const
    {
        cl_event evt;
        OPENCL_CHECK(
            clEnqueueNDRangeKernel(this->q,
                                   kernel,
                                   global.dimensions(),
                                   nullptr,
                                   global,
                                   nullptr,
                                   wait_list.size(),
                                   wait_list.empty() ? nullptr : (cl_event *) &wait_list.front(),
                                   &evt));
        return Event(evt);
    }

    /// Enqueues a command to fill a buffer object with a pattern
    ///
    /// @tparam T C++ type of the buffer being filled
    /// @tparam U C++ type of the pattern
    /// @tparam D Range dimension
    /// @param buffer Buffer being filled
    /// @param pattern Pattern to fill the buffer with
    /// @param range Range
    /// @param wait_list Specify events that need to complete before this particular command can be
    ///        executed
    /// @return Event object that identifies this particular write command
    template <typename T, typename U, int D>
    Event
    enqueue_fill_buffer(const Buffer<T, D> & buffer,
                        const U & pattern,
                        const Range<D> & range,
                        const std::vector<Event> & wait_list = std::vector<Event>()) const
    {
        return enqueue_fill_buffer_raw(buffer,
                                       &pattern,
                                       sizeof(U),
                                       0,
                                       range.size() * sizeof(U),
                                       wait_list);
    }

    /// Issues all previously queued OpenCL commands in a command-queue to the device associated
    /// with the command-queue.
    void flush() const;

    /// Blocks until all previously queued OpenCL commands in a command-queue are issued to the
    /// associated device and have completed.
    void finish() const;

private:
    /// Enqueue commands to read from a buffer object to host memory in blocking mode
    ///
    /// @param buffer Buffer to read from
    /// @param offset The offset in bytes in the buffer object to read from.
    /// @param size The size in bytes of data being read.
    /// @param ptr The pointer to buffer in host memory where data is to be read into.
    /// @param wait_list Specify events that need to complete before this particular command can be
    ///        executed
    void enqueue_read_raw(const Memory & buffer,
                          size_t offset,
                          size_t size,
                          void * ptr,
                          const std::vector<Event> & wait_list = std::vector<Event>()) const;

    /// Enqueue commands to read from a buffer object to host memory in non-blocking mode
    ///
    /// @param buffer Buffer to read from
    /// @param offset The offset in bytes in the buffer object to read from.
    /// @param size The size in bytes of data being read.
    /// @param ptr The pointer to buffer in host memory where data is to be read into.
    /// @param wait_list Specify events that need to complete before this particular command can be
    ///        executed
    /// @return Event object that identifies this particular kernel execution instance
    Event enqueue_iread_raw(const Memory & buffer,
                            size_t offset,
                            size_t size,
                            void * ptr,
                            const std::vector<Event> & wait_list = std::vector<Event>()) const;

    /// Enqueue commands to write to a buffer object from host memory in blocking mode
    ///
    /// @param buffer Buffer to write into
    /// @param offset The offset in bytes in the buffer object to write to.
    /// @param size The size in bytes of data being written.
    /// @param ptr The pointer to buffer in host memory where data is to be written from.
    /// @param wait_list Specify events that need to complete before this particular command can be
    ///        executed
    void enqueue_write_raw(const Memory & buffer,
                           size_t offset,
                           size_t size,
                           const void * ptr,
                           const std::vector<Event> & wait_list = std::vector<Event>()) const;

    /// Enqueue commands to write to a buffer object from host memory in non-blocking mode
    ///
    /// @param buffer Buffer to write into
    /// @param offset The offset in bytes in the buffer object to write to.
    /// @param size The size in bytes of data being written.
    /// @param ptr The pointer to buffer in host memory where data is to be written from.
    /// @param wait_list Specify events that need to complete before this particular command can be
    ///        executed
    /// @return Event object that identifies this particular kernel execution instance
    Event enqueue_iwrite_raw(const Memory & buffer,
                             size_t offset,
                             size_t size,
                             const void * ptr,
                             const std::vector<Event> & wait_list = std::vector<Event>()) const;

    /// Enqueues a command to copy from one buffer object to another.
    ///
    /// @param src Source buffer
    /// @param dest Destination buffer
    /// @param src_offset The offset where to begin copying data from `src`.
    /// @param dest_offset The offset where to begin copying data into `dst`.
    /// @param size Refers to the size in bytes to copy.
    /// @param wait_list Specify events that need to complete before this particular command can be
    ///        executed
    /// @return Event object that identifies this particular kernel execution instance
    Event enqueue_copy_raw(const Memory & src,
                           const Memory & dest,
                           size_t src_offset,
                           size_t dest_offset,
                           size_t size,
                           const std::vector<Event> & wait_list = std::vector<Event>()) const;

    /// Enqueues a command to map a region of the buffer object given by `buffer` into the host
    /// address space and returns a pointer to this mapped region.
    ///
    /// @param buffer Buffer to map
    /// @param blocking Indicates if the map operation is blocking or non-blocking.
    /// @param flags A bit-field with the following supported values:
    ///   - `READ` This flag specifies that the region being mapped in the memory object is being
    ///      mapped for reading.
    ///   - `WRITE` This flag specifies that the region being mapped in the memory object is being
    ///      mapped for writing.
    ///   - `WRITE_INVALIDATE_REGION` This flag specifies that the region being mapped in the memory
    ///      object is being mapped for writing.
    ///
    ///   `READ` or `WRITE` and `WRITE_INVALIDATE_REGION` are mutually exclusive.
    /// @param offset The offset in bytes of the region in the buffer object that is being mapped.
    /// @param size The size of the region in the buffer object that is being mapped.
    /// @return The returned pointer maps a region starting at `offset` and is at least `size` bytes
    ///         in size. The result of a memory access outside this region is undefined.
    void * enqueue_map_buffer_raw(const Memory & buffer,
                                  bool blocking,
                                  MapFlags flags,
                                  size_t offset,
                                  size_t size) const;

    /// Enqueues a command to fill a buffer object with a pattern of a given pattern size.
    ///
    /// @param buffer A valid buffer object
    /// @param pattern A pointer to the data pattern of size `pattern_size` in bytes
    /// @param pattern_size Size of the `pattern` in bytes
    /// @param offset The location in bytes of the region being filled in `buffer` and must be a
    ///        multiple of `pattern_size`.
    /// @param size The size in bytes of region being filled in `buffer` and must be a multiple of
    ///        `pattern_size`.
    /// @param wait_list Specify events that need to complete before this particular command can be
    ///        executed
    /// @return Event object that identifies this particular write command
    Event
    enqueue_fill_buffer_raw(const Memory & buffer,
                            const void * pattern,
                            size_t pattern_size,
                            size_t offset,
                            size_t size,
                            const std::vector<Event> & wait_list = std::vector<Event>()) const;

    template <typename T>
    T
    get_info(cl_command_queue_info name) const
    {
        T val;
        get_info_helper(clGetCommandQueueInfo, this->q, name, val);
        return val;
    }

    /// Underlying OpenCL queue
    cl_command_queue q;

public:
    /// Get the default queue
    ///
    /// @return Default queue object
    static Queue get_default();

private:
    static std::once_flag have_default;
    static Queue default_queue;
};

} // namespace openclcpp_lite
