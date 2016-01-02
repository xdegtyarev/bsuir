<%@ Page Title="Profile" Language="C#" MasterPageFile="~/Kickstart.Master" Inherits="Kickstart.Profile" %>
<asp:Content ID="ProfileContent" ContentPlaceHolderID="ContentPlaceHolder" runat="server">
        <asp:MultiView ID="GeneralInfoMultiView" runat="server">
            <asp:View ID="GeneralInfoView" runat="server">
                <div><asp:Label ID="FullNameLabel" runat="server"/></div>
                <asp:Label ID="LocationLabel" runat="server"/>
                <asp:Button ID="EditFieldButton" runat="server" Text="Edit"/>
            </asp:View>
            <asp:View ID="GeneralInfoEdit" runat="server">
                <div>Full Name <asp:TextBox ID="FullName" runat="server"  /></div>
                <div>Email <asp:TextBox ID="Email" runat="server" /></div>
                <div>Phone <asp:TextBox ID="Phone" runat="server" /></div>
                <div>Country <asp:TextBox ID="Country" runat="server" /></div>
                <div>City <asp:TextBox ID="City" runat="server" /></div>
                <div>Address <asp:TextBox ID="AddressLine" runat="server" /></div>
                <div>
	                <asp:Button ID="SaveGeneralInfoButton" runat="server" Text="Save"/>
	                <asp:Button ID="Cancel" runat="server" Text="Cancel"/>
                </div>
            </asp:View>
        </asp:MultiView>
        <asp:MultiView ID="JobSearchInfoMultiView" runat="server">
            <asp:View ID="JobSearchView" runat="server">
                <div><asp:Label ID="JobSearchStatusLabel" runat="server"/>
                 <asp:Button ID="EditJobStatus" runat="server" Text="Edit"/>
                 </div>
            </asp:View>
            <asp:View ID="JobSearchEdit" runat="server">
                <div>Actively looking for a job<asp:CheckBox ID="LookingForAJobCheckBox" runat="server" />
                </div>
                JobTypeCheckBox Here
                <div>Will you relocate<asp:CheckBox ID="AbleToRelocateCheckBox" runat="server" /></div>
                <asp:Button ID="Button1" runat="server" Text="Save"/>
                <asp:Button ID="Button2" runat="server" Text="Cancel"/>
            </asp:View>
        </asp:MultiView>
        <asp:MultiView ID="PersonalStatementMultiView" runat="server">
            <asp:View ID="PersonalStatementView" runat="server">
            <div>
                <asp:Label ID="PersonalStatementLabel" runat="server"/>
                <asp:Button ID="Button7" runat="server" Text="Edit"/>
                </div>
            </asp:View>
            <asp:View ID="PersonalStatementEditView" runat="server">
                <div>Personal Statement<asp:TextBox ID="PersonalStatementEdit" runat="server"/></div>
                <div><asp:Button ID="Button3" runat="server" Text="Save"/></div>
                <asp:Button ID="Button4" runat="server" Text="Cancel"/>
            </asp:View>
        </asp:MultiView>
        <asp:MultiView ID="TechnologiesMultiView" runat="server">
            <asp:View ID="TechnologiesView" runat="server">
                <asp:Label ID="Technologies" runat="server" Text="Tech" />
            </asp:View>
            <asp:View ID="TechnologiesEditView" runat="server">
                <asp:TextBox ID="TechnologiesEdit" runat="server" />
                <div><asp:Button ID="Button5" runat="server" Text="Save"/></div>
                <asp:Button ID="Button6" runat="server" Text="Cancel"/>
            </asp:View>
        </asp:MultiView>
        <asp:Panel ID="ExperiencesPannel" runat="server">
                List of custom experiences
                <asp:Button ID="AddExperience" runat="server" Text="Add Experience" />
        </asp:Panel>
        <asp:Panel ID="EducationPannel" runat="server">
                List of custom educations
                <asp:Button ID="AddEducation" runat="server" Text="Add Education" />
        </asp:Panel>
</asp:Content>
