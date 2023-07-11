using System.Windows.Input;
using AllSmart.Services;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using Microsoft.AspNetCore.Components;

namespace AllSmart.ViewModels;

public partial class MainPageViewModel : ObservableObject
{

    public ICommand RefreshCommand { get; }

    [ObservableProperty] private int sensorData;

    [Inject] private IWebServiceApi WebServiceApi { get; set; }
    public MainPageViewModel()
    {
        RefreshCommand = new RelayCommand(RefreshPage);
    }

    private static void RefreshPage()
    {
        throw new NotImplementedException();
    }
}