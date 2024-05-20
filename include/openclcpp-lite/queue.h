#pragma once

#include "openclcpp-lite/cl.h"
#include "openclcpp-lite/templ.h"
#include "openclcpp-lite/enums.h"
#include "openclcpp-lite/error.h"
#include "openclcpp-lite/buffer.h"
#include "openclcpp-lite/event.h"

namespace openclcpp_lite {

class Context;
class Device;
class Kernel;
class NDRange;

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

    /// Enqueue commands to read from a buffer object to host memory in blocking mode
    ///
    /// @param buffer Buffer to read from
    /// @param offset The offset entries in the buffer object to read from.
    /// @param n The number of entries of data being read.
    /// @param ptr The pointer to buffer in host memory where data is to be read into.
    /// @param wait_list Specify events that need to complete before this particular command can be
    ///        executed
    template <typename T>
    void
    enqueue_read(const TBuffer<T> & buffer,
                 size_t offset,
                 size_t n,
                 void * ptr,
                 const std::vector<Event> & wait_list = std::vector<Event>()) const
    {
        enqueue_read_raw(buffer, offset * sizeof(T), n * sizeof(T), ptr, wait_list);
    }

    /// Enqueue commands to read from a buffer object to host memory in non-blocking mode
    ///
    /// @param buffer Buffer to read from
    /// @param offset The offset entries in the buffer object to read from.
    /// @param n The number of entries of data being read.
    /// @param ptr The pointer to buffer in host memory where data is to be read into.
    /// @param wait_list Specify events that need to complete before this particular command can be
    ///        executed
    /// @return Event object that identifies this particular kernel execution instance
    template <typename T>
    Event
    enqueue_iread(const TBuffer<T> & buffer,
                  size_t offset,
                  size_t n,
                  void * ptr,
                  const std::vector<Event> & wait_list = std::vector<Event>()) const
    {
        return enqueue_iread_raw(buffer, offset * sizeof(T), n * sizeof(T), ptr, wait_list);
    }

    /// Enqueue commands to write to a buffer object from host memory in blocking mode
    ///
    /// @param buffer Buffer to write into
    /// @param offset The offset entries in the buffer object to write to.
    /// @param n The number of entries of data being read.
    /// @param ptr The pointer to buffer in host memory where data is to be written from.
    /// @param wait_list Specify events that need to complete before this particular command can be
    ///        executed
    template <typename T>
    void
    enqueue_write(const TBuffer<T> & buffer,
                  size_t offset,
                  size_t n,
                  const void * ptr,
                  const std::vector<Event> & wait_list = std::vector<Event>()) const
    {
        enqueue_write_raw(buffer, offset * sizeof(T), n * sizeof(T), ptr, wait_list);
    }

    /// Enqueue commands to write to a buffer object from host memory in non-blocking mode
    ///
    /// @param buffer Buffer to write into
    /// @param offset The offset entries in the buffer object to write to.
    /// @param n The number of entries of data being read.
    /// @param ptr The pointer to buffer in host memory where data is to be written from.
    /// @param wait_list Specify events that need to complete before this particular command can be
    ///        executed
    template <typename T>
    Event
    enqueue_iwrite(const TBuffer<T> & buffer,
                   size_t offset,
                   size_t n,
                   const void * ptr,
                   const std::vector<Event> & wait_list = std::vector<Event>()) const
    {
        return enqueue_iwrite_raw(buffer, offset * sizeof(T), n * sizeof(T), ptr, wait_list);
    }

    /// Enqueues a command to copy from one buffer object to another.
    ///
    /// @param src Source buffer
    /// @param dest Destination buffer
    /// @param src_offset The offset entries where to begin copying data from `src`.
    /// @param dest_offset The offset entries where to begin copying data into `dst`.
    /// @param n Refers to the number of entries to copy.
    /// @param wait_list Specify events that need to complete before this particular command can be
    ///        executed
    /// @return Event object that identifies this particular kernel execution instance
    template <typename T>
    Event
    enqueue_copy(const TBuffer<T> & src,
                 const TBuffer<T> & dest,
                 size_t src_offset,
                 size_t dest_offset,
                 size_t n,
                 const std::vector<Event> & wait_list = std::vector<Event>())
    {
        return enqueue_copy_raw(src,
                                dest,
                                src_offset * sizeof(T),
                                dest_offset * sizeof(T),
                                n * sizeof(T),
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
    /// @param offset The offset entries of the region in the buffer object that is being mapped.
    /// @param n The number of entries of the region in the buffer object that is being mapped.
    /// @return The returned pointer maps a region starting at `offset` and is at least `size` bytes
    ///         in size. The result of a memory access outside this region is undefined.
    template <typename T>
    T *
    enqueue_map_buffer(const TBuffer<T> & buffer,
                       bool blocking,
                       MapFlags flags,
                       size_t offset,
                       size_t n) const
    {
        return static_cast<T *>(
            enqueue_map_buffer_raw(buffer, blocking, flags, offset * sizeof(T), n * sizeof(T)));
    }

    /// A synchronization point that enqueues a barrier operation.
    Event enqueue_barrier(const std::vector<Event> & wait_list = std::vector<Event>()) const;

    /// Enqueues a command to execute a kernel on a device.
    ///
    /// @param kernel Kernel to execute
    /// @param global Global range
    /// @param wait_list Specify events that need to complete before this particular command can be
    ///        executed
    /// @return Event object that identifies this particular kernel execution instance
    Event enqueue_kernel(const Kernel & kernel,
                         const NDRange & global,
                         const std::vector<Event> & wait_list = std::vector<Event>());

    /// Issues all previously queued OpenCL commands in a command-queue to the device associated
    /// with the command-queue.
    void flush() const;

    /// Blocks until all previously queued OpenCL commands in a command-queue are issued to the
    /// associated device and have completed.
    void finish() const;

private:
    Queue(cl_command_queue q);

    /// Enqueue commands to read from a buffer object to host memory in blocking mode
    ///
    /// @param buffer Buffer to read from
    /// @param offset The offset in bytes in the buffer object to read from.
    /// @param size The size in bytes of data being read.
    /// @param ptr The pointer to buffer in host memory where data is to be read into.
    /// @param wait_list Specify events that need to complete before this particular command can be
    ///        executed
    void enqueue_read_raw(const Buffer & buffer,
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
    Event enqueue_iread_raw(const Buffer & buffer,
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
    void enqueue_write_raw(const Buffer & buffer,
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
    Event enqueue_iwrite_raw(const Buffer & buffer,
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
    Event enqueue_copy_raw(const Buffer & src,
                           const Buffer & dest,
                           size_t src_offset,
                           size_t dest_offset,
                           size_t size,
                           const std::vector<Event> & wait_list = std::vector<Event>());

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
    void * enqueue_map_buffer_raw(const Buffer & buffer,
                                  bool blocking,
                                  MapFlags flags,
                                  size_t offset,
                                  size_t size) const;

    template <typename T>
    T
    get_info(cl_command_queue_info name) const
    {
        T val;
        get_info_helper(clGetCommandQueueInfo, this->q, name, val);
        return val;
    }

    cl_command_queue q;

    friend class Event;
};

} // namespace openclcpp_lite
