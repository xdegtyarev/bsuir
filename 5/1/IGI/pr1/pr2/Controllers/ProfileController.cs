using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;

namespace pr2.Controllers
{
    public class ProfileController : Controller
    {
        // GET: /<controller>/
        public IActionResult Index(string id)
        {
            if (string.IsNullOrEmpty(id))
            {
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

        //Get: /account/register
        public string Register()
        {
            return "register account";
        }

        public string Delete()
        {
            return "delete account";
        }


    }
}
