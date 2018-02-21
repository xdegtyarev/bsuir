<%@ Page MasterPageFile="~/MusicdropMaster.master" Language="C#" Inherits="pr1.Profile" EnableViewState="true"%>
<asp:Content id="ProfileContent" ContentPlaceHolderID="ContentPlaceHolder" runat="server" EnableViewState="true">
    <ul>
        <li>ID:<asp:Label id="ProfileId" runat="server"/></li>
        <li>Name:<asp:Label id="ProfileName" runat="server"/></li>
        <li>Email:<asp:Label id="ProfileEmail" runat="server"/></li>
    </ul>
</asp:Content>