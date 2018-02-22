using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text.Encodings.Web;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using pr2.Models;

namespace pr2.Controllers
{
    public class HomeController : Controller
    {
        enum SearchType
        {
            Title,
            Artist,
            Label,
            Year
        }

        [HttpGet]
        public IActionResult Index(string search, int searchtype=1)
        {
            var recordsToDisplay = new List<Record>();
            SearchType searchType = (SearchType)searchtype;
            if (string.IsNullOrEmpty(search))
            {
                recordsToDisplay = Program.recordRepository.GetAll();
            }
            else
            {
                string verifiedString = HtmlEncoder.Default.Encode(search);
                switch (searchType)
                {
                    case SearchType.Title:
                        ViewData["Search"] = search + " of type Title";
                        recordsToDisplay = Program.recordRepository.SearchTitle(verifiedString);
                        break;
                    case SearchType.Artist:
                        ViewData["Search"] = search + " of type Artist";
                        recordsToDisplay = Program.recordRepository.SearchArtist(verifiedString);
                        break;
                    case SearchType.Label:
                        ViewData["Search"] = search + " of type Label";
                        recordsToDisplay = Program.recordRepository.SearchLabel(verifiedString);
                        break;
                    case SearchType.Year:
                        ViewData["Search"] = search + " of type Year";
                        int temp = 0;
                        int year = int.TryParse(verifiedString, out temp) ? temp : 1970;
                        recordsToDisplay = Program.recordRepository.SearchYear(year);
                        break;
                    default:
                        recordsToDisplay = Program.recordRepository.GetAll();
                        break;
                }
            }
            return View(recordsToDisplay);
        }

        [HttpPost]
        public IActionResult Index(string search, string searchtype, bool notused=false)
        {
            switch(searchtype){
                case "Title":
                    return Redirect("?search=" + search + "&searchtype=0");
                case "Artist":
                   return Redirect("?search=" + search + "&searchtype=1");
                case "Label":
                    return Redirect("?search=" + search + "&searchtype=2");
                case "Year":
                    return Redirect("?search=" + search + "&searchtype=3");
                default:
                    return Redirect("?search=" + search + "&searchtype=0");
            }

        }

        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }
    }
}
