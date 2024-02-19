using System.Security.Cryptography;
using System.Text;

namespace LokiServer
{
    public class Utils
    {
        public static string XOR(string input, string key)
        {
            string output = "";
            for (int i = 0; i < input.Length; i++)
            {
                output += (char)(input[i] ^ key[i % key.Length]);
            }
            return output;
        }

        public static int S12hash(string str)
        {
            int sum = 0;
            foreach (char ch in str)
            {
                sum += (int)ch;
            }
            sum = sum * 83 + 12;
            return sum;
        }

        public static ulong CreateS12Hash(string str)
        {
            int h1 = S12hash(str);
            int h2 = S12hash(new string(str.Reverse().ToArray()));
            int h3 = h2 / 12;

            string hashValue = $"{h1}{h2}{h3}";

            ulong finalHashValue = ulong.Parse(hashValue) * (83 + 12);
            return finalHashValue;
        }

        public static bool validateAPIRequest(string XAppTimestamp, string XAppSignature, string url, string method)
        {
            string signatureData = method + url + XAppTimestamp;
            ulong computedSignature = CreateS12Hash(signatureData);

            if (computedSignature.ToString() == XAppSignature)
            {
                DateTime timestamp = DateTime.Parse(XAppTimestamp);
                if (DateTime.Now.Subtract(timestamp).TotalMinutes < 10)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            return false;
        }
    }
}
