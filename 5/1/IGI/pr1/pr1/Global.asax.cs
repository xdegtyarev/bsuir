using System;
using System.Web;
using MongoDB.Driver;
using pr1.Controllers;
using pr1.Models;

namespace pr1
{
    public class Global : HttpApplication
    {
        public static Global Instance;
        public const string DefaultMongoDBConnection = "mongodb://localhost:27017";
        public UserController userController;
        public UserRepository userRepository;
        public RecordRepository recordRepository;
        protected void Application_Start()
        {
            Instance = this;
            Console.WriteLine("Starting APP");
            userRepository = new UserRepository(DefaultMongoDBConnection);
            recordRepository = new RecordRepository(DefaultMongoDBConnection);
            userController = new UserController();
        }

        public void Session_OnStart()
        {
            Console.WriteLine("Session start");
        }
    }
}
