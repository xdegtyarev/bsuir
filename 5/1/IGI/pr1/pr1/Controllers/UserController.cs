using System;
using pr1.Models;
namespace pr1.Controllers
{
    public class UserController
    {
        User currentUser;

        public void SetCurrentUser(string userId){
            if (!string.IsNullOrEmpty(userId))
            {
                currentUser = Global.Instance.userRepository.Read(userId);
            }
        }

        public void ResetCurrentUser(){
            currentUser = null;
            Global.Instance.Session["CurrentUserId"] = null;
        }

        public bool IsCurrentUserAnon(){
            return currentUser != null;
        }

        public UserRole CurrentUserRole(){
            return currentUser != null ? (UserRole)currentUser.role : UserRole.Anon;
        }

        public string CurrentUserName(){
            return currentUser != null ? currentUser.name : "Anon";
        }    

        public User GetCurrentUser(){
            return currentUser;
        }
    }
}
