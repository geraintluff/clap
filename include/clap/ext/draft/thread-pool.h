#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../../clap.h"

/// @page
///
/// This extension let the plugin use the host's thread pool.
///
/// The plugin must provide @ref clap_plugin_thread_pool, and the host may provide @ref
/// clap_host_thread_pool. If it doesn't, the plugin should process its data by its own mean. In the
/// worst case, a single threaded for-loop.
///
/// Simple example with N voices to process
///
/// @code
/// void myplug_thread_pool_exec(const clap_plugin *plugin, uint32_t voice_index)
/// {
///    compute_voice(plugin, voice_index);
/// }
///
/// void myplug_process(const clap_plugin *plugin, const clap_process *process)
/// {
///    ...
///    bool didComputeVoices = false;
///    if (host_thread_pool && host_thread_pool.exec)
///       didComputeVoices = host_thread_pool.request_exec(host, plugin, N);
///
///    if (!didComputeVoices)
///       for (uint32_t i = 0; i < N; ++i)
///          myplug_thread_pool_exec(plugin, N);
///    ...
/// }
/// @endcode

#define CLAP_EXT_THREAD_POOL "clap/draft/thread-pool"

typedef struct clap_plugin_thread_pool {
   // Called by the thread pool
   void (*exec)(const clap_plugin *plugin, uint32_t task_index);
} clap_plugin_thread_pool;

typedef struct clap_host_thread_pool {
   // Schedule num_tasks jobs in the host thread pool.
   // It can't be called concurrently or from the thread pool.
   // Will block until all the tasks are processed.
   // This must be used exclusively for realtime processing within the process call.
   // Returns true if the host did execute all the tasks, false if it rejected the request.
   // The host should check that the plugin is within the process call, and if not, reject the exec
   // request.
   // [audio-thread]
   bool (*request_exec)(const clap_host *host, uint32_t num_tasks);
} clap_host_thread_pool;

#ifdef __cplusplus
}
#endif