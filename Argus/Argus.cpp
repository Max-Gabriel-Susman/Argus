#include "Argus.h"
#include <gst/gst.h>

int main(int argc, char* argv[])
{
    gst_init(&argc, &argv);

    g_print("Starting GStreamer pipeline...\n");

    // Example pipeline:
    //
    // rtmpsrc location=rtmp://yourHost/live/test !
    //   flvdemux name=demux
    //   demux.video ! queue ! h264parse ! avdec_h264 ! videoconvert ! x264enc tune=zerolatency bitrate=2000 ! queue ! mux.
    //   demux.audio ! queue ! decodebin ! audioconvert ! audioresample ! avenc_aac ! queue ! mux.
    //   mpegtsmux name=mux ! tcpserversink host=0.0.0.0 port=5000
    //
    // - "rtmpsrc location=..." pulls RTMP from an existing server
    // - "tcpserversink" lets VLC connect via tcp://host:5000

    const char* pipeline_desc =
        "rtmpsrc location=rtmp://yourHost/live/test ! "
        "flvdemux name=demux "
        "demux.video ! queue ! h264parse ! avdec_h264 ! videoconvert ! x264enc tune=zerolatency bitrate=2000 ! queue ! mux. "
        "demux.audio ! queue ! decodebin ! audioconvert ! audioresample ! avenc_aac ! queue ! mux. "
        "mpegtsmux name=mux ! tcpserversink host=0.0.0.0 port=5000";

    GError* error = nullptr;
    GstElement* pipeline = gst_parse_launch(pipeline_desc, &error);

    if (!pipeline) {
        g_printerr("Pipeline parse error: %s\n", error->message);
        g_clear_error(&error);
        return -1;
    }
    if (error) {
        g_printerr("Pipeline parse warning: %s\n", error->message);
        g_clear_error(&error);
    }

    // Start playing
    GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr("Unable to set the pipeline to the playing state.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    g_print("Pipeline is playing. Press Ctrl+C to stop or wait for EOS.\n");

    // Create a GMainLoop so we can watch for messages (errors, EOS, etc.)
    GMainLoop* loop = g_main_loop_new(nullptr, FALSE);

    // Watch the pipeline's bus
    GstBus* bus = gst_element_get_bus(pipeline);
    gst_bus_add_watch(
        bus,
        [](GstBus* bus, GstMessage* msg, gpointer user_data) -> gboolean {
            switch (GST_MESSAGE_TYPE(msg)) {
            case GST_MESSAGE_ERROR:
            {
                GError* err;
                gchar* debug;
                gst_message_parse_error(msg, &err, &debug);
                g_printerr("Error from %s: %s\n", GST_OBJECT_NAME(msg->src), err->message);
                g_printerr("Debug info: %s\n", debug ? debug : "none");
                g_clear_error(&err);
                g_free(debug);
                g_main_loop_quit((GMainLoop*)user_data);
                break;
            }
            case GST_MESSAGE_EOS:
                g_print("End-Of-Stream reached.\n");
                g_main_loop_quit((GMainLoop*)user_data);
                break;
            default:
                break;
            }
            return TRUE;
        },
        loop);
    gst_object_unref(bus);

    // Run the loop until EOS or error
    g_main_loop_run(loop);

    g_print("Main loop finished. Cleaning up...\n");
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    g_main_loop_unref(loop);

    g_print("Done.\n");
    return 0;
}
