using System;
using MongoDB.Bson;

namespace Kickstart
{
	public class Role
	{
		public ObjectId Id{ get; set; }

		public String type{ get; set; }
	}
}

