using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

// Документацию по шаблону элемента "Пустая страница" см. по адресу https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x419

namespace SystemProgrammingLab6
{
    /// <summary>
    /// Пустая страница, которую можно использовать саму по себе или для перехода внутри фрейма.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
        }
        private void NavigationView_OnSelectionChanged(NavigationView sender, NavigationViewSelectionChangedEventArgs args)
        {
            switch (((NavigationViewItem)args.SelectedItem).Tag.ToString())
            {
                case "AboutDeveloper":
                    Frame.Navigate(typeof(AboutDeveloper));
                    break;
                case "Calculator":
                    Frame.Navigate(typeof(Calculator));
                    break;
                default: break;
            }
        }
        private void MainPage_OnLoaded(object sender, RoutedEventArgs e)
        {
            AboutDeveloper.IsSelected = true;
        }
    }
}
