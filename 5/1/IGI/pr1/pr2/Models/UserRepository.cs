using System;
using System.Collections.Generic;
using System.Dynamic;
using MongoDB.Bson;
using MongoDB.Driver;

namespace pr2.Models
{
    public class UserRepository: IRepository<User>
    {
        FilterDefinitionBuilder<User> filterBuilder;
        UpdateDefinitionBuilder<User> updateBuilder;
        MongoClient mongoDBClient;
        IMongoDatabase database;
        IMongoCollection<User> users;

        public UserRepository(string connection)
        {
            if (String.IsNullOrEmpty(connection))
            {
                connection = Program.DefaultMongoDBConnection;
            }
            mongoDBClient = new MongoClient(connection);
            database = mongoDBClient.GetDatabase("IGI");
            users = database.GetCollection<User>("Users");
            filterBuilder = Builders<User>.Filter;
            updateBuilder = Builders<User>.Update;
            Console.WriteLine("User Repository Created");
        }

        public List<User> GetAll()
        {
            var result = users.Find(filterBuilder.Ne(user => user.id, null));
            return result.ToList();
        }

        public User Create(User item)
        {
            item.id = ObjectId.GenerateNewId().ToString();
            item.modification = DateTime.UtcNow;
            users.InsertOne(item);
            return item;
        }

        public User Read(string id)
        {
            var result = users.Find(filterBuilder.Eq(record => record.id, id));
            return result.FirstOrDefault();
        }

        public bool Update(string id, User item)
        {
            var result = users.UpdateOne(filterBuilder.Eq(user => user.id, id),
                             updateBuilder
                                         .Set(user => user.modification, DateTime.UtcNow)
                                         .Set(user => user.email, item.email));
            return result.IsAcknowledged && result.IsModifiedCountAvailable && result.MatchedCount > 0 && result.ModifiedCount > 0;
        }

        public bool Delete(string id)
        {
            var result = users.DeleteOne(filterBuilder.Eq(user => user.id, id));
            return result.IsAcknowledged && result.DeletedCount > 0;
        }

        public User FindUserByMail(string email)
        {
            var result = users.Find(filterBuilder.Eq(user => user.email, email));
            return result.FirstOrDefault();
        }
    }
}
