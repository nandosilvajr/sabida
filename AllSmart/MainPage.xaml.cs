using AllSmart.Services;
using AllSmart.ViewModels;

namespace AllSmart;

public partial class MainPage : ContentPage
{
    public MainPage(MainPageViewModel vm)
    {
        InitializeComponent();
        BindingContext = vm;
    }
}