using CommunityToolkit.Mvvm.ComponentModel;

namespace AllSmart.ViewModels
{
    public partial class BaseViewModel : ObservableObject
    {
        [ObservableProperty] private bool _isRefreshing;
        [ObservableProperty] private bool _isBusy;
    }
}