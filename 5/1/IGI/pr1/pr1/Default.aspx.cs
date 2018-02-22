using System;
using System.Web;
using System.Web.UI;
using System.Collections.Generic;
using pr1.Models;

namespace pr1
{
    enum SearchType{
        Title,
        Artist,
        Label,
        Year
    }

    public partial class Default : System.Web.UI.Page
    {
        protected void Page_PreInit(){
            Console.WriteLine("[Default]PreInit");
            Console.WriteLine(Session["CurrentThemeId"] as String);
            Page.Theme = Session["CurrentThemeId"] as String;
        }

        protected void Page_Load(object sender, EventArgs e)
        {
            Console.WriteLine("[Default]Load");
            Master.Page.Header.Title = "Musicdrop - Main";

            var recordsToDisplay = new List<Models.Record>();
            string searchString = Request.QueryString["search"];
            if(string.IsNullOrEmpty(searchString)){
                DefaultSearchReqest.Enabled = false;
                DefaultSearchReqest.Text = "";
                Console.WriteLine("[Default]empty search string");
                recordsToDisplay = Global.Instance.recordRepository.GetAll();
            }else{
                DefaultSearchReqest.Enabled = true;
                Console.WriteLine("[Default]nonempty search string: " + searchString);
                DefaultSearchReqest.Text = "Search results for: " + searchString;
                var searchType = SearchType.Title;
                int temp = 0;
                if(!string.IsNullOrEmpty(Request.QueryString["searchtype"])){
                    searchType = int.TryParse(Request.QueryString["searchtype"], out temp) ? (SearchType)temp : SearchType.Title;
                }
                switch (searchType) {
                    case SearchType.Title:
                        recordsToDisplay = Global.Instance.recordRepository.SearchTitle(searchString);
                        break;
                    case SearchType.Artist:
                        recordsToDisplay = Global.Instance.recordRepository.SearchArtist(searchString);
                        break;
                    case SearchType.Label:
                        recordsToDisplay = Global.Instance.recordRepository.SearchLabel(searchString);
                        break;
                    case SearchType.Year:
                        int year = int.TryParse(searchString, out temp) ? temp : 1970;
                        recordsToDisplay = Global.Instance.recordRepository.SearchYear(year);
                        break;
                    default:
                        recordsToDisplay = Global.Instance.recordRepository.GetAll();
                        break;
                }
            }
            Console.WriteLine(recordsToDisplay.Count);
            if (recordsToDisplay != null && recordsToDisplay.Count > 0)
            {
                DefaultPageRecordList.DataSource = recordsToDisplay;
                DefaultPageRecordList.DataBind();
            }
        }

        protected void Page_Init(object sender, EventArgs e){
            Console.WriteLine("[Default]Init");
            Console.WriteLine("[Default]Username:" + Global.Instance.userController.CurrentUserName());

            switch (Global.Instance.userController.CurrentUserRole())
            {
                case UserRole.Anon:
                    DefaultPageMultiView.ActiveViewIndex = 0;
                    break;
                case UserRole.Member:
                    DefaultPageMultiView.ActiveViewIndex = 1;
                    DefaultPageMemberViewGreeting.Text = "Hello " +  Global.Instance.userController.CurrentUserName() + "!";
                    break;
                case UserRole.Moderator:
                    break;
                case UserRole.Admin:
                    break;
            }
        }

        public void RegisterButton_Click(object sender, EventArgs args)
        {
            Console.WriteLine("[Default]Create account");

            User user = new User
            {
                name = RegisterUsername.Text,
                email = RegisterEmail.Text,
                password = RegisterPassword.Text,
                role = 0
            };

            if (ValidateUserRegistration(user)){
                user = Global.Instance.userRepository.Create(user);
                Console.WriteLine("[Default]User created " + user.id);
                Session["CurrentUserId"] = user.id;
                Global.Instance.userController.SetCurrentUser(user.id);
                Response.Redirect(Request.RawUrl);
            }else{
                RegisterEmailValidator.Text = "Such email is already used";
                RegisterEmailValidator.EnableViewState = true;
            }
        }

        public bool ValidateUserRegistration(User user)
        {
            var userWithSameMail = Global.Instance.userRepository.FindUserByMail(user.email);
            return userWithSameMail == null;
        }

        public string GetCoverArt(object url){
            return "/Content/" + (string.IsNullOrEmpty(url as string) ? "default-release.png" : (string)url);
        }
    }
}
