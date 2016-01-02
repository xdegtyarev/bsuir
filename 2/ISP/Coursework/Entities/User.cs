using System;
using MongoDB.Bson;
namespace Kickstart
{
	public class User
	{
		public ObjectId Id{get;set;}
		public string email{get;set;}
		public string password{get;set;}
		public ObjectId role{get;set;}
	}
}

