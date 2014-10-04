using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace image_viewer {

  public partial class MainWindow : Window {
    public struct Image {
      public string path;
      public string filename;
      public BitmapImage bitmap;

      public Image(string path) {
        this.path = path;
        this.filename = System.IO.Path.GetFileName(path);
        this.bitmap = new BitmapImage(new Uri(path));
      }

      public override string ToString() {
        return this.filename;
      }
    }

    public MainWindow() {
      InitializeComponent();
    }

    private void AddButton_Click(object sender, RoutedEventArgs e) {
      Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();
      dlg.DefaultExt = ".jpg";
      dlg.Filter = "Images|*.jpg";

      Nullable<bool> result = dlg.ShowDialog();
      if (result == true) {
        string path = dlg.FileName;
        ImageList.Items.Add(new Image(path));
      }
    }

    private void ImageList_MouseDoubleClick(object sender, MouseButtonEventArgs e) {
      int index = ImageList.SelectedIndex;
      if (index != -1) {
        ImageView.Source = ((Image) ImageList.Items[index]).bitmap;
      }
    }

    private void DeleteButton_Click(object sender, RoutedEventArgs e) {
      int index = ImageList.SelectedIndex;
      if (index != -1) {
        ImageList.Items.RemoveAt(index);
      }
    }

  }
}
