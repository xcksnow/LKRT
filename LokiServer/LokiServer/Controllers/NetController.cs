using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;

namespace LokiServer.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class NetController : Controller
    {
        [HttpPost]
        [Route("Example")]

        public static string XOR(string input, string key)
        {
            string output = "";
            for (int i = 0; i < input.Length; i++)
            {
                output += (char)(input[i] ^ key[i % key.Length]);
            }
            return output;
        }
        public IActionResult Example()
        {
            string host = Request.Host.Value;
            string XAppTimestamp = Request.Headers["X-App-Timestamp"];
            string XAppSignature = Request.Headers["X-App-Signature"];
            string url = "/Example";
            if (Utils.validateAPIRequest(XAppTimestamp, XAppSignature, url, Request.Method))
            {
                return Ok();
            }
            else
            {
                return Unauthorized();
            }
        }
       
    }
}
