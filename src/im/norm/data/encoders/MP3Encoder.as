package im.norm.data.encoders
{
    import flash.utils.ByteArray;
    import flash.utils.Endian;

    import com.demonsters.debugger.MonsterDebugger;
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
            var baWave:ByteArray = WaveEncoder.encode(baRaw, config);

            return im.norm.data.encoders.MP3Encoder.shine.compress(baWave);
        }
    }
}