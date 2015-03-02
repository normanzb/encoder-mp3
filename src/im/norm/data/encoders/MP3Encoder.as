package im.norm.data.encoders
{
    import flash.utils.ByteArray;
    import flash.utils.Endian;

    import im.norm.data.encoders.WaveEncoder;
    import im.norm.data.encoders.MP3Encoder.shine.compress;

    public class MP3Encoder
        extends Encoder
    {
        function MP3Encoder()
        {

        }

        public override function encode(baRaw:ByteArray, config:Object ):ByteArray
        {
            var waveEncoder:WaveEncoder = new WaveEncoder();
            var baWave:ByteArray = waveEncoder.encode(baRaw, config);
            var ret:ByteArray;

            try
            {
                ret = im.norm.data.encoders.MP3Encoder.shine.compress(baWave);
            }
            catch(ex:*)
            {
                trace('failed to encode mp3, ex:' );
                trace(ex );
            }

            return ret;
        }
    }
}