using System;
using System.Web;
using System.Web.UI;
using MongoDB.Bson;
using MongoDB.Driver.Builders;

namespace Kickstart
{
	public partial class Kickstart : System.Web.UI.MasterPage
	{
		User cachedUser;

		void Page_Load (Object sender, EventArgs e)
		{
			if (!IsPostBack) {
				MongoDBConnector connector = new MongoDBConnector ();
				var collection = connector.GetCollection<User> ("Users");
				var query = Query.EQ ("id", (ObjectId)Cache.Get ("currentUser"));
				var result = collection.FindOne (query);
				if (result != null) {
					cachedUser = result;
					ContentPlaceHolder.Page.Title = cachedUser.email;
				} else {
					//Response.Redirect("/Default.aspx");

				}
			}
		}
	}
}

