using Newtonsoft.Json;

namespace Keepass.Transfer.DataEngine
{
    public class DataEntry
    {
        public string Key { get; set; }
        public bool Guarded { get; set; } = false;
        public string Value { get; set; }
    }
}