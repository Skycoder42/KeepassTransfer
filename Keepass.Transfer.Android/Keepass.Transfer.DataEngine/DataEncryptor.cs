using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using Java.Security;
using Java.Security.Spec;
using Javax.Crypto;

namespace Keepass.Transfer.DataEngine
{
    internal static class DataEncryptor
    {
        public static Task EncryptDataAsync(IList<DataEntry> transferData, string publicKey)
        {
            return Task.Run(() => EncryptData(transferData, publicKey));
        }

        public static void EncryptData(IList<DataEntry> transferData, string publicKey)
        {
            var key = DataEncryptor.ReadKey(publicKey);
            foreach (var dataEntry in transferData)
                dataEntry.Value = Convert.ToBase64String(DataEncryptor.Encrypt(Encoding.UTF8.GetBytes(dataEntry.Value), key));
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