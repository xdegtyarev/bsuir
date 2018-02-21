using System;
using System.Collections.Generic;
using MongoDB.Bson;
using MongoDB.Driver;

namespace pr1.Models
{
    public class RecordRepository: IRepository<Record>
    {
        FilterDefinitionBuilder<Record> filterBuilder;
        UpdateDefinitionBuilder<Record> updateBuilder;

        MongoClient mongoDBClient;
        IMongoDatabase database;
        IMongoCollection<Record> records;

        public RecordRepository(string connection)
        {
            if(String.IsNullOrEmpty(connection)){
                connection = Global.DefaultMongoDBConnection;
            }
            mongoDBClient = new MongoClient(connection);
            database = mongoDBClient.GetDatabase("IGI");
            records = database.GetCollection<Record>("Records");
            filterBuilder = Builders<Record>.Filter;
            updateBuilder = Builders<Record>.Update;
            Console.WriteLine("Record Repository Created");
        }

        public List<Record> GetAll()
        {
            var result = records.Find(filterBuilder.Ne(record => record.id, null));
            return result.ToList();
        }

        public Record Create(Record record)
        {
            record.id = string.IsNullOrEmpty(record.id) ? ObjectId.GenerateNewId().ToString() : record.id;
            record.modification = DateTime.UtcNow;
            records.InsertOne(record);
            return record;
        }

        public Record Create()
        {
            var record = new Record();
            record.id = ObjectId.GenerateNewId().ToString();
            record.modification = DateTime.UtcNow;
            return record;
        }

        public Record Read(string id)
        {
            var result = records.Find(filterBuilder.Eq(record => record.id, id));
            return result.Count()>0 ? result.First() : null;
        }

        public List<Record> SearchTitle(string title)
        {
            Console.WriteLine("SearchingTitle:" + title);
            var result = records.Find(filterBuilder.Regex(record=>record.title,new BsonRegularExpression(title, "i")));
            return result.ToList();
        }

        public List<Record> SearchArtist(string artist)
        {
            Console.WriteLine("SearchingArtist:" + artist);
            var result = records.Find(filterBuilder.Regex(record=>record.artist,new BsonRegularExpression(artist, "i")));
            return result.ToList();
        }

        public List<Record> SearchLabel(string label)
        {
            Console.WriteLine("SearchingLabel:" + label);
            var result = records.Find(filterBuilder.Regex(record=>record.label,new BsonRegularExpression(label, "i")));
            return result.ToList();
        }

        public List<Record> SearchYear(int year)
        {
            Console.WriteLine("SearchingYear:" + year);
            var result = records.Find(filterBuilder.Eq(record => record.releaseDate.Year, year));
            return result.ToList();
        }

        public bool Update(string id, Record item)
        {
            if(Read(id)==null){
                return Create(item) != null;
            }else{
                var result = records.UpdateOne(filterBuilder.Eq(record => record.id, id),
                                   updateBuilder
                                               .Set(record => record.modification, DateTime.UtcNow)
                                               .Set(record => record.title, item.title)
                                               .Set(record => record.artist, item.artist)
                                               .Set(record => record.label, item.label)
                                               .Set(record => record.coverURL, item.coverURL)
                                               .Set(record => record.releaseDate, item.releaseDate)
                                               .Set(record => record.genre, item.genre)
                                               .Set(record => record.style, item.style)
                                               .Set(record => record.format, item.format)
                                               .Set(record => record.country, item.country)
                                               .Set(record => record.tracklist, item.tracklist));
                return result.IsAcknowledged && result.IsModifiedCountAvailable && result.MatchedCount > 0 && result.ModifiedCount > 0;
            }
        }

        public bool UpdateRatings(string id, int score){
            var result = records.UpdateOne(filterBuilder.Eq(record => record.id, id),updateBuilder.Set(record => record.ratings, score));
            return result.IsAcknowledged && result.IsModifiedCountAvailable && result.MatchedCount > 0 && result.ModifiedCount > 0;
        }

        public bool Delete(string id)
        {
            var result = records.DeleteOne(filterBuilder.Eq(record => record.id,id));
            return result.IsAcknowledged && result.DeletedCount > 0;
        }
    }
}
