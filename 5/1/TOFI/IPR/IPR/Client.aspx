<%@ Page Language="C#" Inherits="IPR.Default" MasterPageFile="~/BankMaster.master" %>
<asp:Content runat="server" ContentPlaceHolderID="bodyPlaceholder">
   <asp:Button ID="ClientCreaditRequestButton" runat="server" Text="Request Credit" PostBackUrl="Client.aspx" CssClass="NavigationButtons"/>
   <asp:Button ID="ClientContactButton" runat="server" Text="Contact Bank" PostBackUrl="Client.aspx" CssClass="NavigationButtons"/>

   <p>Hello Client</p>
   <p>Amount to pay:</p>

   <div>
        <p>Current Credits:</p>
        <asp:GridView runat="server" id="ClientCreditGridView" ShowHeaderWhenEmpty="true" AutoGenerateColumns="false">
        <Columns>
           <asp:BoundField DataField="Date" HeaderText="Date"></asp:BoundField>
           <asp:BoundField DataField="Status" HeaderText="Status"></asp:BoundField>
            <asp:ButtonField></asp:ButtonField>
        </Columns>
        <EmptyDataTemplate>No Record Available</EmptyDataTemplate> 
    </asp:GridView>
   </div>


   <div id="CreditRequestForm">
        <p>Credit request form</p>
        <p>Amount</p>
        <p><asp:TextBox runat="server" id="CreditRequestFormAmount"></asp:TextBox></p>
        <p>Guarantor</p>
        <asp:CheckBox id="CreditRequestFormGuarantor" Text="Guarantor"></asp:CheckBox>
        <p>Grace period</p>
        <asp:CheckBox id="CreditRequestFormGrace" Text="Grace period"></asp:CheckBox>
        <p>Term</p>
        <asp:DropDownList Text="Term" id="CreditRequestFormTerm" runat="server">
                <asp:ListItem>ANY</asp:ListItem>
                <asp:ListItem>3 Month</asp:ListItem>
                <asp:ListItem>6 Month</asp:ListItem>
                <asp:ListItem>1 Year</asp:ListItem>
                <asp:ListItem>2 Years</asp:ListItem>
                <asp:ListItem>3 Years</asp:ListItem>
                <asp:ListItem>4 Years</asp:ListItem>
                <asp:ListItem>5 Years</asp:ListItem>
                <asp:ListItem>5+ Years</asp:ListItem>
        </asp:DropDownList>
        <p>Currency</p>
        
        <p><asp:DropDownList Text="Currency" id="CreditRequestFormCurrency" runat="server">
             <asp:ListItem>BYN</asp:ListItem>
             <asp:ListItem>USD</asp:ListItem>
             <asp:ListItem>EUR</asp:ListItem>
             <asp:ListItem>RUB</asp:ListItem>
        </asp:DropDownList></p>
        
        <p><asp:Button id="CreditRequestFormCalculateButton" Text="Calculate" OnClick="OnCalculateCredit" CssClass="NavigationButtons"/></p>

        <div id="CreditRequestFormCalculations">
        <p>Credit Sum</p>
            <p>0</p>
        <p>Overpayment</p>
            <p>0</p>
        <p>Rate</p>
            <p>0</p>
        <p>Effective rate</p>
            <p>0</p>
        <asp:Button id="CreditRequestFormApplyButton" Text="Apply" CssClass="NavigationButtons"/>
        </div>
   </div>
    
</asp:Content>