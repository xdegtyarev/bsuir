using System;
using System.ComponentModel.DataAnnotations;
using MongoDB.Bson.Serialization.Attributes;

namespace pr2.Models
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

        [Required]
        [StringLength(60, MinimumLength = 3)]
        public string name { get; set; }

        [Required]
        [StringLength(60, MinimumLength = 3)]
        public string password { get; set; }

        [Required]
        [StringLength(60, MinimumLength = 3)]
        [RegularExpression(@"\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,6}\b")]
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
