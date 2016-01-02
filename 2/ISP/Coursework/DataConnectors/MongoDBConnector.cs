using System;
using MongoDB.Bson;
using MongoDB.Driver;
using MongoDB.Driver.Builders;
using MongoDB.Driver.GridFS;
using MongoDB.Driver.Linq;

namespace Kickstart
{
	public class MongoDBConnector
	{
		String connectionString = "mongodb://localhost";
		String DBName = "KickstartDB";
		MongoClient client;
		MongoServer server;

		public MongoDBConnector ()
		{
			client = new MongoClient (connectionString);
			server = client.GetServer ();
		}

		public MongoDatabase GetMongoDatebase ()
		{
			return server.GetDatabase (DBName);
		}

		public MongoCollection<T> GetCollection<T> (string name)
		{
			return GetMongoDatebase ().GetCollection<T> (name);
		}
	}
}

