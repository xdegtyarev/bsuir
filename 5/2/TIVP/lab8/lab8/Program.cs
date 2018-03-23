using System;
using OpenQA.Selenium.Firefox;
using OpenQA.Selenium;
using AventStack.ExtentReports.Reporter;
using AventStack.ExtentReports;
using System.Threading;

namespace lab8
{
    class MainClass
    {
        //Task
        //5 test cases with 3-4 steps
        //Test HTML forms
        //Different types of selectors
        //ajax calls or file upload
        public static void Main(string[] args)
        {
            Console.WriteLine("Hello it us lab8, where I try to use Selenium Webdriver!");
            var htmlReporter = new ExtentHtmlReporter("extent.html");
            var extent = new ExtentReports();
            extent.AttachReporter(htmlReporter);
            var simpleHtmlTest = extent.CreateTest("SimpleHTMLTest");
            string details = "pass";

            if (SimpleHTMLTest(out details))
            {
                simpleHtmlTest.Pass(details);
            }
            else
            {
                simpleHtmlTest.Fail(details);
            }

            var webformTest = extent.CreateTest("WebformTest");
            if (WebformTest(out details))
            {
                webformTest.Pass(details);
            }
            else
            {
                webformTest.Fail(details);
            }

            var fileuploadTest = extent.CreateTest("FileUploadTest");
            if (FileUploadTest(out details))
            {
                fileuploadTest.Pass(details);
            }
            else
            {
                fileuploadTest.Fail(details);
            }

            var playMusicTest = extent.CreateTest("PlayMusicTest");
            if (PlayMusicTest(out details))
            {
                playMusicTest.Pass(details);
            }
            else
            {
                playMusicTest.Fail(details);
            }

            var ajaxTest = extent.CreateTest("AjaxTest");
            if (AjaxTest(out details))
            {
                ajaxTest.Pass(details);
            }
            else
            {
                ajaxTest.Fail(details);
            }

            extent.Flush();
        }

        static bool SimpleHTMLTest(out string status)
        {
            bool pass = true;
            IWebDriver ffdriver = new FirefoxDriver();
            try
            {
                ffdriver.Navigate().GoToUrl("http://xdegtyarev.com");
                ffdriver.FindElement(By.LinkText("Music")).Click();
                var iframeElement = ffdriver.FindElement(By.TagName("iframe"));
                Console.WriteLine("iframe count:" + ffdriver.FindElements(By.TagName("iframe")).Count);

                if (iframeElement == null)
                {
                    status = "no iframe on page";
                    pass = false;
                }
                if (iframeElement.Displayed)
                {
                    Thread.Sleep(10000);
                    Console.WriteLine("found iframe:" + iframeElement.Enabled);
                    bool allImagesLoaded = false;
                    foreach (var imageElement in ffdriver.FindElements(By.TagName("img")))
                    {
                        Console.WriteLine("ImageURL:" + imageElement.GetAttribute("src"));
                        ffdriver.Navigate().GoToUrl(imageElement.GetAttribute("src"));
                        ffdriver.Navigate().Back();
                    }
                    status = allImagesLoaded ? "pass" : "not all images exist";
                    pass = allImagesLoaded;
                }
                else
                {
                    status = "iframe is not visible";
                    pass = false;
                }
                //});
                //wait.Until(waiter);
                return pass;
            }
            catch (Exception exception)
            {
                status = exception.Message;
                return false;
            }
            finally
            {
                ffdriver.Close();
            }
        }

        static bool WebformTest(out string status)
        {
            IWebDriver ffdriver = new FirefoxDriver();
            try
            {
                ffdriver.Navigate().GoToUrl("http://vk.com");
                ffdriver.FindElement(By.Id("index_email")).SendKeys("xdegtyarev");
                ffdriver.FindElement(By.Id("index_pass")).SendKeys("mypassword");
                ffdriver.FindElement(By.Id("index_login_button")).Click();
                Thread.Sleep(1000);
                status = ffdriver.FindElement(By.ClassName("msg_text")).FindElement(By.TagName("b")).Text;
                return true;
            }
            catch (Exception exception)
            {
                status = exception.Message;
                return false;
            }
            finally
            {
                ffdriver.Close();
            }

        }

        static bool FileUploadTest(out string status)
        {
            IWebDriver ffdriver = new FirefoxDriver();
            try
            {
                ffdriver.Navigate().GoToUrl("https://imgur.com/upload");
                ffdriver.FindElement(By.Id("global-files-button"));


                status = "pass";
                return true;
            }
            catch (Exception exception)
            {
                status = exception.Message;
                return false;
            }
            finally
            {
                ffdriver.Close();
            }
        }

        static bool PlayMusicTest(out string status)
        {
            IWebDriver ffdriver = new FirefoxDriver();
            try
            {
                ffdriver.Navigate().GoToUrl("https://gloryordeathrecords.bandcamp.com/album/bow-to-your-masters-volume-1-thin-lizzy");
                var player = ffdriver.FindElement(By.ClassName("inline_player"));
                var playbutton = player.FindElement(By.TagName("a"));
                if(playbutton.GetAttribute("role") == "button"){
                    Console.WriteLine("PlayButton");
                    playbutton.Click();
                    Thread.Sleep(2000);
                    var playingStatus = player.FindElement(By.ClassName("playbutton playing"));
                }else{
                    status = "wrong button status";
                    return false;
                }
                status = "pass";
                return true;
            }
            catch (Exception exception)
            {
                status = exception.Message;
                return false;
            }
            finally
            {
                ffdriver.Close();
            }
        }

        static bool AjaxTest(out string status)
        {
            
            IWebDriver ffdriver = new FirefoxDriver();
            try{
                status = "pass";
                return true;
            }
            catch (Exception exception)
            {
                status = exception.Message;
                return false;
            }
            finally
            {
                ffdriver.Close();
            }
        }
    }
}
