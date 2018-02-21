using System;
using System.Web;
using System.Web.UI;

namespace pr1
{

    public partial class Profile : System.Web.UI.Page
    {
        protected void Page_Init(object sender, EventArgs e)
        {
            Console.WriteLine("[Profile]Init");
            Console.WriteLine("[Profile]Username:" + Global.Instance.userController.CurrentUserName());

            var user = Global.Instance.userController.GetCurrentUser();
            if (user != null)
            {
                ProfileId.Text = user.id;
                ProfileName.Text = user.name;
                ProfileEmail.Text = user.email;
            }
        }
    }
}
