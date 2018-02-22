using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore;
using Microsoft.AspNetCore.Hosting;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;
using pr2.Models;

namespace pr2
{
    public class Program
    {
        public const string DefaultMongoDBConnection = "mongodb://localhost:27017";
        //public static UserController userController;
        public static UserRepository userRepository;
        public static RecordRepository recordRepository;

        public static void Main(string[] args)
        {
            userRepository = new UserRepository(DefaultMongoDBConnection);
            recordRepository = new RecordRepository(DefaultMongoDBConnection);
            BuildWebHost(args).Run();
        }

        public static IWebHost BuildWebHost(string[] args) =>
            WebHost.CreateDefaultBuilder(args)
                .UseStartup<Startup>()
                .Build();
    }
}
