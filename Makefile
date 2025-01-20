

stream:
	ffmpeg -re -i media/input.mp4 \
    -c:v libx264 -preset veryfast -c:a aac -ar 44100 \
    -f flv rtmp://localhost/outgoing/myRestream
