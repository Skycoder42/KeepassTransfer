using Newtonsoft.Json;

namespace Keepass.Transfer.DataEngine
{
    public class QrData
    {
        public string Secret { get; set; }
        public string PublicKey { get; set; }
    }
}