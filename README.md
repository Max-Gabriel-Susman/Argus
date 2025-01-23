# Argus

Argus is a media streaming service that uses gstreamer to build and manage media pipelines and performing image processing tasks like object detection.

## Local Usage

Argus development is done in Visual Studio 2022 on Windows 11 against Ubuntu 24.04 on WSL 2; the following instructions are for this setup.

In powershell navigate to C:\Users\<your-user>\source\repos\ and clone the service: 
```
https://github.com/Max-Gabriel-Susman/Argus
```

Open the folder of the service in Visual Studio on Windows 11 and set the following targets: WSL: Ubuntu(24.04), Linux Debug, & Argus

If you don't have WSL2 setup yet for Ubuntu 24.04 with nginx and nginx RTMP module installed do so now; you'll need to use the nginx.conf included at the root level of this repository. Instructions for this can be found elsewhere.

The rest of the Local Usage Instructions are to be performed from C:\Users\{your-user}\source\repos\Argus in Ubuntu-22.04/WSL 2; from the linux terminal navigate to /mnt/c/Users/{your-user}/source/repos/Argus to use this repositories resources.

Start nginx with the config:
```
sudo systemctl start nginx
```

Push an mp4 file(you'll have to add your own mp4 file as none are kept on Github) from this repositories media directory with ffmpeg:
```
make stream
```

ingest the stream with VLC:
    1. Media > Open Network Stream 

    2. Use `rtmp://localhost/outgoing/myRestream` as the network URL

