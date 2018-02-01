<%@ Page Language="C#" Inherits="IPR.Default" MasterPageFile="~/BankMaster.master" %>
<asp:Content runat="server" ContentPlaceHolderID="bodyPlaceholder">
    <p>
            <p><asp:LoginStatus id="DefaultPageLoginStatus" runat="server"></asp:LoginStatus></p>
            <p><asp:LoginView id="DefaultPageLoginView" runat="server">
                    <AnonymousTemplate>
                        Welcome to TOFI Bank, please login, or register as client, for personalized information.
                    </AnonymousTemplate>
                    <LoggedInTemplate>
                        Thanks for logging in 
                        <asp:LoginName id="LoginName1" runat="Server"></asp:LoginName>.
                    </LoggedInTemplate>
                    <RoleGroups>
                        <asp:RoleGroup Roles="Admin">
                            <ContentTemplate>
                                <asp:LoginName id="LoginName2" runat="Server"></asp:LoginName>, you
                                are logged in as an administrator.
                            </ContentTemplate>
                        </asp:RoleGroup>
                    </RoleGroups>
               </asp:LoginView></p>
    
        <p>(Logged Worker only)Watch credit requests;
            <asp:Button ID="WorkerPageButton" runat="server" Text="Worker" PostBackUrl="Employee.aspx" CssClass="NavigationButtons"/>
        </p>
        <p>(Logged Client only)Watch Client dashboard
            <asp:Button ID="ClientPageButton" runat="server" Text="Client" PostBackUrl="Client.aspx" CssClass="NavigationButtons"/>
        </p>
    </p>
</asp:Content>