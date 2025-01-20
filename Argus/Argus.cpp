#include "Argus.h"
#include <iostream>

// Include GStreamer headers
#include <gst/gst.h>
#include <glib.h>

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
    // Parse the pipeline string into a GstElement*
    GstElement* pipeline = gst_parse_launch(pipelineStr.c_str(), nullptr);
    if (!pipeline) {
        std::cerr << "Failed to create pipeline from string." << std::endl;
        return;
    }

    // Create a main loop so we can run the pipeline
    loop = g_main_loop_new(nullptr, FALSE);

    // Attach a bus watch to handle async messages (errors, EOS, etc.)
    GstBus* bus = gst_element_get_bus(pipeline);
    gst_bus_add_watch(bus, (GstBusFunc)gst_bus_async_signal_func, nullptr);
    gst_object_unref(bus);

    // Set the pipeline to PLAYING
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // This blocks until stop_pipeline() is called (g_main_loop_quit)
    g_main_loop_run(loop);

    // Once we exit g_main_loop_run, set pipeline to NULL and unref
    gst_element_set_state(pipeline, GST_STATE_NULL);
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

/**
 * Main entry point
 */
int main()
{
    std::cout << "Initializing Argus..." << std::endl;

    // 1. Initialize GStreamer
    init_gstreamer();

    // 2. Define your GStreamer pipeline string
    //    (Update URIs and elements as needed for your setup)
    const std::string pipelineStr =
        "rtmpsrc location=\"rtmp://localhost/incoming/myStream\" ! "
        "flvdemux name=demux "
        "demux.video ! queue ! h264parse ! mux. "
        "demux.audio ! queue ! aacparse ! mux. "
        "flvmux name=mux ! "
        "rtmpsink location=\"rtmp://localhost/outgoing/myRestream\"";

    // 3. Start RTMP forwarding. This call will block until stop_pipeline() is called.
    //    In a real application, you might trigger stop_pipeline() on some event or signal.
    start_rtmp_forwarding(pipelineStr);

    std::cout << "Argus online." << std::endl;
    return 0;
}
