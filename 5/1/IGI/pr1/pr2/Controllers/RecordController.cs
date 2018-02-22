using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using pr2.Models;

namespace pr2.Controllers
{
    public class RecordController : Controller
    {
        // GET: /<controller>/
        public IActionResult Index(string id)
        {
            if(string.IsNullOrEmpty(id)){
                return NoContent();
            }else{
                var record = Program.recordRepository.Read(id);
                if(record!=null){
                    return View(record);
                }else{
                    return NoContent();
                }
            }
        }

        public IActionResult Edit(string id)
        {
            Record record = (id == null ? Program.recordRepository.Create() : Program.recordRepository.Read(id)) ?? Program.recordRepository.Create();
            record.coverURL = string.IsNullOrEmpty(record.coverURL) ? "~/images/default_release.png" : record.coverURL;
            return View(record);
        }

        [HttpPost]
        public IActionResult Edit(string id,[Bind("title,artist,label,releaseDate,genre,style,format,country")] Record record){
            if(id!=record.id){
                return NotFound();
            }
            if(record.IsValid()){
                if(Program.recordRepository.Update(id,record)){
                    ViewData["Status"] = "Success updating redirecting";
                    return Redirect("/Record/Index/"+record.id);
                }else{
                    ViewData["Status"] = "Failed updating redirecting";
                    return View(record);
                }

            }else{
                return View(record);
            }
        }


        [HttpDelete]
        public IActionResult Delete(string id){
            if (id != null)
            {
                if (Program.recordRepository.Delete(id))
                {
                    Console.WriteLine("Success");
                }else{
                    Console.WriteLine("Delete failed");
                }
            }
            return Redirect("Index");
        }
    }
}
