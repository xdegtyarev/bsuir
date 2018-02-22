using System;
using System.Collections.Generic;
using MongoDB.Bson.Serialization.Attributes;

namespace pr2.Models
{
    public class Record
    {
        [BsonId]
        public string id { get; set; }
        public DateTime modification { get; set; }

        public string title { get; set; }
        public string artist { get; set; }
        public string label { get; set; }

        public string coverURL { get; set; }

        public DateTime releaseDate;

        public string genre;
        public string style;
        public string format;
        public string country;
        public List<string> tracklist;

        public int ratings;

        public bool IsValid(){
            return true;
        }

        public Record(){}
    }
}
