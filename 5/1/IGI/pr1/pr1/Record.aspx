<%@ Page MasterPageFile="~/MusicdropMaster.master" Language="C#" Inherits="pr1.Record" EnableViewState="true"%>
<asp:Content id="RecordContent" ContentPlaceHolderID="ContentPlaceHolder" runat="server" EnableViewState="true">
<asp:MultiView id="RecordMultiView" runat="server" ActiveViewIndex="1">
<asp:View id="RecordViewForm" runat="server">
    <div id="RecordViewForm">
        <ul>
            <li>
                <b>Viewing Record</b><asp:Label id="ViewRecordId" runat="server"/>
                <asp:Button id="ViewRecordEditButton" Text="Edit" runat="server" OnClick="ViewRecordEdit_Click"/>
                <asp:Button id="ViewRecordDeleteButton" Text="Delete" runat="server" OnClick="DeleteRecordButton_Click"/>
            </li>

            <br/>
            <li>
                Rating:
                <asp:Label id="ViewRecordRating" runat="server"/>
                <asp:Button id="ViewRecordUp" Text="+" runat="server" OnClick="ViewRecordUp_Click"/>
                <asp:Button id="ViewRecordDown" Text="-" runat="server" OnClick="ViewRecordDown_Click"/>
            </li>

            <br/>
            <asp:ImageButton id="ViewRecordCover" ImageUrl="/Content/default-release.png" runat="server" ImageAlign="Left" Width="150px"/>
            <li><b>Artist:</b><asp:Label id="ViewRecordArtist" runat="server"></asp:Label></li>
            <li><b>Title:</b><asp:Label id="ViewRecordTitle" runat="server"></asp:Label></li>
            <li><b>Label:</b><asp:Label id="ViewRecordLabel" runat="server"></asp:Label></li>
            <li><b>Genre:</b><asp:Label id="ViewRecordGenre" runat="server"></asp:Label></li>
            <li><b>Style:</b><asp:Label id="ViewRecordStyle" runat="server"></asp:Label></li>
            <li><b>Format:</b><asp:Label id="ViewRecordFormat" runat="server"></asp:Label></li>
            <li><b>Contry:</b><asp:Label id="ViewRecordContry" runat="server"></asp:Label></li>
        </ul>
    </div>
</asp:View>

<asp:View id="RecordEditView" runat="server">
<div id="RecordEditForm">
    <ul>
        <li><asp:Label id="EditRecordId" runat="server"><b>Editing Record</b></asp:Label></li>
        <br/>

        <li>
            <asp:FileUpload id="EditRecordFileUpload" FileContent="image/jpeg" runat="server" />
            <asp:Button runat="server" id="EditRecordFileUploadButton" text="Upload" onclick="UploadButton_Click" />
            <asp:Label runat="server" id="EditRecordFileUploadStatus" text="Upload status: "/>
        </li>

        <div><asp:ImageButton id="EditRecordCover" ImageUrl="/Content/default-release.png" runat="server" ImageAlign="Left" Width="100px"/></div>

        <li><asp:TextBox id="EditRecordTitle" ValidationGroup="EditRecordGroup" placeholder="Title" TextMode="SingleLine" runat="server"/></li>
        <li><asp:RequiredFieldValidator id="EditRecordUsernameValidator" ValidationGroup="EditRecordGroup" ControlToValidate="EditRecordTitle" Display="Dynamic" ErrorMessage="Title is required" runat="server"/></li>

        <li><asp:TextBox id="EditRecordArtist" ValidationGroup="EditRecordGroup" placeholder="Artist" TextMode="SingleLine" runat="server"/></li>
        <li><asp:RequiredFieldValidator id="EditRecordArtistValidator" ValidationGroup="EditRecordGroup" ControlToValidate="EditRecordArtist" Display="Dynamic" ErrorMessage="Artist is required" runat="server"/></li>

        <li><asp:TextBox id="EditRecordLabel" ValidationGroup="EditRecordGroup" placeholder="Label" TextMode="SingleLine" runat="server"/></li>
        <li><asp:RequiredFieldValidator id="EditRecordLabelValidator" ValidationGroup="EditRecordGroup" ControlToValidate="EditRecordLabel" Display="Dynamic" ErrorMessage="Label is required (or use N/A)" runat="server"/></li>

        <li><asp:TextBox id="EditRecordYear" ValidationGroup="EditRecordGroup" placeholder="Release date (DD/MM/YYYY)" runat="server"/></li>
        <li><asp:TextBox id="EditRecordOther" ValidationGroup="EditRecordGroup" placeholder="Other" runat="server"/></li>

        <li>Genre<asp:DropDownList id="EditRecordGenre" runat="server"/></li>
        <li>Style<asp:DropDownList id="EditRecordStyle" runat="server"/></li>
        <li>Format<asp:DropDownList id="EditRecordFormat" runat="server"/></li>
        <li>Country<asp:DropDownList id="EditRecordCountry" runat="server"/></li>

        <ul><asp:Panel id="EditRecordTracksPanel" runat="server"/></ul>
        <li>TrackCount:
            <asp:RangeValidator id="EditRecordTrackCountValidator" ValidationGroup="AddTracksValidation" MinimumValue="1" MaximumValue="100"  Type="Integer" ControlToValidate="EditRecordTrackCount" Display="Dynamic" ErrorMessage="Number it not in Range(0,100)" runat="server"/>
            <asp:TextBox id="EditRecordTrackCount" ValidationGroup="AddTracksValidation"  Text="0" runat="server"/>
            <asp:Button id="EditRecordAddTrack" ValidationGroup="AddTracksValidation" runat="server" Text="Create Tracklist" OnClick="EditRecordAddTrack_Click"/>
        </li>

        <li>
            <asp:Button id="EditRecordButton" ValidationGroup="EditRecordGroup" Text="Save" runat="server" OnClick="SaveRecordButton_Click"/>
            <asp:Button id="ResetRecordButton" CausesValidation="false" Text="Reset" runat="server" OnClick="ResetRecordButton_Click"/>
            <asp:Button id="DeleteRecordButton" CausesValidation="false" Text="Delete" runat="server" OnClick="DeleteRecordButton_Click"/>
        </li>
    </ul>

</div>
</asp:View>

</asp:MultiView>
</asp:Content>