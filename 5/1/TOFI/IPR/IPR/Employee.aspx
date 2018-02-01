<%@ Page Language="C#" MasterPageFile="~/BankMaster.master"%>
<asp:Content runat="server" id="EmployeeDashboard" ContentPlaceHolderID="bodyPlaceholder">
    <p>Employee</p>
    <p>Here you can see all pending credit requests:</p>
    <p>
    <asp:GridView id="EmployeeCreditRequestsGridView" EmptyDataText="No Records Found" ShowHeaderWhenEmpty="true" AutoGenerateColumns="false">
        <Columns>
           <asp:BoundField DataField="Date" HeaderText="Date"></asp:BoundField>
           <asp:BoundField DataField="Status" HeaderText="Status"></asp:BoundField>
            <asp:ButtonField></asp:ButtonField>
        </Columns>
            <EmptyDataTemplate>No Record Available</EmptyDataTemplate>
    </asp:GridView>
    </p>
</asp:Content>