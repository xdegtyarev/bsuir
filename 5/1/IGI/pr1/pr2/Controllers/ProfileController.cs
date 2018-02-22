using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using pr2.Models;

namespace pr2.Controllers
{
    public class ProfileController : Controller
    {
        public static string loggedId;

        // GET: /<controller>/
        public IActionResult Index(string id)
        {
            if (string.IsNullOrEmpty(id))
            {
                if(!string.IsNullOrEmpty(loggedId)){
                    var user = Program.userRepository.Read(loggedId);
                    if(user!=null){
                        return View(user);
                    }else{
                        return NoContent();
                    }
                }
                return View();
            }
            else
            {
                var user = Program.userRepository.Read(id);
                if (user != null)
                {
                    return View(user);
                }
                else
                {
                    return NoContent();
                }
            }
        }

        [HttpPost]
        public IActionResult Index(string email,string password){
            if (string.IsNullOrEmpty(email))
            {
                return Redirect("Error");
            }else{
                var user = Program.userRepository.FindUserByMail(email);
                if (user!=null){
                    if (user.password.Equals(password))
                    {
                        loggedId = user.id;
                        return Redirect("/Profile/Index/" + user.id + "/");
                    }else{
                        return Redirect("Error");
                    }
                }else{
                    return Redirect("Error");
                }
            }
        }

        //Get: /account/register
        public IActionResult Edit(string id)
        {
            User user = null;
            if(string.IsNullOrEmpty(id)){
                //create new
            }else{
                user = Program.userRepository.Read(id);
                if(user==null){
                    //create new
                }
            }

            return View(user);
        }

        [HttpPost]
        public IActionResult Edit(string id, [Bind("id,name,email,password")] User user, string create)
        {
            if(id!=user.id){
                return NotFound();
            }
            if(user.IsValid()){
                if(Program.userRepository.Update(id,user)){
                    return Redirect("Index");
                }else{
                    return View(user);
                }
            }else{
                return View(user);
            }
        }

        [HttpPost]
        public IActionResult Logout(){
            loggedId = string.Empty;
            return Redirect("/Profile/");
        }

        public string Delete()
        {
            return "delete account";
        }


    }
}
