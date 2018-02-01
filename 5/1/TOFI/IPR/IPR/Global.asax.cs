using System;
using System.Web;
using System.Web.Http;
using System.Web.Security;
using IPR.DataAccess;

namespace IPR
{
    public class Global : HttpApplication
    {
        public DatabaseConnection dbconnection;
        protected void Application_Start()
        {
            Console.WriteLine("Starting APP");
            GlobalConfiguration.Configure(WebApiConfig.Register);
            //start db connection
            dbconnection = new DatabaseConnection();
        }
    }
}
