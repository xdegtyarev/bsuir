using System;
using System.Collections.Generic;
using System.IO;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using MongoDB.Bson;

namespace pr1 {

    public partial class Record : System.Web.UI.Page {
        static Models.Record record;
        List<string> genres;
        List<string> styles;
        List<string> format;
        List<string> countries;
        List<TextBox> trackTextboxes;
        bool edit;
        protected void Page_Init(object sender, EventArgs e) {
            Console.WriteLine("[Record]Init");


            if (genres == null) {
                genres = new List<string> { "Rock", "Pop", "Metal", "Folk", "Jazz" };
            }

            if (countries == null) {
                countries = new List<string> { "Belarus", "Russia", "Ukraine", "United States", "Germany" };
            }

            if (styles == null) {
                styles = new List<string> { "Heavy metal", "Grunge", "Alternative" };
            }

            if (format == null) {
                format = new List<string> { "EP", "LP", "Single" };
            }

            if (!IsPostBack) {
                var recordIdParam = Request.QueryString["id"];
                record = string.IsNullOrEmpty(recordIdParam) ? Global.Instance.recordRepository.Create() : Global.Instance.recordRepository.Read(recordIdParam);

                edit = !string.IsNullOrEmpty(Request.QueryString["edit"]);

                if (edit) {
                    RecordMultiView.ActiveViewIndex = 1;
                    SetupRecordEditView();
                } else {
                    RecordMultiView.ActiveViewIndex = 0;
                    SetupRecordView();
                }
            }
        }

        void SetupRecordView() {
            ViewRecordId.Text = record.id;
            ViewRecordArtist.Text = record.artist;
            ViewRecordTitle.Text = record.title;
            ViewRecordLabel.Text = record.label;
            ViewRecordContry.Text = record.country;
            ViewRecordFormat.Text = record.format;
            ViewRecordGenre.Text = record.genre;
            ViewRecordStyle.Text = record.style;
            ViewRecordCover.ImageUrl = string.IsNullOrEmpty(record.coverURL) ? "/Content/default-release.png" : record.coverURL;
            ViewRecordRating.Text = record.ratings.ToString();
        }

        void SetupRecordEditView() {
            EditRecordGenre.Items.Clear();
            foreach (var o in genres) {
                EditRecordGenre.Items.Add(o);
            }
            EditRecordStyle.Items.Clear();
            foreach (var o in styles) {
                EditRecordStyle.Items.Add(o);
            }
            EditRecordFormat.Items.Clear();
            foreach (var o in format) {
                EditRecordFormat.Items.Add(o);
            }
            EditRecordCountry.Items.Clear();
            foreach (var o in countries) {
                EditRecordCountry.Items.Add(o);
            }

            EditRecordFormat.SelectedIndex = string.IsNullOrEmpty(record.format) ? 0 : format.IndexOf(record.format);
            EditRecordStyle.SelectedIndex = string.IsNullOrEmpty(record.style) ? 0 : styles.IndexOf(record.style);
            EditRecordGenre.SelectedIndex = string.IsNullOrEmpty(record.genre) ? 0 : genres.IndexOf(record.genre);
            EditRecordCountry.SelectedIndex = string.IsNullOrEmpty(record.country) ? 0 : countries.IndexOf(record.country);

            EditRecordCover.ImageUrl = string.IsNullOrEmpty(record.coverURL) ? "/Content/default-release.png" : record.coverURL;

            EditRecordId.Text = "Editing record: " + record.id;
            EditRecordTitle.Text = record.title;
            EditRecordArtist.Text = record.artist;
            EditRecordLabel.Text = record.label;
            //tracklist
            UpdateTracklistView();
        }

        void UpdateTracklistView() {
            //Clear
            if (trackTextboxes != null) {
                foreach (var o in trackTextboxes) {
                    if (EditRecordTracksPanel.Controls.Contains(o)) {
                        EditRecordTracksPanel.Controls.Remove(o);
                        o.Dispose();
                    }
                }
                trackTextboxes.Clear();
            } else {
                trackTextboxes = new List<TextBox>();
            }

            if (record.tracklist == null) {
                record.tracklist = new List<string>();
            }

            for (int i = 0; i < record.tracklist.Count; i++) {
                var trackLabelControl = new TextBox();
                trackLabelControl.ID = "EditRecordTrack" + i.ToString() + "Label";
                trackLabelControl.Text = string.IsNullOrEmpty(record.tracklist[i]) ? "" : record.tracklist[i];
                trackLabelControl.ValidationGroup = "EditRecordGroup";
                EditRecordTracksPanel.Controls.Add(new LiteralControl("<li>" + (i + 1)));
                trackTextboxes.Add(trackLabelControl);
                EditRecordTracksPanel.Controls.Add(trackLabelControl);
                EditRecordTracksPanel.Controls.Add(new LiteralControl("</li>"));
            }

        }

        public void EditRecordAddTrack_Click(object sender, EventArgs args) {
            Console.WriteLine("[Record]CreateTracklist click");
            var trackCount = int.Parse(EditRecordTrackCount.Text);
            record.tracklist = record.tracklist ?? new List<string>();
            var temp = record.tracklist;

            record.tracklist = new List<string>();

            for (int i = 0; i < trackCount; i++) {
                if (temp != null && i < temp.Count) {
                    record.tracklist.Add(temp[i]);
                } else {
                    record.tracklist.Add(i.ToString());
                }
            }

            UpdateTracklistView();
        }

        public void SaveRecordButton_Click(object sender, EventArgs args) {
            Console.WriteLine("[Record]Save record button click");

            record.title = EditRecordTitle.Text;
            record.artist = EditRecordArtist.Text;
            record.label = EditRecordLabel.Text;

            record.format = format[EditRecordFormat.SelectedIndex];
            record.style = styles[EditRecordStyle.SelectedIndex];
            record.genre = genres[EditRecordGenre.SelectedIndex];
            record.country = countries[EditRecordCountry.SelectedIndex];

            DateTime dateTime = DateTime.UtcNow;
            if (DateTime.TryParse(EditRecordYear.Text, out dateTime)) {
                record.releaseDate = dateTime;
            }

            record.tracklist = new List<string>();
            if (trackTextboxes != null) {
                foreach (var o in trackTextboxes) {
                    record.tracklist.Add(o.Text);
                }
            }
            Global.Instance.recordRepository.Update(record.id, record);
            Response.Redirect("/Record.aspx?id=" + record.id);
        }

        public void ResetRecordButton_Click(object sender, EventArgs args) {
            Console.WriteLine("[Record]Reset record button click");
            SetupRecordEditView();
        }

        public void DeleteRecordButton_Click(object sender, EventArgs args) {
            Console.WriteLine("[Record]Delete record button click");
            Global.Instance.recordRepository.Delete(record.id);
            Response.Redirect("/Default.aspx");
        }

        public void ViewRecordDown_Click(object sender, EventArgs args) {
            Console.WriteLine("[Record]ViewRecordDown");
            record.ratings--;
            Global.Instance.recordRepository.UpdateRatings(record.id, record.ratings);
            SetupRecordView();
        }

        public void ViewRecordUp_Click(object sender, EventArgs args) {
            Console.WriteLine("[Record]ViewRecordUp");
            record.ratings++;
            Global.Instance.recordRepository.UpdateRatings(record.id, record.ratings);
            SetupRecordView();
        }

        public void ViewRecordEdit_Click(object sender, EventArgs args) {
            Console.WriteLine("[Record]ViewRecordEdit");
            Response.Redirect("/Record.aspx?id=" + record.id + "&edit=1");
        }

        public void UploadButton_Click(object sender, EventArgs args) {
            Console.WriteLine("[Record]Upload button clicked");
            if (EditRecordFileUpload.HasFile) {
                try {
                    var filename = ObjectId.GenerateNewId();
                    var finalUrl = "/Content/" + filename;
                    EditRecordFileUpload.SaveAs(Server.MapPath("~/Content/") + filename);
                    EditRecordFileUploadStatus.Text = "Upload status: File uploaded!";
                    record.coverURL = finalUrl;
                    EditRecordCover.ImageUrl = record.coverURL;
                } catch (Exception ex) {
                    EditRecordFileUploadStatus.Text = "Upload filed: The file could not be uploaded. The following error occured: " + ex.Message;
                }
            }
        }
    }
}
