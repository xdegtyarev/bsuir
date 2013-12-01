using System;
using System.Web;
using System.Web.UI;
using System.Collections.Generic;
using System.Security.Cryptography;
using MongoDB.Driver.Builders;
using MongoDB.Bson;

namespace Kickstart
{
	public partial class Default : System.Web.UI.Page
	{

		public void OnRegisterButtonClick (object sender, EventArgs e)
		{
			LoginStatus.Text = String.Empty;
			EmailFieldRegExpValidator.Validate ();
			if (EmailField.Text.Length > 0 && EmailFieldRegExpValidator.IsValid) {
				if (EmailNotRegistered (EmailField.Text)) {
					PasswordFieldRegExpValidator.Validate ();
					if (PasswordField.Text.Length > 0 && PasswordFieldRegExpValidator.IsValid) {
						LoginFormMultiView.ActiveViewIndex = 1;
					} else {
						LoginStatus.Text = "Password does not match requirements";
					}
				} else {
					LoginStatus.Text = "Account with such email is already registered";
				}
			} else {
				LoginStatus.Text = "Email does not match requirements";
			}
			return;
		}

		bool EmailNotRegistered (string mail)
		{
			MongoDBConnector dbConnector = new MongoDBConnector ();
			var collection = dbConnector.GetCollection<User> ("Users");
			var q = Query.EQ ("email", mail);
			return collection.Find (q).Count () == 0;
		}

		public void OnLoginButtonClick (object sender, EventArgs e)
		{
			LoginStatus.Text = String.Empty;
			if (EmailFieldRegExpValidator.IsValid) {
				if (PasswordFieldRegExpValidator.IsValid) {
					if (VerifyCredentials (EmailField.Text, PasswordField.Text)) {
						Cache.Insert ("currentUser", GetUserByEmail (EmailField.Text));
						LoginStatus.Text = "Success";
						SaveCookies ();
						Response.Redirect ("/Profile.aspx");
					} else {
						LoginStatus.Text = "Email and password pair is incorrect. Try Again";
					}
				} else {
					LoginStatus.Text = "Wrong Password. Try Again";
				}
			} else {
				LoginStatus.Text = "Wrong Email";
			}
		}
			
		ObjectId GetUserByEmail (string email)
		{
			MongoDBConnector dbConnector = new MongoDBConnector ();
			var collection = dbConnector.GetCollection<User> ("Users");
			var q = Query.EQ ("email", email);
			var queryResult = collection.FindOne (q);
			if (queryResult == null)
				throw new Exception ("User Not Found");
			return queryResult.Id;
		}

		bool VerifyCredentials (string email, string password)
		{
			MongoDBConnector dbConnector = new MongoDBConnector ();
			var collection = dbConnector.GetCollection<User> ("Users");
			var q = Query.EQ ("email", email);
			var queryResult = collection.FindOne (q);
			if (queryResult != null) {
				return queryResult.password == password;
			}
			return false;
		}

		public void OnRegisterConfirmButtonClick (object sender, EventArgs e)
		{
			LoginStatus.Text = "Account creation Successful";
			Cache.Insert ("currentUser", CreateAccount (EmailField.Text, PasswordField.Text, AcountTypeDropdownList.SelectedValue));
			SaveCookies ();
			Response.Redirect (@"~/Profile.aspx");
		}

		ObjectId CreateAccount (string emailField, string passwordField, string selectedValue)
		{
			MongoDBConnector dbConnector = new MongoDBConnector ();
			var collection = dbConnector.GetCollection<User> ("Users");
			var roles = dbConnector.GetCollection<Role> ("Roles");
			var q = Query.EQ ("type", selectedValue);
			var user = new User {email = emailField, password = passwordField, role = roles.FindOne(q).Id};
			collection.Insert (user);
			collection.Save (user);
			return user.Id;
		}

		public void OnCancelRegisterButtonClick (object sender, EventArgs e)
		{
			LoginFormMultiView.ActiveViewIndex = 0;
			LoginStatus.Text = "Registration Canceled";
		}

		void Page_Load (Object sender, EventArgs e)
		{
			if (!IsPostBack) {
				if (Cache.Get ("currentUser") != null) {
					LoginStatus.Text = "Cached User Exists";
					Response.Redirect (@"~/Profile.aspx");
				} else {
					if ((Request.Browser.Cookies)) { 
						if (Request.Cookies.Get ("com.xdegtyarev.kickstart.login.email") != null && 
							Request.Cookies.Get ("com.xdegtyarev.kickstart.login.password") != null) {
							if (VerifyCredentials (Request.Cookies.Get ("com.xdegtyarev.kickstart.login.email").Value, Request.Cookies.Get ("com.xdegtyarev.kickstart.login.password").Value)) {
								LoginStatus.Text = "Login from Cookies";
								Cache.Insert ("currentUser", GetUserByEmail (Request.Cookies.Get ("com.xdegtyarev.kickstart.login.email").Value));
								//No need to save cookies can extend cookie expiration
								Response.Redirect ("/Profile.aspx");
							} else {
								//Empty Cookies
							}
						}
					}
				}
				MongoDBConnector connector = new MongoDBConnector ();
				var roles = connector.GetCollection<Role> ("Roles");
				if (roles.Count () > 0) {
					List<String> l = new List<String> ();
					foreach (var o in roles.FindAll()) {
						l.Add (o.type);
					}
					AcountTypeDropdownList.DataSource = l;
					AcountTypeDropdownList.DataBind ();
				} else {
					RegisterButton.Visible = false;
				}
			}
		}

		public void SaveCookies ()
		{
			if (RememberCheckBox.Checked) { 
				if (Request.Browser.Cookies) { 
					HttpCookie cookieEmail = new HttpCookie ("com.xdegtyarev.kickstart.login.email", EmailField.Text);
					HttpCookie cookiePassword = new HttpCookie ("com.xdegtyarev.kickstart.login.password", PasswordField.Text);
					
					cookieEmail.Expires = DateTime.Now.AddDays (30);
					cookiePassword.Expires = DateTime.Now.AddDays (30);

					Response.Cookies.Add (cookieEmail);
					Response.Cookies.Add (cookiePassword);

				} 
			} 
		}
	}
}

