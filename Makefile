
stream: 
	ffmpeg -re -i media/input.mp4 \
    -c:v libx264 -c:a aac -f flv \
    rtmp://localhost/live/test