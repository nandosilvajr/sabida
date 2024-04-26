using System.Windows.Input;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;

namespace AllSmart.ViewModels
{
    public partial class BaseViewModel : ObservableObject
    {

        [ObservableProperty] private bool _isRefreshing;
        
        [RelayCommand]
        void Refresh()
        {
            IsRefreshing = false;
        }

    }
}