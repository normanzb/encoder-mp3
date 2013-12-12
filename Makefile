default:
	gcc ./*.c -emit-swc=im.norm.data.encoders.MP3Encoder -o encoder-mp3.swc

clean:
	rm -f *.swf *.swc *.o *.exe