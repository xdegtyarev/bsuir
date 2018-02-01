using System;
using MongoDB.Bson.Serialization.Attributes;
using MongoDB.Bson.Serialization.IdGenerators;

namespace IPR.Entity
{
    public class BankClient
    {
        [BsonId(IdGenerator = typeof(StringObjectIdGenerator))]
        public string ID { get; set; }

        public string name{ get; set; }
        public string email{ get; set; }
        public string phone{ get; set; }

        public int age{ get; set; }
        public int salary{ get; set; }
        public int marrigestatus{ get; set; }
        public bool hasKids{ get; set; }
        public bool decret{ get; set; }
        public int education{ get; set; }
        public int lastWorkTerm{ get; set; }

        public BankClient()
        {
        }
    }
}
