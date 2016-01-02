<%@ Page Language="C#" Inherits="Kickstart.Default" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<head runat="server">
	<title>Kickstart</title>
</head>
<body>
	 <div id="LoginDialog" runat="server" align="center">
        <form id="LoginForm" runat="server" style="border: 1px solid #C0C0C0; padding: 10px;
        color: #222222; background-color: #eeeeee; margin: 250px; width: 300px;">
        <div id="LoginDialogContentBar">
            <div id="Logo">
                <img alt="Kickstart" src="Resources/Logo.png" />
            </div>
            <div id="LoginFormBar">
                <asp:MultiView ID="LoginFormMultiView" runat="server" ActiveViewIndex="0">
	                <asp:View ID="EmailPasswordRegistrationView" runat="server">
	                    <div>
	                    	<div>
	                    		<asp:Label ID="EmailLabel" runat="server" Text="Email"/>
	                    	</div>
	                    	<div>
	                        	<asp:TextBox ID="EmailField" TextMode="SingleLine" runat="server" ValidationGroup="LoginInfoValidationGroup" CausesValidation="True" ToolTip="Email Address"/>
	                        </div>
	                        <div>
	                        	<asp:RegularExpressionValidator ID="EmailFieldRegExpValidator" runat="server" ErrorMessage="Invalid Email"
	                            	ControlToValidate="EmailField" ValidationExpression="^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,4})$"
	                            	ValidationGroup="LoginInfoValidationGroup" SetFocusOnError="True"/>
	                   		</div>
	                    </div>

	                    <div>
	                    	<div>
	                    		<asp:Label ID="PasswordLabel" runat="server" Text="Password"/>
	                    	</div>
							<div>
	                        	<asp:TextBox ID="PasswordField" TextMode="Password" runat="server" ValidationGroup="LoginInfoValidationGroup" CausesValidation="True" ToolTip="More than 6 symbols. Letters and Digits"/>
	                        </div>
	                        <div>
		                        <asp:RegularExpressionValidator ID="PasswordFieldRegExpValidator" runat="server"
		                            ErrorMessage="Invalid Password" ControlToValidate="PasswordField" SetFocusOnError="True"
		                            ValidationExpression="[A-z0-9-]{6,15}" ValidationGroup="LoginInfoValidationGroup"/>
	                 		</div>
	                    </div>
	                    
	                    <div>
	                    	<asp:CheckBox ID="RememberCheckBox" runat="server" Text="Remember Me"/>
	                    </div>
	                    
	                    <div id="AccountButtonsBar">
	                        <div>
	                            <asp:Panel ID="AccountButtonsPannel" runat="server">
	                                <asp:Button ID="RegisterButton" runat="server" Text="Register Now" OnClick="OnRegisterButtonClick" />
	                                <asp:Button ID="LoginButton" runat="server" Text="Login" OnClick="OnLoginButtonClick" />
	                                <div>
	                                    <asp:LinkButton ID="ForgetPasswordLinkButton" runat="server" Text="Remind?"/>
	                                </div>
	                            </asp:Panel>
	                        </div>
	                    </div>
	                </asp:View>
	                <asp:View ID="ContinueRegistrationView" runat="server">
		                <div id="AccountTypeSelectionBar">
		                    <asp:Panel ID="AccountTypeSelectionPannel" runat="server">
		                    	<asp:DropDownList ID="AcountTypeDropdownList" runat="server"/>
		                    </asp:Panel>
		                </div>  	
			            <div id="RegistrationStatusBar">
			            	<asp:Label ID="RegistartionStatusLabel" runat="server" Visible="False" />
			            </div>
			            <div id="ConfirmRegistrationButtonBar">
		                   	<asp:Button ID="ConfirmRegistrationButton" runat="server" Text="Confirm" OnClick="OnRegisterConfirmButtonClick" />
		                	<asp:Button ID="CancelRegistrationButton" runat="server" Text="Cancel" OnClick="OnCancelRegisterButtonClick" />
		                </div>
	                </asp:View>
	            </asp:MultiView>
                <asp:Label ID="LoginStatus" runat="server" Text=""/>
            </div>
        </div>
        </form>
    </div>
</body>
</html>
