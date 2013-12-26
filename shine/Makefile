default:
	gcc ./*.c -Oz -emit-llvm -emit-swc=im.norm.data.encoders.MP3Encoder.shine -o ../lib/encoder-mp3.swc

debug:
	gcc ./*.c -g -O0 -emit-swc=im.norm.data.encoders.MP3Encoder.shine -o ../lib/encoder-mp3.swc	

clean:
	rm -f *.swf *.swc *.o *.exe