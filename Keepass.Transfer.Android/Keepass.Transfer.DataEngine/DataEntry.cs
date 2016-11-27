using Newtonsoft.Json;

namespace Keepass.Transfer.DataEngine
{
    public class DataEntry
    {
        public string Key { get; set; }
        public string Value { get; set; }

        public bool Guarded { get; set; } = false;
        public bool Encrypted { get; set; } = false;

        public override string ToString()
        {
            return Guarded ? Key + " \uD83D\uDD12" : Key;
        }
    }
}