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
        public static void EncryptData(IList<DataEntry> transferData, string publicKey)
        {
            var key = ReadKey(publicKey);
            foreach (var dataEntry in transferData)
                dataEntry.Value = Convert.ToBase64String(Encrypt(Encoding.UTF8.GetBytes(dataEntry.Value), key));
        }

        public static async Task EncryptDataAsync(IList<DataEntry> transferData, string publicKey)
        {
            var key = await Task.Run(() => ReadKey(publicKey));//TODO configureAwait
            await Task.WhenAll(transferData
                .Select(dataEntry => Task.Run(() => {
                    dataEntry.Value = Convert.ToBase64String(Encrypt(Encoding.UTF8.GetBytes(dataEntry.Value), key));
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