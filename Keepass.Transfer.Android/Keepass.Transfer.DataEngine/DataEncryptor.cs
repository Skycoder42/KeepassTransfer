using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Java.Security;
using Java.Security.Spec;
using Javax.Crypto;

namespace Keepass.Transfer.DataEngine
{
    internal static class DataEncryptor
    {
        public static void EncryptData(IList<DataEntry> transferData, string publicKey, bool encryptUnGuarded)
        {
            var key = ReadKey(publicKey);
            foreach (var dataEntry in transferData) {
                if (encryptUnGuarded || dataEntry.Guarded) {
                    dataEntry.Value = Convert.ToBase64String(Encrypt(Encoding.UTF8.GetBytes(dataEntry.Value), key));
                    dataEntry.Encrypted = true;
                }
            }
        }

        public static async Task EncryptDataAsync(IList<DataEntry> transferData, string publicKey, bool encryptUnGuarded)
        {
            var key = await Task.Run(() => ReadKey(publicKey));
            await Task.WhenAll(transferData
                .Where(dataEntry => encryptUnGuarded || dataEntry.Guarded)
                .Select(dataEntry => Task.Run(() => {
                    dataEntry.Value = Convert.ToBase64String(Encrypt(Encoding.UTF8.GetBytes(dataEntry.Value), key));
                    dataEntry.Encrypted = true;
                }))
                .ToArray());
        }

        private static IPublicKey ReadKey(string key)
        {
            var publicKeyBytes = Convert.FromBase64String(key);
            
            var x509PublicKey = new X509EncodedKeySpec(publicKeyBytes);
            var kf = KeyFactory.GetInstance("RSA");

            return kf.GeneratePublic(x509PublicKey);
        }

        private static byte[] Encrypt(byte[] data, IPublicKey key)
        {
            var encryptor = Cipher.GetInstance("RSA/ECB/PKCS1Padding");
            encryptor.Init(CipherMode.EncryptMode, key);
            return encryptor.DoFinal(data);
        }
    }
}