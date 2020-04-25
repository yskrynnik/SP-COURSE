using System;

using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

// Документацию по шаблону элемента "Пустая страница" см. по адресу https://go.microsoft.com/fwlink/?LinkId=234238

namespace SystemProgrammingLab6
{
    /// <summary>
    /// Пустая страница, которую можно использовать саму по себе или для перехода внутри фрейма.
    /// </summary>
    public sealed partial class Calculator : Page
    {
        private int _selectedRadioButton;
        public Calculator()
        {
            this.InitializeComponent();
        }
        private void CalculateButton_Click(object sender, RoutedEventArgs e)
        {
            double x1, x2;

            Result.Text = "";

            if (!Double.TryParse(A.Text, out x1))
            {
                Result.Text = "Операнд х1 не число!";
                return;
            }

            if (!Double.TryParse(B.Text, out x2))
            {
                Result.Text = "Операнд х2 не число!";
                return;
            }

            switch (_selectedRadioButton)
            {
                case 1: Result.Text = (x1 + x2).ToString(); break;
                case 2: Result.Text = (x1 - x2).ToString(); break;
                case 3: Result.Text = (x1 * x2).ToString(); break;
                case 4: Result.Text = (x2 == 0 ? "Нельзя делить на ноль" : (x1 / x2).ToString()); break;
                case 5: Result.Text = (Math.Sin(x1)).ToString(); break;
                case 6: Result.Text = (Math.Cos(x1)).ToString(); break;
                case 7 : Result.Text = (Math.Tan(x1)).ToString(); break;
                case 8: Result.Text = (x1 < 0 ? "Значение не может быть меньше нуля!" : (Math.Sqrt(x1)).ToString()); break;
                case 9: Result.Text = (Math.Log(x1)).ToString(); break;
                case 10: Result.Text = (Math.Pow(x1, x2)).ToString(); break;
            }
        }
        private void RadioButton_Checked(object sender, RoutedEventArgs e)
        {
            RadioButton pressed = (RadioButton)sender;
            _selectedRadioButton = Convert.ToInt32(pressed.Tag);
        }
    }
}
