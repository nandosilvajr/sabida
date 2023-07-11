using AllSmart.ViewModels;

namespace AllSmart;

public partial class MainPage : ContentPage
{
    public MainPage()
    {
        BindingContext = new MainPageViewModel();
        InitializeComponent();
    }
}