# raspivid -n -mm matrix -w 1280 -h 720 -fps 25 -g 100 -t 0 -b 2000000 -o - | ffmpeg -y -f h264 -i - -c:v copy -map 0:0 -f flv rtmp://192.168.100.199/live/stream
ffmpeg -an -f video4linux2 -s 853x480 -i /dev/video0 -r 10 -b:v 500k -f flv rtmp://192.168.100.183/live/stream
