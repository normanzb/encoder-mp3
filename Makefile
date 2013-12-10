default:
	gcc ./*.c -emit-swc=test.namespace -o encoder-mp3.swc

clean:
	rm -f *.swf *.swc *.o *.exe