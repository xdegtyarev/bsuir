using System;
using System.Collections.Generic;
using System.Web;
using System.Web.UI;
namespace pr1
{
    public partial class MusicdropMaster : System.Web.UI.MasterPage
    {
        List<string> searchTypes = new List<string> {"Title","Artist","Label","Year"};
        protected void Page_Load(object sender, EventArgs e)
        {
            Console.WriteLine("[Master]Load");
        }

        protected void Page_Init(object sender, EventArgs e)
        {
            Console.WriteLine("[Master]Init");
            MasterSearchDropdown.Items.Clear();
            foreach(var o in searchTypes){
                MasterSearchDropdown.Items.Add(o);
            }

            switch (Global.Instance.userController.CurrentUserRole())
            {
                case Models.UserRole.Anon:
                    Console.WriteLine("[Master]Setting anon view");
                    TopBarMultiView.ActiveViewIndex = 0;
                    NavigationBarView.ActiveViewIndex = 0;
                    break;
                case Models.UserRole.Member:
                    Console.WriteLine("[Master]Setting member view");
                    MasterLoggedViewCurrentUser.Text = Global.Instance.userController.CurrentUserName();
                    NavigationBarView.ActiveViewIndex = 1;
                    TopBarMultiView.ActiveViewIndex = 1;
                    break;
                case Models.UserRole.Moderator:
                    TopBarMultiView.ActiveViewIndex = 1;
                    NavigationBarView.ActiveViewIndex = 1;
                    break;
                case Models.UserRole.Admin:
                    TopBarMultiView.ActiveViewIndex = 1;
                    NavigationBarView.ActiveViewIndex = 1;
                    break;
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }

        public void ThemeToggle_Click(object sender, EventArgs args){
            Console.WriteLine("[Master]theme toggle clicked");
            if (Session["CurrentThemeId"] != null)
            {
                Session["CurrentThemeId"] = (Session["CurrentThemeId"] as String == "Red") ? "Yellow" : "Red";
            }else{
                Session["CurrentThemeId"] = "Red";
            }
            Response.Redirect(Request.RawUrl);
        }

        public void LoginButton_Click(object sender, EventArgs args)
        {
            var email = MasterLoginEmail.Text;
            var password = MasterLoginPassword.Text;
            var user = Global.Instance.userRepository.FindUserByMail(email);
            if (user != null && user.password == password)
            {
                Global.Instance.userController.SetCurrentUser(user.id);
                Console.WriteLine("[Master]Login success " + Global.Instance.userController.CurrentUserName());
                Response.Redirect(Request.RawUrl);
            }
            else
            {
                Console.WriteLine("[Master]Failed to login");
            }
        }

        public void MasterLoggedViewLogOutButton_Click(object sender, EventArgs args){
            Console.WriteLine("[Master]LogOut clicked");
            Global.Instance.userController.ResetCurrentUser();
            Response.Redirect(Request.RawUrl);
        }

        public void AddRecord_Clicked(object sender, EventArgs args){
            Console.WriteLine("[Master]AddRecordclicked");
            Response.Redirect("/Record.aspx?edit=1");
        }

        public void Dashboard_Clicked(object sender, EventArgs args){
            Console.WriteLine("[Master]Dashboard_Clicked");
            Response.Redirect("/Profile.aspx");
        }

        public void Logo_Clicked(object sender, EventArgs args){
            Console.WriteLine("[Master]LogoClicked");
            Response.Redirect("/Default.aspx");
        }

        public void Main_Clicked(object sender, EventArgs args){
            Console.WriteLine("[Master]Main_Clicked");
            Response.Redirect("/Default.aspx");
        }

        public void Search_Clicked(object sender, EventArgs args){
            Console.WriteLine("[Master]Search_Clicked");
            Response.Redirect("/Default.aspx?search="+MainSearchbox.Text+"&searchtype="+MasterSearchDropdown.SelectedIndex);
        }
    }
}
