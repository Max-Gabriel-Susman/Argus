#include "Argus.h"
#include <iostream>

// Include GStreamer headers
#include <gst/gst.h>
#include <glib.h>
#include <gst/gstdebugutils.h> // Needed for gst_debug_bin_to_dot_file* macros

// We’ll keep a pointer to the main loop here so that start/stop can manage it
static GMainLoop* loop = nullptr;

/**
 * Initialize the GStreamer library
 */
void init_gstreamer()
{
    gst_init(nullptr, nullptr);
}

/**
 * Parse and run an RTMP-forwarding pipeline until stopped.
 *
 * \param pipelineStr A string describing the GStreamer pipeline
 */
void start_rtmp_forwarding(const std::string& pipelineStr)
{
    std::cout << "Parsing pipeline string..." << std::endl;
    // Parse the pipeline string into a GstElement*
    GstElement* pipeline = gst_parse_launch(pipelineStr.c_str(), nullptr);
    if (!pipeline) {
        std::cerr << "Failed to create pipeline from string." << std::endl;
        return;
    }
    std::cout << "Creating main loop..." << std::endl;

    // Create a main loop so we can run the pipeline
    loop = g_main_loop_new(nullptr, FALSE);

    std::cout << "Attaching bus..." << std::endl;

    // Attach a bus watch to handle async messages (errors, EOS, etc.)
    GstBus* bus = gst_element_get_bus(pipeline);
    gst_bus_add_watch(bus, (GstBusFunc)gst_bus_async_signal_func, nullptr);
    gst_object_unref(bus);

    std::cout << "Setting pipeline to playing..." << std::endl;

    // Set the pipeline to PLAYING
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    std::cout << "Dumping dot file..." << std::endl;

    //// Optionally wait for state change to complete before dumping the dot file
    //GstStateChangeReturn ret = gst_element_get_state(pipeline, nullptr, nullptr, GST_CLOCK_TIME_NONE);
    //if (ret == GST_STATE_CHANGE_SUCCESS || ret == GST_STATE_CHANGE_ASYNC) {
    //    // Generate a DOT file showing the pipeline topology at this time
    //    gst_debug_bin_to_dot_file_with_ts(
    //        GST_BIN(pipeline),
    //        GST_DEBUG_GRAPH_SHOW_ALL,
    //        "rtmp_forward_pipeline"
    //    );
    //    std::cout << "DOT file generated for pipeline graph." << std::endl;
    //}

    std::cout << "Starting main loop..." << std::endl;

    // This blocks until stop_pipeline() is called (g_main_loop_quit)
    g_main_loop_run(loop);

    std::cout << "Setting pipeline to playing..." << std::endl;

    // Once we exit g_main_loop_run, set pipeline to NULL and unref
    gst_element_set_state(pipeline, GST_STATE_NULL);

    std::cout << "Unreferencing pipeline..." << std::endl;

    gst_object_unref(pipeline);

    // Cleanup the main loop
    if (loop) {
        g_main_loop_unref(loop);
        loop = nullptr;
    }
}

/**
 * Stop the currently running pipeline
 */
void stop_pipeline()
{
    if (loop) {
        g_main_loop_quit(loop);
    }
}

int main()
{
    std::cout << "Initializing Argus..." << std::endl;

    // 1. Initialize GStreamer
    init_gstreamer();

    std::cout << "Argus initialized." << std::endl;

    // 2. Define GStreamer pipeline string for 2-server setup
    const std::string pipelineStr =
        "rtmpsrc location=\"rtmp://localhost:1935/live/myStream\" ! "
        "flvdemux name=demux "
        "demux.video ! queue ! h264parse ! mux. "
        "demux.audio ! queue ! aacparse ! mux. "
        "flvmux name=mux ! "
        "rtmpsink location=\"rtmp://localhost:2935/live/myRestream\"";

    // 3. Start RTMP forwarding. Blocks until stop_pipeline() is called.
    start_rtmp_forwarding(pipelineStr);

    std::cout << "Argus online." << std::endl;
    return 0;
}
