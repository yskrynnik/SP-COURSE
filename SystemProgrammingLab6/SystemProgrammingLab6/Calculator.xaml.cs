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
        private string _selectedRadioButton;
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
                case "x1 + x2": Result.Text = (x1 + x2).ToString(); break;
                case "x1 - x2": Result.Text = (x1 - x2).ToString(); break;
                case "x1 * x2": Result.Text = (x1 * x2).ToString(); break;
                case "x1 / x2": Result.Text = (x2 == 0 ? "Нельзя делить на ноль" : (x1 / x2).ToString()); break;
                case "sin(x1)": Result.Text = (Math.Sin(x1)).ToString(); break;
                case "cos(x1)": Result.Text = (Math.Cos(x1)).ToString(); break;
                case "tn(x1)" : Result.Text = (Math.Tan(x1)).ToString(); break;
                case "sqrt(x1)": Result.Text = (x1 < 0 ? "Значение не может быть меньше нуля!" : (Math.Sqrt(x1)).ToString()); break;
                case "log(x1)": Result.Text = (Math.Log(x1)).ToString(); break;
                case "x1 ^ x2": Result.Text = (Math.Pow(x1, x2)).ToString(); break;
            }
        }
        private void RadioButton_Checked(object sender, RoutedEventArgs e)
        {
            RadioButton pressed = (RadioButton)sender;
            _selectedRadioButton = pressed.Content.ToString();
        }
    }
}
