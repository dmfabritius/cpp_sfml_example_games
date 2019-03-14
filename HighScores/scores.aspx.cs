using System;
using System.Configuration;
using System.Data;
using System.Data.SqlClient;

namespace HighScores {

    public partial class scores : System.Web.UI.Page {

        protected void Page_Load(object sender, EventArgs e) {
            string response = "";
            string apikey = Request.QueryString["apikey"];
            if (apikey == "11") {
                string action = Request.QueryString["action"];
                string name = Request.QueryString["name"];
                string score = Request.QueryString["score"];
                if (action == "read") {
                    response = dbRead();
                } else if (action == "reset") {
                    dbReset();
                } else if (action == "write" && name != "" && score != "") {
                    dbWrite(name, score);
                }
            }
            Response.Clear();
            Response.ContentType = "text/plain; charset=utf-8";
            Response.Write(response);
            Response.End();
        }

        private string dbRead() {
            SqlDataAdapter adapter = new SqlDataAdapter(
                "SELECT TOP 10 name, score FROM Scores" +
                " ORDER BY CAST(score AS int) DESC",
                ConfigurationManager.ConnectionStrings["myDb"].ToString());
            DataTable table = new DataTable();
            adapter.Fill(table);
            if (table.Rows.Count != 0) {
                string data = "";
                foreach (DataRow row in table.Rows) {
                    data += ";" + row["name"] + "," + row["score"];
                }
                return data.Substring(1);
            } else {
                return "";
            }
        }

        private void dbReset() {
            SqlConnection cnn = new SqlConnection(ConfigurationManager.ConnectionStrings["myDb"].ToString());
            cnn.Open();
            SqlCommand cmd = new SqlCommand("DELETE FROM scores", cnn);
            cmd.ExecuteNonQuery();
            cnn.Close();
        }

        private void dbWrite(string name, string score) {
            SqlConnection cnn = new SqlConnection(ConfigurationManager.ConnectionStrings["myDb"].ToString());
            cnn.Open();
            SqlCommand cmd = new SqlCommand(
                "INSERT scores (timestamp, name, score, agent, address, browser)" +
                "VALUES (@timestamp, @name, @score, @agent, @address, @browser)", cnn);
            cmd.Parameters.Add("@timestamp", SqlDbType.VarChar).Value = DateTime.Now.ToString();
            cmd.Parameters.Add("@name", SqlDbType.VarChar).Value =
                name.Substring(0, Math.Min(10, name.Length)).Replace(",", "").Replace(";", "");
            cmd.Parameters.Add("@score", SqlDbType.VarChar).Value =
                score.Substring(0, Math.Min(6, score.Length)).Replace(",", "").Replace(";", "");
            cmd.Parameters.Add("@agent", SqlDbType.VarChar).Value = Request.UserAgent;
            cmd.Parameters.Add("@address", SqlDbType.VarChar).Value = Request.UserHostAddress;
            cmd.Parameters.Add("@browser", SqlDbType.VarChar).Value = Request.Browser.Browser;
            cmd.ExecuteNonQuery();
            cnn.Close();
        }
    }
}
