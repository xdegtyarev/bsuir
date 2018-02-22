using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using MongoDB.Bson.Serialization.Attributes;

namespace pr2.Models
{
    public class Record
    {
        [BsonId]
        public string id { get; set; }
        public DateTime modification { get; set; }

        [StringLength(60, MinimumLength = 1)]
        [Required]
        public string title { get; set; }
        [StringLength(60, MinimumLength = 1)]
        [Required]
        public string artist { get; set; }
        [StringLength(60, MinimumLength = 1)]
        [Required]
        public string label { get; set; }

        public string coverURL { get; set; }

        [Display(Name = "Release Date")]
        [DataType(DataType.Date)]
        public DateTime releaseDate;

        [StringLength(60, MinimumLength = 3)]
        public string genre;

        [StringLength(60, MinimumLength = 3)]
        public string style;

        [StringLength(60, MinimumLength = 3)]
        public string format;

        [StringLength(60, MinimumLength = 3)]
        public string country;

        public List<string> tracklist;

        public int ratings;

        public bool IsValid(){
            return true;
        }

        public Record(){}
    }
}
