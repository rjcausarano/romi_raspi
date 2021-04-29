raspivid -n -mm matrix -w 1280 -h 720 -fps 25 -g 100 -t 0 -b 2000000 -o - | ffmpeg -y -f h264 -i - -c:v copy -map 0:0 -f flv rtmp://192.168.100.199/live/stream
