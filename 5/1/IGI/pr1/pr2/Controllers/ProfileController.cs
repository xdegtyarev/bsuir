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
                user = Program.userRepository.Create();
            }else{
                user = Program.userRepository.Read(id);
                if(user==null){
                    return NotFound();
                }
            }
            return View(user);
        }

        [HttpPost]
        public IActionResult Edit([Bind("id,name,email,password")] User user)
        {
            if(user.IsValid()){
                if (Program.userRepository.FindUserByMail(user.email) != null)
                {
                    if (Program.userRepository.Update(user.id, user))
                    {
                        ViewData["Status"] = "Success updating redirecting";
                        loggedId = user.id;
                        return Redirect("/Profile/Index/" + user.id);
                    }
                    else
                    {
                        ViewData["Status"] = "Failed updating redirecting";
                        return View(user);
                    }
                }else{
                    if(Program.userRepository.Create(user)!=null){
                        ViewData["Status"] = "Success creating redirecting";
                        loggedId = user.id;
                        return Redirect("/Profile/Index/" + user.id);
                    }else{
                        ViewData["Status"] = "Failed creating redirecting";
                        return View(user);
                    }
                }
            }else{
                ViewData["Status"] = "Success updating redirecting";
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
