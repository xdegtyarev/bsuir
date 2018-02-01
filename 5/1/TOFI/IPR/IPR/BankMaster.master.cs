using System;
using System.Web;
using System.Web.UI;
namespace IPR
{
    public partial class BankMaster : System.Web.UI.MasterPage
    {
        void Page_Load() { 
            Console.WriteLine("Master page loaded");
        }
    }
}
