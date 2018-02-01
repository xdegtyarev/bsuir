using System;
using System.Collections;
using System.Collections.Generic;
using System.Web;
using System.Web.UI;
using IPR.DataAccess;
using MongoDB.Driver;
using System.Linq;
namespace IPR
{
    public partial class Client : System.Web.UI.Page
    {
        void Page_Load()
        {
            Console.WriteLine("Client page load");
            ClientCreditGridView.DataSource = CreateDataSource();
            ClientCreditGridView.DataBind();
        }

        //void Page_Load(object sender, EventArgs e)  {
        //    Console.WriteLine("Client page load");
        //    // Load data for the DropDownList control only once, when the 
        //    // page is first loaded.
        //    if (!IsPostBack)
        //    {
        //        Console.Write("Client page load not postback");
        //        ClientCreditGridView.DataSource = CreateDataSource();
        //        ClientCreditGridView.DataBind();
        //    }
        //}

        void OnCalculateCredit(){
            Console.WriteLine("On credit calculate clicked");
        }

        IEnumerable CreateDataSource()
        {
            
            var query =
                from c in DatabaseConnection.Instance.creditsDB.AsQueryable()
                where c != null
                select c;
            Console.WriteLine("Trying to query" + query.Count());
            return query.AsEnumerable();
        }
    }
}
