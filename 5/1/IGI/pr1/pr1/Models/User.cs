using System;
using MongoDB.Bson.Serialization.Attributes;

namespace pr1.Models
{
    public enum UserRole{
        Anon = -1,
        Member = 0,
        Moderator,
        Admin
    }

    public class User
    {
        [BsonId]
        public string id { get; set; }

        public string name { get; set; }
        public string password { get; set; }
        public string email { get; set; }
        public int role { get; set; }

        public DateTime modification { get; set; }
        public DateTime lastLogin { get; set; }
        public DateTime lastActivity { get; set; }

        public User()
        {
            
        }
    }
}
