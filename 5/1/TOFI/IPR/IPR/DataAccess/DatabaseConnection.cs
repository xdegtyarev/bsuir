using System;
using MongoDB;
using MongoDB.Driver;
using IPR.Entity;
namespace IPR.DataAccess
{
    public class DatabaseConnection
    {
        public static DatabaseConnection Instance;
        MongoClient client;
        IMongoDatabase database;
        public IMongoCollection<BankCredit> creditsDB;
        IMongoCollection<BankClient> clientsDB;
        IMongoCollection<BankEmployee> employeeDB;
        public DatabaseConnection()
        {
            if (Instance == null){
                Instance = this;
                client = new MongoClient("mongodb://localhost:27017");
                database = client.GetDatabase("TOFIBank");
                Console.WriteLine("MongoConnected" + " : " + database.DatabaseNamespace);
                creditsDB = database.GetCollection<BankCredit>("Credits");
                clientsDB = database.GetCollection<BankClient>("Clients");
                employeeDB = database.GetCollection<BankEmployee>("Employees");
            }
        }
    }
}
