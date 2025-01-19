#include "Argus.h"
#include <gst/gst.h>  // GStreamer main header

int main(int argc, char* argv[])
{
    // Standard GStreamer init
    gst_init(&argc, &argv);

    g_print("Initializing Argus with GStreamer...\n");

    // Create a simple test pipeline: videotestsrc ! autovideosink
    GError* error = nullptr;
    GstElement* pipeline = gst_parse_launch("videotestsrc ! autovideosink", &error);

    if (!pipeline) {
        g_printerr("Pipeline parse error: %s\n", error->message);
        return -1;
    }

    // Start playing
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // Let it run a little
    g_print("Pipeline is playing...\n");
    g_usleep(3 * G_USEC_PER_SEC);  // sleep 3 seconds

    // Shutdown
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    g_print("Argus shutting down.\n");
    return 0;
}
