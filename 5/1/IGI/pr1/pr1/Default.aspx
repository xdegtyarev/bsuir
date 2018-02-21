<%@Page MasterPageFile="~/MusicdropMaster.master" Language="C#" Inherits="pr1.Default" EnableViewState="true"%>
<asp:Content id="DefaultContent" ContentPlaceHolderID="ContentPlaceHolder" runat="server" EnableViewState="true">
    <asp:MultiView id="DefaultPageMultiView" runat="server">
        <asp:View id="DefaultPageAnonView" runat="server">
        <div id="RegisterFormDescription">
            <p>Discover new music.</p>
            <p>Track your collection and wishlist.</p>
            <p>Contribute to the database.</p>
            <p>Buy & Sell records in the Marketplace.</p>
        </div>

        <div id="RegisterForm">
                <ul>
                    <li><asp:Label runat="server"><b>Don't have and account?</b></asp:Label></li>
                    <li><asp:TextBox SkinID="RegisterTextBox" id="RegisterUsername" ValidationGroup="RegisterGroup" placeholder="Choose a username" TextMode="SingleLine" runat="server"/></li>
                    <li><asp:RequiredFieldValidator id="RegisterUsernameValidator" ValidationGroup="RegisterGroup" ControlToValidate="RegisterUsername" Display="Dynamic" ErrorMessage="Username is required" runat="server"/></li>
                    <li><asp:TextBox SkinID="RegisterTextBox" id="RegisterEmail" ValidationGroup="RegisterGroup" placeholder="Enter your email" TextMode="Email" runat="server"/></li>
                    <li><asp:RequiredFieldValidator id="RegisterEmailValidator" ValidationGroup="RegisterGroup" ControlToValidate="RegisterEmail" Display="Dynamic" ErrorMessage="Email is required" runat="server"/></li>
                    <li><asp:TextBox SkinID="RegisterTextBox" id="RegisterPassword" ValidationGroup="RegisterGroup" placeholder="Choose a password" TextMode="Password" runat="server"/></li>
                    <li><asp:RequiredFieldValidator id="RegisterPasswordValidor" ValidationGroup="RegisterGroup" ControlToValidate="RegisterPassword" Display="Dynamic" ErrorMessage="Password is required" runat="server"/></li>
                    <li><asp:CheckBox id="RegisterAcceptsTerms" ValidationGroup="RegisterGroup" runat="server"/><asp:Label runat="server">I accept Terms of Service</asp:Label></li>
                    <li><asp:Button id="RegisterButton" ValidationGroup="RegisterGroup" Text="Create account" runat="server" OnClick="RegisterButton_Click"/></li>
                </ul>
        </div>
        </asp:View>

        <asp:View id="DefaultPageMemberView" runat="server">
            <asp:Label id="DefaultPageMemberViewGreeting" runat="server"/>
        </asp:View>
    </asp:MultiView>
    <asp:Label id="DefaultSearchReqest" runat="server"/>
        <asp:DataList id="DefaultPageRecordList" RepeatColumns="5" runat="server">
                <ItemTemplate id="DefaultPageRecordListItem" runat="server">
                    <div style="width: 150px; height: 200px; text-align: center; border: thin solid #000000;">
                        <asp:ImageButton id="DefaultPageRecordListItemImage" CausesValidation="false" PostBackUrl=<%# "/Record.aspx?id="+Eval("id")%> runat="server" Width="150px" ImageUrl=<%#GetCoverArt(Eval("coverURL"))%> />
                        <br/><b><asp:Label id="DefaultPageRecordListItemTitle"  runat="server" Text=<%# Eval("title")%>/></b>
                        <br/><asp:Label id="DefaultPageRecordListItemArtist"    runat="server" Text=<%# Eval("artist")%>/>
                    </div>
                </ItemTemplate>
        </asp:DataList>
</asp:Content>